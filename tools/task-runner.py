#!/usr/bin/env python3

import argparse
import json
import subprocess
import sys
import os
import signal
from datetime import datetime
from pathlib import Path

class TaskRunner:
    def __init__(self, taskfile_path, max_times=None, dry_run=False):
        self.taskfile_path = taskfile_path
        self.max_times = max_times
        self.dry_run = dry_run
        self.stop_requested = False
        self.count = 0
        self.consecutive_failures = 0

        # Load taskfile
        with open(taskfile_path, 'r') as f:
            self.taskfile = json.load(f)

        self.task_name = self.taskfile['task_name']
        self.script = self.taskfile['script']
        self.prompt_template = self.taskfile['prompt']
        self.claude_command = self.taskfile.get('claude_command', 'claude')

        # Setup log files
        self.ignored_prompts_file = f"tools/ignored_prompts_{self.task_name}.log"
        self.claude_log_file = f"tools/claude_logs_{self.task_name}.log"

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

    def _select_argument(self, candidates):
        """Select the first candidate not in ignored list."""
        ignored = self._load_ignored_prompts()

        for candidate in candidates:
            if candidate not in ignored:
                return candidate

        return None

    def _run_claude(self, prompt):
        """Run Claude with the given prompt and log output."""
        # Escape any quotes in the prompt for shell safety
        escaped_prompt = prompt.replace('"', '\\"')
        command = f'{self.claude_command} -p "{escaped_prompt}"'

        print(f"Running: {command}")

        try:
            result = subprocess.run(
                command,
                shell=True,
                capture_output=True,
                text=True
            )

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

        while True:
            if self.stop_requested:
                print("Stopping gracefully.")
                break

            if self.max_times is not None and self.count >= self.max_times:
                print(f"Reached maximum iterations: {self.max_times}")
                break

            print(f"=== Starting iteration {self.count + 1} at {datetime.now().strftime('%Y-%m-%d %H:%M:%S')} ===")

            # Run script to get candidates
            candidates = self._run_script()

            if not candidates:
                print("No candidates returned from script.")
                break

            print(f"Script returned {len(candidates)} candidate(s)")

            # Select first non-ignored argument
            selected = self._select_argument(candidates)

            if selected is None:
                print("All candidates have been processed (in ignored list).")
                break

            print(f"Selected argument: {selected}")

            # Interpolate prompt
            prompt = self.prompt_template.replace('$ARGUMENT', selected)

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

            if selected in new_candidates:
                # Issue still exists, clean up and add to ignored list
                print(f"Argument '{selected}' still appears in script output.")

                # Reset git to clean up Claude's changes
                subprocess.run(['git', 'reset', '--hard', 'HEAD'], check=False)

                # Verify build still works
                if not self._run_build_verify():
                    print("Build verification failed, stopping.")
                    break

                print(f"Adding '{selected}' to ignored list.")
                self._add_to_ignored(selected)
            else:
                # Issue was fixed! Verify build and commit if successful
                print(f"Argument '{selected}' no longer appears in script output.")

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
                    self._add_to_ignored(selected)
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
                                ['git', 'commit', '-m', f'task-runner: fix {selected}'],
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

        print(f"\nTotal iterations: {self.count}")


def main():
    parser = argparse.ArgumentParser(
        description='Task runner harness for Claude',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example taskfile.json:
{
  "task_name": "decompile",
  "script": "python3 tools/list_functions.py",
  "prompt": "decompile the function $ARGUMENT",
  "claude_command": "claude"  (optional, defaults to "claude")
}

The script should output a JSON array of strings, e.g.:
["func_80001234", "func_80005678"]
        """
    )

    parser.add_argument('taskfile', help='Path to task JSON file')
    parser.add_argument('--times', type=int, metavar='X',
                       help='Maximum number of iterations')
    parser.add_argument('--dry-run', action='store_true',
                       help='Print the prompt that would be passed to Claude without executing')

    args = parser.parse_args()

    # Validate taskfile exists
    if not os.path.exists(args.taskfile):
        print(f"Error: Taskfile not found: {args.taskfile}")
        sys.exit(1)

    # Validate --times argument
    if args.times is not None and args.times < 1:
        print("Error: --times argument must be a positive integer")
        sys.exit(1)

    runner = TaskRunner(args.taskfile, args.times, args.dry_run)
    runner.run()


if __name__ == '__main__':
    main()
