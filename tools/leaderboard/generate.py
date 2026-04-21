#!/usr/bin/env python3
"""
Generate an HTML leaderboard of decomp.me scratch progress for Snowboard Kids 2.

Reads scratches.json and outputs a styled HTML page based on template.html.
"""

import html
import json
import re
import sys
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Tuple


DECOMP_ME_BASE = "https://decomp.me/scratch"


def load_scratches(scratches_path: Path) -> List[Dict]:
    """Load scratches.json and return the full list of scratch dicts."""
    if not scratches_path.exists():
        return []
    try:
        with open(scratches_path, "r") as f:
            return json.load(f)
    except (IOError, json.JSONDecodeError):
        return []


def get_unmatched_set(repo_root: Path) -> set:
    """Return set of function names (stems) that still exist in asm/nonmatchings/."""
    asm_dir = repo_root / "asm" / "nonmatchings"
    if not asm_dir.exists():
        return set()
    return {f.stem for f in asm_dir.rglob("*.s")}


def build_best_scratches(scratches: List[Dict], unmatched: set) -> Dict[str, Dict]:
    """Build a dict of base_func_name -> {slug, percentage, name} keeping best per function.

    Only includes functions whose base name still exists in asm/nonmatchings/.
    """
    result: Dict[str, Dict] = {}
    for scratch in scratches:
        name = scratch.get("name", "")
        slug = scratch.get("slug", "")
        score = scratch.get("score")
        max_score = scratch.get("max_score")
        if not name or not slug or score is None or not max_score or max_score <= 0:
            continue
        base = re.sub(r"-\d+$", "", name)
        if base not in unmatched:
            continue
        pct = ((max_score - score) / max_score) * 100
        if base not in result or pct > result[base]["pct"]:
            result[base] = {"slug": slug, "pct": pct, "name": name}
    return result


def render_rows(best: Dict[str, Dict]) -> str:
    """Render one board-row per function, sorted by pct descending."""
    sorted_funcs = sorted(best.values(), key=lambda x: x["pct"], reverse=True)
    rows = []
    for entry in sorted_funcs:
        escaped_name = html.escape(entry["name"])
        pct = entry["pct"]
        slug = entry["slug"]

        bar_html = (
            '<div class="match-bar">'
            + f'<div class="match-fill" style="width:{pct:.1f}%"></div>'
            + "</div>"
            + f'<div class="match-num">{pct:.2f}%</div>'
        )
        link = (
            f'<a class="scratch-btn" href="{DECOMP_ME_BASE}/{slug}"'
            + ' target="_blank" rel="noopener">decomp.me ▸</a>'
        )
        rows.append(
            '        <div class="board-row">\n'
            + f'          <div><div class="fn-name">{escaped_name}</div></div>\n'
            + f"          <div>{bar_html}</div>\n"
            + f"          <div>{link}</div>\n"
            + "        </div>"
        )
    return "\n".join(rows)


def generate_html(best: Dict[str, Dict]) -> str:
    """Build the full HTML page from template.html."""
    template_path = Path(__file__).parent / "template.html"
    with open(template_path, "r") as f:
        template = f.read()

    # Splice into the template at <div class="board"> ... </main>
    board_tag = '<div class="board">'
    board_start = template.index(board_tag) + len(board_tag)
    board_end = template.rindex("</main>")

    header_part = template[:board_start]
    footer_part = template[board_end:]

    # Replace title "REMAINING FUNCTIONS" with "N FUNCTIONS LEFT"
    # Each semantic word gets its own .word span; spacing comes from .word margin
    count = len(best)
    num_text = str(count)
    func_text = "FUNCTION" if count == 1 else "FUNCTIONS"
    num_spans = "".join(f"<span>{c}</span>" for c in num_text)
    func_spans = "".join(f"<span>{c}</span>" for c in func_text)
    left_spans = "".join(f"<span>{c}</span>" for c in "LEFT")
    new_line2 = (
        '<span class="line2">'
        f'<span class="word w1">{num_spans}</span>'
        f'<span class="word w2">{func_spans}</span>'
        f'<span class="word w3">{left_spans}</span>'
        "</span>"
    )
    header_part = re.sub(
        r'<span class="line2">.*?</span>\s*</h1>',
        new_line2 + "\n      </h1>",
        header_part,
        flags=re.DOTALL,
    )

    # Inject og:image meta tag
    og_tag = '    <meta property="og:image" content="og.png" />\n'
    header_part = header_part.replace(
        "</title>", "</title>\n" + og_tag, 1
    )

    header_row = (
        '<div class="board-row head">\n'
        "          <div>Function</div>\n"
        "          <div>Match</div>\n"
        '          <div style="text-align:center">Scratch</div>\n'
        "        </div>"
    )

    now = datetime.now().strftime("%Y-%m-%d %H:%M")
    timestamp = (
        '<div style="text-align:center;padding:12px 24px;font-size:11px;'
        'color:#999;font-family:JetBrains Mono,monospace">'
        + f"Generated {now}"
        + "</div>"
    )

    body = "\n".join([header_row, render_rows(best), timestamp])
    return header_part + "\n" + body + "\n      </div>\n    " + footer_part


def screenshot_og_image(html_path: Path, out_path: Path) -> None:
    """Take a screenshot of the page for use as og:image."""
    from playwright.sync_api import sync_playwright

    with sync_playwright() as p:
        browser = p.chromium.launch()
        page = browser.new_page(viewport={"width": 1200, "height": 630})
        page.goto(html_path.as_uri())
        page.screenshot(path=str(out_path), clip={"x": 0, "y": 0, "width": 1200, "height": 630})
        browser.close()


def main():
    repo_root = Path(__file__).parent.parent.parent
    tools_dir = Path(__file__).parent.parent
    scratches_path = tools_dir / "scratches.json"
    public_dir = Path(__file__).parent / "public"
    public_dir.mkdir(exist_ok=True)

    unmatched = get_unmatched_set(repo_root)
    scratches = load_scratches(scratches_path)
    best = build_best_scratches(scratches, unmatched)

    page = generate_html(best)

    html_path = public_dir / "index.html"
    with open(html_path, "w") as f:
        f.write(page)

    og_path = public_dir / "og.png"
    screenshot_og_image(html_path, og_path)

    print(f"Written {html_path} + {og_path} ({len(best)} functions)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
