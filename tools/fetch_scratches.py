#!/usr/bin/env python3
"""
Fetch all scratches from decomp.me API for a user and save as a combined JSON array.
Supports resuming from a specific cursor on error.
"""

import argparse
import json
import os
import sys
import time
import urllib.error
import urllib.parse
import urllib.request

BASE_URL = "https://decomp.me/api/users/cdlewis/scratches"
OUTPUT_FILE = "scratches.json"
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


def main():
    parser = argparse.ArgumentParser(description="Fetch scratches from decomp.me")
    parser.add_argument(
        "--cursor", "-c",
        help="Resume from a specific cursor (printed on error exit)"
    )
    args = parser.parse_args()

    all_results = []
    cursor = args.cursor
    page_num = 1
    user_agent = get_user_agent()

    if user_agent:
        print(f"Using User-Agent from {SECRET_FILE}")

    try:
        while True:
            data = fetch_page(cursor, user_agent)
            results = data.get("results", [])
            all_results.extend(results)
            print(f"Page {page_num}: fetched {len(results)} scratches (total: {len(all_results)})")

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
    with open(OUTPUT_FILE, "w") as f:
        json.dump(all_results, f, indent=2)

    print(f"\nSaved {len(all_results)} scratches to {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
