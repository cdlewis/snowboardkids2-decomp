#!/usr/bin/env python3
"""
Fetch all scratches from decomp.me API for a user and save as a combined JSON array.
Supports resuming from a specific cursor on error.
Supports --update mode to fetch only new or changed scratches.
"""

import argparse
import json
import os
import sys
import time
import urllib.error
import urllib.parse
import urllib.request

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BASE_URL = "https://decomp.me/api/users/cdlewis/scratches"
OUTPUT_FILE = os.path.join(SCRIPT_DIR, "scratches.json")
SECRET_FILE = ".decomp_dev_secret"


def get_user_agent() -> str | None:
    """Read user agent from .decomp_dev_secret file."""
    if os.path.exists(SECRET_FILE):
        with open(SECRET_FILE, "r") as f:
            return f.readline().strip()
    return None


def fetch_page(cursor: str | None = None, user_agent: str | None = None) -> dict:
    """Fetch a single page of scratches from the API."""
    url = BASE_URL
    if cursor:
        url = f"{BASE_URL}?cursor={urllib.parse.quote(cursor)}"

    print(f"Fetching: {url}")

    headers = {"Accept": "application/json"}
    if user_agent:
        headers["User-Agent"] = user_agent

    req = urllib.request.Request(url, headers=headers)

    with urllib.request.urlopen(req, timeout=30) as response:
        return json.loads(response.read().decode("utf-8"))


def load_existing_scratches() -> tuple[list, dict]:
    """Load existing scratches from OUTPUT_FILE.
    Returns (scratches_list, slug_to_scratch_dict).
    """
    if not os.path.exists(OUTPUT_FILE):
        return [], {}

    with open(OUTPUT_FILE, "r") as f:
        scratches = json.load(f)

    # Build dict indexed by slug for efficient lookup
    scratches_by_slug = {}
    for scratch in scratches:
        slug = scratch.get("slug") or scratch.get("id")
        if slug:
            scratches_by_slug[slug] = scratch

    return scratches, scratches_by_slug


def main():
    parser = argparse.ArgumentParser(description="Fetch scratches from decomp.me")
    parser.add_argument(
        "--cursor", "-c",
        help="Resume from a specific cursor (printed on error exit)"
    )
    parser.add_argument(
        "--update", "-u",
        action="store_true",
        help="Only fetch new or changed scratches (updates existing scratches.json)"
    )
    args = parser.parse_args()

    if args.update and args.cursor:
        print("Error: --update and --cursor are mutually exclusive", file=sys.stderr)
        sys.exit(1)

    # In update mode, load existing scratches
    existing_scratches = []
    existing_by_slug = {}
    if args.update:
        existing_scratches, existing_by_slug = load_existing_scratches()
        if existing_scratches:
            print(f"Loaded {len(existing_scratches)} existing scratches from {OUTPUT_FILE}")
        else:
            print(f"No existing {OUTPUT_FILE} found, fetching all scratches")

    all_results = []
    new_or_updated = []
    cursor = args.cursor
    page_num = 1
    user_agent = get_user_agent()

    if user_agent:
        print(f"Using User-Agent from {SECRET_FILE}")

    try:
        should_stop = False
        while True:
            data = fetch_page(cursor, user_agent)
            results = data.get("results", [])
            all_results.extend(results)
            print(f"Page {page_num}: fetched {len(results)} scratches (total: {len(all_results)})")

            # In update mode, check for new/changed scratches
            if args.update:
                page_has_unchanged = True
                for scratch in results:
                    slug = scratch.get("slug") or scratch.get("id")
                    if not slug:
                        continue

                    score = scratch.get("score")
                    last_updated = scratch.get("last_updated")

                    if slug in existing_by_slug:
                        local_scratch = existing_by_slug[slug]
                        local_score = local_scratch.get("score")
                        local_last_updated = local_scratch.get("last_updated")

                        # Check if ANY field changed (score or last_updated)
                        if score != local_score or last_updated != local_last_updated:
                            new_or_updated.append(scratch)
                            page_has_unchanged = False
                            print(f"  Changed: {slug} (score: {local_score} -> {score}, last_updated changed)")
                        else:
                            print(f"  Unchanged: {slug} (score={score}, last_updated={last_updated})")
                    else:
                        # New scratch
                        new_or_updated.append(scratch)
                        page_has_unchanged = False
                        print(f"  New: {slug}")

                # Stop if all scratches on this page were unchanged
                if page_has_unchanged:
                    print("All scratches on this page were unchanged, stopping.")
                    should_stop = True
                    break

            next_url = data.get("next")
            if not next_url:
                print("No more pages.")
                break

            # Parse cursor from next URL
            parsed = urllib.parse.urlparse(next_url)
            params = urllib.parse.parse_qs(parsed.query)
            cursor = params.get("cursor", [None])[0]

            if not cursor:
                print("No cursor in next URL, stopping.")
                break

            page_num += 1
            time.sleep(1)  # Rate limiting

    except (urllib.error.URLError, urllib.error.HTTPError, json.JSONDecodeError) as e:
        print(f"\nError: {e}", file=sys.stderr)
        if cursor:
            print(f"\nTo resume, run:", file=sys.stderr)
            print(f"  python3 {sys.argv[0]} --cursor '{cursor}'", file=sys.stderr)
        sys.exit(1)

    # Save combined results
    if args.update and new_or_updated:
        # Merge new/updated with existing unchanged scratches
        new_or_updated_slugs = {s.get("slug") or s.get("id") for s in new_or_updated}
        unchanged = [s for s in existing_scratches if (s.get("slug") or s.get("id")) not in new_or_updated_slugs]
        merged = new_or_updated + unchanged

        with open(OUTPUT_FILE, "w") as f:
            json.dump(merged, f, indent=2)

        print(f"\nSaved {len(merged)} scratches to {OUTPUT_FILE} ({len(new_or_updated)} new/updated)")
    elif args.update:
        print(f"\nNo new or updated scratches found. {OUTPUT_FILE} unchanged.")
    else:
        # Original behavior
        with open(OUTPUT_FILE, "w") as f:
            json.dump(all_results, f, indent=2)

        print(f"\nSaved {len(all_results)} scratches to {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
