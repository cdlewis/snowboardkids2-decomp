#!/usr/bin/env python3

import argparse
import hashlib
import json
import subprocess
import sys
import os
import signal
import shlex
from datetime import datetime
from pathlib import Path
from glob import glob

# ANSI color codes
class Colors:
    CYAN = '\033[96m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    MAGENTA = '\033[95m'
    BOLD = '\033[1m'
    RESET = '\033[0m'

class TaskRunner:
    def __init__(self, taskfile_path, max_times=None, dry_run=False, verbose=False, hash_filter=None):
        self.taskfile_path = taskfile_path
        self.max_times = max_times
        self.dry_run = dry_run
        self.verbose = verbose
        self.hash_filter = hash_filter  # 'evens', 'odds', or None
        self.stop_requested = False
        self.count = 0
        self.consecutive_failures = 0

        # Load taskfile
        with open(taskfile_path, 'r') as f:
            self.taskfile = json.load(f)

        # Validate taskfile structure
        allowed_fields = {'task_name', 'script', 'prompt', 'template', 'claude_command', 'accept_best_effort'}
        unexpected_fields = set(self.taskfile.keys()) - allowed_fields
        if unexpected_fields:
            raise ValueError(f"Unexpected fields in taskfile: {', '.join(sorted(unexpected_fields))}")

        self.task_name = self.taskfile['task_name']
        self.script = self.taskfile['script']

        # Handle prompt vs template
        has_prompt = 'prompt' in self.taskfile
        has_template = 'template' in self.taskfile

        if has_prompt and has_template:
            raise ValueError("Task file cannot have both 'prompt' and 'template' fields")

        if not has_prompt and not has_template:
            raise ValueError("Task file must have either 'prompt' or 'template' field")

        if has_template:
            template_path = self.taskfile['template']
            if not os.path.isfile(template_path):
                raise ValueError(f"Template file not found or not a valid file: {template_path}")

            with open(template_path, 'r') as f:
                self.prompt_template = f.read()
        else:
            self.prompt_template = self.taskfile['prompt']

        self.claude_command = self.taskfile.get('claude_command', 'claude')
        self.accept_best_effort = self.taskfile.get('accept_best_effort', False)

        # Setup log files
        log_dir = Path("tools/task-runner/logs")
        log_dir.mkdir(parents=True, exist_ok=True)

        self.ignored_prompts_file = log_dir / f"ignored_prompts_{self.task_name}.log"
        self.claude_log_file = log_dir / f"claude_logs_{self.task_name}.log"

        # Ensure log files exist
        Path(self.ignored_prompts_file).touch()

        # Setup signal handler for graceful shutdown
        signal.signal(signal.SIGINT, self._signal_handler)

    def _signal_handler(self, sig, frame):
        print("\nInterrupt received, will stop after current iteration...")
        self.stop_requested = True

    def _run_script(self):
        """Run the script and return JSON array of strings."""
        try:
            result = subprocess.run(
                self.script,
                shell=True,
                capture_output=True,
                text=True,
                check=True
            )
            return json.loads(result.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Error running script: {e}")
            print(f"stderr: {e.stderr}")
            return []
        except json.JSONDecodeError as e:
            print(f"Error parsing script output as JSON: {e}")
            print(f"stdout: {result.stdout}")
            return []

    def _load_ignored_prompts(self):
        """Load the set of ignored prompts."""
        if not os.path.exists(self.ignored_prompts_file):
            return set()

        with open(self.ignored_prompts_file, 'r') as f:
            return set(line.strip() for line in f if line.strip())

    def _add_to_ignored(self, prompt_arg):
        """Add a prompt argument to the ignored list."""
        with open(self.ignored_prompts_file, 'a') as f:
            f.write(f"{prompt_arg}\n")

    def _get_candidate_key(self, candidate):
        """Get the key used for tracking a candidate in the ignored list.

        For string candidates, the key is the string itself.
        For array candidates, the key is the first element.
        """
        if isinstance(candidate, list):
            return candidate[0] if candidate else None
        return candidate

    def _filter_by_hash(self, candidates):
        """Filter candidates based on hash parity.

        Uses the candidate key (function name) to compute a deterministic hash,
        then filters to only even or odd hashes based on self.hash_filter.

        Note: We use MD5 instead of Python's built-in hash() because hash() is
        randomized per-process (PYTHONHASHSEED), which would cause --odds and
        --evens to work on the same functions when run in separate processes.
        """
        if not self.hash_filter:
            return candidates

        filtered = []
        for candidate in candidates:
            key = self._get_candidate_key(candidate)
            if key:
                # Use MD5 for deterministic hashing across processes
                h = int(hashlib.md5(key.encode()).hexdigest(), 16)
                is_even = (h % 2) == 0
                if self.hash_filter == 'evens' and is_even:
                    filtered.append(candidate)
                elif self.hash_filter == 'odds' and not is_even:
                    filtered.append(candidate)

        return filtered

    def _interpolate_prompt(self, template, candidate):
        """Interpolate the prompt template with the candidate.

        For string candidates:
            - $ARGUMENT is replaced with the string

        For array candidates:
            - $ARGUMENT_1, $ARGUMENT_2, etc. are replaced with array elements
            - $ARGUMENT is replaced with the first element (backwards compat)
            - $REMAINING_ARGUMENTS is replaced with a comma-separated list of elements 2+
        """
        if isinstance(candidate, list):
            prompt = template
            # Replace indexed arguments ($ARGUMENT_1, $ARGUMENT_2, etc.)
            for i, arg in enumerate(candidate, start=1):
                prompt = prompt.replace(f'$ARGUMENT_{i}', arg if arg else '')
            # Replace $REMAINING_ARGUMENTS with comma-separated list of elements 2+
            remaining = candidate[1:] if len(candidate) > 1 else []
            prompt = prompt.replace('$REMAINING_ARGUMENTS', ', '.join(remaining))
            # Backwards compatibility: $ARGUMENT refers to first element
            prompt = prompt.replace('$ARGUMENT', candidate[0] if candidate else '')
            return prompt
        else:
            return template.replace('$ARGUMENT', candidate)

    def _select_argument(self, candidates):
        """Select the first candidate not in ignored list.

        Candidates can be either strings or arrays of strings.
        For arrays, the first element is used as the key for the ignored list.
        """
        ignored = self._load_ignored_prompts()

        for candidate in candidates:
            key = self._get_candidate_key(candidate)
            if key and key not in ignored:
                return candidate

        return None

    def _run_claude(self, prompt):
        """Run Claude with the given prompt and log output."""
        # Properly escape the prompt for shell safety
        command = f'{self.claude_command} -p {shlex.quote(prompt)}'

        if self.verbose:
            print(f"Running: {command}")
        else:
            print(f"Running: {self.claude_command} -p <prompt>")

        try:
            # Temporarily ignore SIGINT in the parent process during subprocess execution
            # This allows the subprocess to complete before we check stop_requested
            old_handler = signal.signal(signal.SIGINT, signal.SIG_IGN)

            result = subprocess.run(
                command,
                shell=True,
                capture_output=True,
                text=True
            )

            # Restore the original signal handler
            signal.signal(signal.SIGINT, old_handler)

            output = result.stdout + result.stderr

            # Log to file
            with open(self.claude_log_file, 'a') as f:
                f.write(f"\n{'='*80}\n")
                f.write(f"Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
                f.write(f"Prompt: {prompt}\n")
                f.write(f"{'='*80}\n")
                f.write(output)
                f.write(f"\n{'='*80}\n\n")

            print(output)

            return result.returncode, output
        except Exception as e:
            # Restore signal handler in case of exception
            signal.signal(signal.SIGINT, self._signal_handler)
            print(f"Error running Claude: {e}")
            return 1, str(e)

    def _run_build_verify(self):
        """Run build-and-verify.sh script."""
        try:
            # Run build and verify
            result = subprocess.run(
                ['bash', './tools/build-and-verify.sh'],
                capture_output=True,
                text=True
            )

            if result.returncode != 0:
                print("Build verification failed!")
                return False

            return True
        except Exception as e:
            print(f"Error running build-and-verify.sh: {e}")
            return False

    def run(self):
        """Main execution loop."""
        print(f"Starting task runner for: {self.task_name}")
        if self.dry_run:
            print("MODE: DRY RUN (will not execute Claude)")
        print(f"Script: {self.script}")
        if self.verbose:
            print(f"Prompt template: {self.prompt_template}")
        print(f"Ignored prompts file: {self.ignored_prompts_file}")
        print(f"Claude log file: {self.claude_log_file}")
        print()

        # Verify claude command exists (skip in dry-run mode)
        if not self.dry_run:
            try:
                result = subprocess.run(
                    f'{self.claude_command} --version',
                    shell=True,
                    capture_output=True,
                    timeout=5
                )
                if result.returncode != 0:
                    print(f"Error: Claude command '{self.claude_command}' not found or not working.")
                    print("Please ensure claude is installed and accessible.")
                    return
            except (subprocess.TimeoutExpired, FileNotFoundError, Exception) as e:
                print(f"Error: Cannot execute claude command '{self.claude_command}': {e}")
                print("Please ensure claude is installed and accessible.")
                return

        try:
            self._run_loop()
        except KeyboardInterrupt:
            print("\nInterrupt received, stopping gracefully...")
            self.stop_requested = True

        print(f"\nTotal iterations: {self.count}")

    def _run_loop(self):
        """Internal loop that can be interrupted."""
        while True:
            if self.stop_requested:
                print("Stopping gracefully.")
                break

            if self.max_times is not None and self.count >= self.max_times:
                print(f"Reached maximum iterations: {self.max_times}")
                break

            # Print visually distinct iteration header
            print()  # blank line before
            print(f"{Colors.BOLD}{Colors.CYAN}{'='*70}{Colors.RESET}")
            print(f"{Colors.BOLD}{Colors.MAGENTA}🚀 Starting iteration {self.count + 1}{Colors.RESET} {Colors.YELLOW}at {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}{Colors.RESET}")
            print(f"{Colors.BOLD}{Colors.CYAN}{'='*70}{Colors.RESET}")
            print()  # blank line after

            # Run script to get candidates
            candidates = self._run_script()

            if not candidates:
                print("No candidates returned from script.")
                break

            # Apply hash filter for parallel execution
            candidates = self._filter_by_hash(candidates)

            if not candidates:
                print("No candidates after hash filter.")
                break

            print(f"Script returned {len(candidates)} candidate(s)")

            # Select first non-ignored argument
            selected = self._select_argument(candidates)

            if selected is None:
                print("All candidates have been processed (in ignored list).")
                break

            print(f"Selected argument: {selected}")

            # Interpolate prompt
            prompt = self._interpolate_prompt(self.prompt_template, selected)

            # Dry run mode - just print the prompt and exit
            if self.dry_run:
                print(f"\n{'='*80}")
                print("DRY RUN - Would execute Claude with prompt:")
                print(f"{'='*80}")
                print(prompt)
                print(f"{'='*80}\n")
                break

            # Run Claude
            exit_code, output = self._run_claude(prompt)

            # If claude command failed to execute (not found), abort immediately
            if "No such file or directory" in output or "command not found" in output:
                print(f"Error: Claude command '{self.claude_command}' not found. Aborting.")
                break

            if self.stop_requested:
                print("Stopping gracefully.")
                break

            # Re-run script to check if same argument appears
            new_candidates = self._run_script()
            new_candidates = self._filter_by_hash(new_candidates)

            # Get the key for the selected candidate (first element if array, string otherwise)
            selected_key = self._get_candidate_key(selected)

            # Check if the key appears in any of the new candidates
            new_candidate_keys = {self._get_candidate_key(c) for c in new_candidates}

            if selected_key in new_candidate_keys:
                # Issue still exists
                print(f"Argument '{selected_key}' still appears in script output.")

                if self.accept_best_effort:
                    # Accept best effort - verify build and commit if successful
                    print("Best effort mode: attempting to verify and commit changes...")

                    if not self._run_build_verify():
                        print("Build verification failed. Resetting changes.")
                        subprocess.run(['git', 'reset', '--hard', 'HEAD'], check=False)

                        # Verify we can still build after reset
                        if not self._run_build_verify():
                            print("Cannot recover to a good state. Stopping.")
                            break
                    else:
                        # Build passed - check if there are uncommitted changes to commit
                        result = subprocess.run(
                            ['git', 'diff', '--quiet'],
                            capture_output=True
                        )

                        if result.returncode != 0:
                            # There are uncommitted changes, commit them
                            print("Committing best effort changes...")
                            try:
                                subprocess.run(['git', 'add', '-A'], check=True)
                                subprocess.run(
                                    ['git', 'commit', '-m', f'task-runner: best effort {selected_key}'],
                                    check=True
                                )
                                print("Best effort changes committed successfully.")
                            except subprocess.CalledProcessError as e:
                                print(f"Warning: Could not commit changes: {e}")
                        else:
                            print("No uncommitted changes to commit.")

                    print(f"Adding '{selected_key}' to ignored list.")
                    self._add_to_ignored(selected_key)
                else:
                    # Standard mode - reset git to clean up Claude's changes
                    subprocess.run(['git', 'reset', '--hard', 'HEAD'], check=False)

                    # Verify build still works
                    if not self._run_build_verify():
                        print("Build verification failed, stopping.")
                        break

                    print(f"Adding '{selected_key}' to ignored list.")
                    self._add_to_ignored(selected_key)
            else:
                # Issue was fixed! Verify build and commit if successful
                print(f"Argument '{selected_key}' no longer appears in script output.")

                # Run build and verify
                if not self._run_build_verify():
                    print("Build verification failed after Claude's fix.")
                    print("Attempting to recover by resetting git...")

                    # Try to recover by resetting
                    subprocess.run(['git', 'reset', '--hard', 'HEAD'], check=False)

                    if not self._run_build_verify():
                        print("Cannot recover to a good state. Stopping.")
                        break

                    print("Successfully recovered. Adding to ignored list and continuing.")
                    self._add_to_ignored(selected_key)
                else:
                    # Build passed! Check if there are uncommitted changes
                    result = subprocess.run(
                        ['git', 'diff', '--quiet'],
                        capture_output=True
                    )

                    if result.returncode != 0:
                        # There are uncommitted changes, commit them
                        print("Claude fixed the issue but didn't commit. Committing changes...")
                        try:
                            subprocess.run(['git', 'add', '-A'], check=True)
                            subprocess.run(
                                ['git', 'commit', '-m', f'task-runner: fix {selected_key}'],
                                check=True
                            )
                            print("Changes committed successfully.")
                        except subprocess.CalledProcessError as e:
                            print(f"Warning: Could not commit changes: {e}")
                    else:
                        print("Claude fixed the issue and committed changes.")

            # Handle consecutive failures
            if exit_code != 0:
                self.consecutive_failures += 1
                if self.consecutive_failures >= 3:
                    print(">= 3 consecutive failures detected. Sleeping for 5 minutes...")
                    import time
                    time.sleep(300)
            else:
                self.consecutive_failures = 0

            self.count += 1


def discover_tasks():
    """Discover all task files and create a mapping from task name to file path."""
    tasks_dir = Path("tools/task-runner/tasks")
    task_map = {}

    if not tasks_dir.exists():
        return task_map

    for task_file in tasks_dir.glob("*.json"):
        try:
            with open(task_file, 'r') as f:
                task_data = json.load(f)
                task_name = task_data.get('task_name')
                if task_name:
                    task_map[task_name] = str(task_file)
        except (json.JSONDecodeError, KeyError, IOError) as e:
            print(f"Warning: Could not load task from {task_file}: {e}", file=sys.stderr)

    return task_map


def list_available_tasks():
    """List all available tasks."""
    task_map = discover_tasks()

    if not task_map:
        print("No tasks found in tools/task-runner/tasks/")
        return

    print("Available tasks:")
    for task_name, task_file in sorted(task_map.items()):
        print(f"  - {task_name} ({task_file})")


def main():
    parser = argparse.ArgumentParser(
        description='Task runner harness for Claude',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example taskfile.json (with inline prompt):
{
  "task_name": "decompile",
  "script": "python3 tools/list_functions.py",
  "prompt": "decompile the function $ARGUMENT",
  "claude_command": "claude",  (optional, defaults to "claude")
  "accept_best_effort": false  (optional, defaults to false)
}

Example taskfile.json (with template file):
{
  "task_name": "cleanup",
  "script": "python3 tools/list_functions.py",
  "template": "tools/task-runner/tasks/cleanup-template.md",
  "claude_command": "claude",  (optional, defaults to "claude")
  "accept_best_effort": false  (optional, defaults to false)
}

The script should output a JSON array of strings or arrays, e.g.:
["func_80001234", "func_80005678"]
or with additional context per item:
[["func_80001234", "similar_func"], ["func_80005678", "other_func"]]

Task files must have either "prompt" (inline string) or "template" (file path),
but not both. Variables in the prompt or template will be replaced:
  - $ARGUMENT: The selected argument (or first element if array)
  - $ARGUMENT_1, $ARGUMENT_2, ...: Individual elements when using array format
  - $REMAINING_ARGUMENTS: Comma-separated list of elements 2+ (for array format)

When accept_best_effort is true, changes are committed even if the
argument still appears in the script output (best effort fix).

Parallel execution:
Use --evens and --odds to run two sessions in parallel without conflicts.
Each session will only process candidates whose function name hashes to
an even or odd number respectively.

Tasks are discovered from tools/task-runner/tasks/*.json
Use --list to see all available tasks.
        """
    )

    parser.add_argument('task', nargs='?', help='Task name to run (use --list to see available tasks)')
    parser.add_argument('--list', action='store_true',
                       help='List all available tasks')
    parser.add_argument('--limit', type=int, metavar='X', dest='times',
                       help='Maximum number of iterations')
    parser.add_argument('--times', type=int, metavar='X', dest='times',
                       help='(deprecated, use --limit) Maximum number of iterations')
    parser.add_argument('--dry-run', action='store_true',
                       help='Print the prompt that would be passed to Claude without executing')
    parser.add_argument('--verbose', action='store_true',
                       help='Print verbose output including prompt template')

    # Mutually exclusive group for parallel execution
    parallel_group = parser.add_mutually_exclusive_group()
    parallel_group.add_argument('--evens', action='store_true',
                       help='Only process candidates with even hash (for parallel execution)')
    parallel_group.add_argument('--odds', action='store_true',
                       help='Only process candidates with odd hash (for parallel execution)')

    args = parser.parse_args()

    # Handle --list
    if args.list:
        list_available_tasks()
        sys.exit(0)

    # Require task name
    if not args.task:
        parser.print_help()
        print("\nError: task name is required (use --list to see available tasks)")
        sys.exit(1)

    # Discover tasks
    task_map = discover_tasks()

    # Look up task name
    if args.task not in task_map:
        print(f"Error: Task '{args.task}' not found")
        print()
        list_available_tasks()
        sys.exit(1)

    taskfile_path = task_map[args.task]

    # Validate --limit/--times argument
    if args.times is not None and args.times < 1:
        print("Error: --limit/--times argument must be a positive integer")
        sys.exit(1)

    # Determine hash filter
    hash_filter = None
    if args.evens:
        hash_filter = 'evens'
    elif args.odds:
        hash_filter = 'odds'

    runner = TaskRunner(taskfile_path, args.times, args.dry_run, args.verbose, hash_filter)
    runner.run()


if __name__ == '__main__':
    main()
