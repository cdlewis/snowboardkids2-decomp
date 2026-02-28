#!/usr/bin/env python3
"""
Migrate src/ files into organized subdirectories.
Updates YAML, Makefile, INCLUDE_ASM paths, #include paths, and moves all files.
"""

import os
import re
import subprocess
import sys

# Root of the project
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Mapping: old_yaml_name -> new_yaml_name
# For unnamed segments, old_yaml_name is the hex address string (e.g., "15080")
# For named segments, old_yaml_name is the name (e.g., "boot")
RENAMES = {
    # === src/core/ ===
    "boot": "core/boot",
    "mainEntrypoint": "core/mainEntrypoint",
    "main": "core/main",
    "game_state_init": "core/game_state_init",
    "15080": "core/mode_init",
    "15690": "core/session_manager",
    "buffers": "core/buffers",
    "90D0": "core/stubs",

    # === src/system/ ===
    "thread_manager": "system/thread_manager",
    "task_scheduler": "system/task_scheduler",
    "memory_allocator": "system/memory_allocator",
    "rom_loader": "system/rom_loader",
    "controller_io": "system/controller_io",
    "33E00": "system/motor_rumble",

    # === src/audio/ ===
    "audio_manager": "audio/audio_manager",
    "audio": "audio/audio",

    # === src/graphics/ ===
    "graphics": "graphics/graphics",
    "displaylist": "graphics/displaylist",
    "10AD0": "graphics/sprite_rdp",
    "5520": "graphics/tiled_texture",
    "6DD0": "graphics/quad_displaylist",
    "6DE50": "graphics/clip_text_render",
    "CBA0": "graphics/model_loader",
    "90F0": "graphics/sprite_table",
    "C870": "graphics/palette",
    "36BE0": "graphics/camera_transform",
    "31870": "graphics/preview_render",
    "39020": "graphics/tiled_sprite_grid",

    # === src/text/ ===
    "33FE0": "text/hud_text",
    "36B80": "text/font_render",
    "3CD70": "text/text_elements",
    "3C960": "text/numeric_text",
    "68CF0": "text/font_assets",

    # === src/math/ ===
    "geometry": "math/geometry",
    "rand": "math/rand",

    # === src/animation/ ===
    "5EA60": "animation/bone_animation",
    "76B0": "animation/stepped_matrix",
    "7F80": "animation/model_transition",
    "8700": "animation/model_scale",
    "8A40": "animation/bobbing_model",
    "8DC0": "animation/oscillating_sprite_model",
    "69B0": "animation/oscillating_sprite",
    "7850": "animation/pulsing_indicator",
    "B040": "animation/easing_state",
    "1E2BE0": "animation/animation_loop",
    "1E36C0": "animation/slot_animation",

    # === src/effects/ ===
    "3B80": "effects/transparent_render",
    "7C70": "effects/orbital_sprite",
    "DA20": "effects/static_sprite_effect",
    "D6F0": "effects/animated_sprite_effect",
    "C570": "effects/rocket_boost",
    "fan_effect": "effects/fan_effect",
    "nonrace_shadow": "effects/nonrace_shadow",
    "1E2070": "effects/screen_transition",
    "1E25B0": "effects/trick_burst",
    "1E60D0": "effects/scrolling_texture",
    "1E64A0": "effects/zoom_effect",
    "1E2890": "effects/fan_sound_3d",
    "4050": "effects/cutscene_keyframes",

    # === src/race/ ===
    "race_session": "race/race_session",
    "player": "race/player",
    "9FF70": "race/race_main",
    "A9A40": "race/ai_pathfinding",
    "5AA90": "race/track_collision",
    "5DBC0": "race/track_height",
    "2D4C0": "race/track_geometry",
    "46080": "race/race_hud",
    "4CD70": "race/race_effects",
    "594E0": "race/hit_reactions",
    "59290": "race/character_stats",
    "42170": "race/particle_items",
    "413E0": "race/position_markers",
    "51060": "race/spray_particles",
    "52880": "race/obstacle_sprites",

    # === src/story/ ===
    "17F50": "story/map_character_anim",
    "19400": "story/map_camera",
    "198B0": "story/location_discovery",
    "199C0": "story/location_init",
    "19AB0": "story/map_state",
    "1A770": "story/race_state_machine",
    "1BBA0": "story/map_init",
    "1D410": "story/unlock_intro",
    "1F070": "story/map_extra_intro",
    "1F190": "story/story_intro",
    "288A0": "story/map_events",
    "29200": "story/npc_models",
    "297B0": "story/npc_dialogue",
    "2AF90": "story/rare_events",
    "2C8F0": "story/rare_event_npcs",
    "2F990": "story/shop_ui",
    "36E50": "story/location_markers",

    # === src/ui/ ===
    "title_screen": "ui/title_screen",
    "options_menu": "ui/options_menu",
    "16FA0": "ui/title_ui_elements",
    "E090": "ui/logo_splash",
    "E770": "ui/gallery",
    "DC90": "ui/cutscene_wait_menu",
    "1C9C0": "ui/unlock_screen",
    "1D520": "ui/save_slot_select",
    "37E70": "ui/options_camera",
    "38BE0": "ui/unlock_system",
    "38C90": "ui/save_data",
    "22D30": "ui/player_select",
    "227D0": "ui/level_select_state",
    "22920": "ui/versus_mode_init",
    "202A0": "ui/level_preview",
    "20F0": "ui/level_preview_3d",
    "232F0": "ui/character_select_ui",
    "24A30": "ui/character_select_gfx",
    "1DB7A0": "ui/player_select_sprites",
    "1DA660": "ui/character_select_sprites",

    # === src/credits/ ===
    "credits": "credits/credits",
    "1DC260": "credits/credits_decorations",
    "1DC480": "credits/credits_subtitles",
    "1DC900": "credits/credits_text",
    "1DCF60": "credits/offset_table",

    # === src/triggers/ ===
    "town_collision": "triggers/town_collision",
    "clocktower_trigger": "triggers/clocktower_trigger",
    "trick_trigger": "triggers/trick_trigger",
    "phone_trigger": "triggers/phone_trigger",
    "wall_newspaper_trigger": "triggers/wall_newspaper_trigger",
    "unused_triggers": "triggers/unused_triggers",

    # === src/data/ ===
    "56910": "data/course_data",
    "5E590": "data/asset_metadata",
    "19E80": "data/data_table",
}

# Header files in src/ that need to move alongside their .c files
# Map from old header name (without .h) to new path (without .h)
HEADER_MOVES = {
    "15690": "core/session_manager",
    "race_session": "race/race_session",
    "rand": "math/rand",
    "42170": "race/particle_items",
    "202A0": "ui/level_preview",
    "1DC260": "credits/credits_decorations",
    "1DC480": "credits/credits_subtitles",
}


def git_mv(old_path, new_path):
    """Run git mv, creating parent directories as needed."""
    new_dir = os.path.dirname(new_path)
    if new_dir and not os.path.exists(new_dir):
        os.makedirs(new_dir, exist_ok=True)
    if os.path.exists(old_path):
        subprocess.run(["git", "mv", old_path, new_path], cwd=ROOT, check=True)
        return True
    return False


def move_src_files():
    """Move C and H files in src/."""
    print("=== Moving src/ files ===")
    for old_name, new_name in RENAMES.items():
        # Determine old filename
        # Check if old file exists with the name directly
        old_c = os.path.join(ROOT, "src", f"{old_name}.c")
        new_c = os.path.join(ROOT, "src", f"{new_name}.c")
        if os.path.exists(old_c):
            print(f"  src/{old_name}.c -> src/{new_name}.c")
            git_mv(old_c, new_c)
        else:
            print(f"  WARNING: src/{old_name}.c not found, skipping")

    # Move header files
    for old_name, new_name in HEADER_MOVES.items():
        old_h = os.path.join(ROOT, "src", f"{old_name}.h")
        new_h = os.path.join(ROOT, "src", f"{new_name}.h")
        if os.path.exists(old_h):
            print(f"  src/{old_name}.h -> src/{new_name}.h")
            git_mv(old_h, new_h)


def move_asm_data_files():
    """Move asm/data/ files to match new segment names."""
    print("\n=== Moving asm/data/ files ===")
    data_dir = os.path.join(ROOT, "asm", "data")
    for old_name, new_name in RENAMES.items():
        for suffix in [".data.s", ".rodata.s", ".bss.s"]:
            old_f = os.path.join(data_dir, f"{old_name}{suffix}")
            new_f = os.path.join(data_dir, f"{new_name}{suffix}")
            if os.path.exists(old_f):
                print(f"  asm/data/{old_name}{suffix} -> asm/data/{new_name}{suffix}")
                git_mv(old_f, new_f)


def move_asm_nonmatchings():
    """Move asm/nonmatchings/ directories."""
    print("\n=== Moving asm/nonmatchings/ directories ===")
    nm_dir = os.path.join(ROOT, "asm", "nonmatchings")
    for old_name, new_name in RENAMES.items():
        old_d = os.path.join(nm_dir, old_name)
        new_d = os.path.join(nm_dir, new_name)
        if os.path.isdir(old_d):
            print(f"  asm/nonmatchings/{old_name}/ -> asm/nonmatchings/{new_name}/")
            git_mv(old_d, new_d)


def move_asm_matchings():
    """Move asm/matchings/ directories."""
    print("\n=== Moving asm/matchings/ directories ===")
    m_dir = os.path.join(ROOT, "asm", "matchings")
    for old_name, new_name in RENAMES.items():
        old_d = os.path.join(m_dir, old_name)
        new_d = os.path.join(m_dir, new_name)
        if os.path.isdir(old_d):
            print(f"  asm/matchings/{old_name}/ -> asm/matchings/{new_name}/")
            git_mv(old_d, new_d)


def update_yaml():
    """Update segment names in snowboardkids2.yaml."""
    print("\n=== Updating snowboardkids2.yaml ===")
    yaml_path = os.path.join(ROOT, "snowboardkids2.yaml")
    with open(yaml_path, "r") as f:
        content = f.read()

    # We need to replace segment name references in YAML
    # Types of references:
    # 1. [0xADDR, c, NAME] -> [0xADDR, c, NEW_NAME]
    # 2. [0xADDR, c] (unnamed, name defaults to address) -> [0xADDR, c, NEW_NAME]
    # 3. [0xADDR, .data, NAME] -> [0xADDR, .data, NEW_NAME]
    # 4. [0xADDR, data, NAME] -> [0xADDR, data, NEW_NAME]
    # 5. [0xADDR, .rodata, NAME] -> [0xADDR, .rodata, NEW_NAME]
    # 6. [0xADDR, hasm, NAME] -> skip (don't touch hasm)
    # 7. { ... name: NAME } for some formats

    lines = content.split("\n")
    new_lines = []

    for line in lines:
        new_line = line

        # Handle named segments: [0xADDR, c, NAME] or [0xADDR, .data, NAME] etc.
        # Pattern: captures the bracket content with a name
        for old_name, new_name in RENAMES.items():
            # Match patterns like: [0xADDR, c, OLD_NAME]
            # The name can be at different positions
            # Be careful to match whole names only

            # For code segments with explicit name
            pat = rf'(\[0x[0-9A-Fa-f]+,\s*c,\s*){re.escape(old_name)}(\s*\])'
            new_line = re.sub(pat, rf'\g<1>{new_name}\2', new_line)

            # For .data segments
            pat = rf'(\[0x[0-9A-Fa-f]+,\s*\.data,\s*){re.escape(old_name)}(\s*\])'
            new_line = re.sub(pat, rf'\g<1>{new_name}\2', new_line)

            # For data segments (assembly blob)
            pat = rf'(\[0x[0-9A-Fa-f]+,\s*data,\s*){re.escape(old_name)}(\s*\])'
            new_line = re.sub(pat, rf'\g<1>{new_name}\2', new_line)

            # For .rodata segments
            pat = rf'(\[0x[0-9A-Fa-f]+,\s*\.rodata,\s*){re.escape(old_name)}(\s*\])'
            new_line = re.sub(pat, rf'\g<1>{new_name}\2', new_line)

        new_lines.append(new_line)

    content = "\n".join(new_lines)

    # Now handle unnamed code segments: [0xADDR, c] -> [0xADDR, c, NEW_NAME]
    # These are segments where the name defaults to the hex address
    # We need to match [0xADDR, c] where ADDR matches an old_name that's a hex address
    for old_name, new_name in RENAMES.items():
        # Only process hex-address names (unnamed segments)
        if not re.match(r'^[0-9A-Fa-f]+$', old_name):
            continue

        # Match [0xADDR, c] with optional whitespace, but NO third argument
        # The address in YAML might have different case than our key
        addr_upper = old_name.upper()
        # Pattern: [0xADDR, c] at end or followed by nothing else
        pat = rf'(\[0x{addr_upper},\s*c)\s*\]'
        replacement = rf'\g<1>, {new_name}]'
        content = re.sub(pat, replacement, content)

    with open(yaml_path, "w") as f:
        f.write(content)

    print("  Updated YAML segment references")


def update_include_asm():
    """Update INCLUDE_ASM paths in C files."""
    print("\n=== Updating INCLUDE_ASM paths ===")
    count = 0
    for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, "src")):
        for fname in filenames:
            if not fname.endswith(".c"):
                continue
            fpath = os.path.join(dirpath, fname)
            with open(fpath, "r") as f:
                content = f.read()

            new_content = content
            for old_name, new_name in RENAMES.items():
                # INCLUDE_ASM("asm/nonmatchings/OLD_NAME", func)
                old_path = f"asm/nonmatchings/{old_name}"
                new_path = f"asm/nonmatchings/{new_name}"
                if old_path in new_content:
                    new_content = new_content.replace(
                        f'"{old_path}"', f'"{new_path}"'
                    )

            if new_content != content:
                with open(fpath, "w") as f:
                    f.write(new_content)
                count += 1
                rel = os.path.relpath(fpath, ROOT)
                print(f"  Updated INCLUDE_ASM in {rel}")

    print(f"  Updated {count} files")


def update_includes():
    """Update #include paths for moved header files."""
    print("\n=== Updating #include paths ===")

    # Build a map of header old basename -> new subdir-qualified path
    # e.g., "race_session.h" -> "race/race_session.h"
    header_remap = {}
    for old_name, new_name in HEADER_MOVES.items():
        old_header = f"{old_name}.h"
        new_header = f"{new_name}.h"
        header_remap[old_header] = new_header

    count = 0
    for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, "src")):
        for fname in filenames:
            if not (fname.endswith(".c") or fname.endswith(".h")):
                continue
            fpath = os.path.join(dirpath, fname)
            with open(fpath, "r") as f:
                content = f.read()

            new_content = content
            # Get the subdirectory this file is in (relative to src/)
            rel_dir = os.path.relpath(dirpath, os.path.join(ROOT, "src"))
            if rel_dir == ".":
                rel_dir = ""

            for old_header, new_header in header_remap.items():
                if f'#include "{old_header}"' not in new_content:
                    continue

                # Determine if the including file is in the same directory as the header
                new_header_dir = os.path.dirname(new_header)
                if rel_dir == new_header_dir:
                    # Same directory - use just the filename
                    new_include = os.path.basename(new_header)
                else:
                    # Different directory - use subdir-qualified path
                    new_include = new_header

                new_content = new_content.replace(
                    f'#include "{old_header}"',
                    f'#include "{new_include}"'
                )

            if new_content != content:
                with open(fpath, "w") as f:
                    f.write(new_content)
                count += 1
                rel = os.path.relpath(fpath, ROOT)
                print(f"  Updated #includes in {rel}")

    print(f"  Updated {count} files")


def update_makefile():
    """Update Makefile SRC_DIRS and custom rules."""
    print("\n=== Updating Makefile ===")
    mf_path = os.path.join(ROOT, "Makefile")
    with open(mf_path, "r") as f:
        content = f.read()

    # Update SRC_DIRS
    new_dirs = [
        "src", "src/core", "src/system", "src/audio", "src/graphics",
        "src/text", "src/math", "src/animation", "src/effects",
        "src/race", "src/story", "src/ui", "src/credits",
        "src/triggers", "src/data", "src/cutscene", "src/levels",
    ]
    old_src_dirs = "SRC_DIRS  = src src/cutscene"
    new_src_dirs = "SRC_DIRS  = " + " ".join(new_dirs)
    content = content.replace(old_src_dirs, new_src_dirs)

    # Update custom OPT_FLAGS for 39020.c -> graphics/tiled_sprite_grid.c
    content = content.replace(
        "$(BUILD_DIR)/src/39020.o: OPT_FLAGS := -O0",
        "$(BUILD_DIR)/src/graphics/tiled_sprite_grid.o: OPT_FLAGS := -O0"
    )

    with open(mf_path, "w") as f:
        f.write(content)

    print("  Updated SRC_DIRS and custom rules")


def move_misc_files():
    """Move miscellaneous files that aren't .c or .h but are alongside renamed files."""
    print("\n=== Moving misc src/ files ===")
    # src/33E00 directory (not a .c file)
    old_dir = os.path.join(ROOT, "src", "33E00")
    if os.path.isdir(old_dir):
        new_dir = os.path.join(ROOT, "src", "system", "motor_rumble")
        print(f"  src/33E00/ -> src/system/motor_rumble/")
        git_mv(old_dir, new_dir)


def main():
    os.chdir(ROOT)

    # Create all target directories first
    print("=== Creating directories ===")
    dirs_to_create = [
        "src/core", "src/system", "src/audio", "src/graphics",
        "src/text", "src/math", "src/animation", "src/effects",
        "src/race", "src/story", "src/ui", "src/credits",
        "src/triggers", "src/data",
        # asm/data subdirs
        "asm/data/core", "asm/data/system", "asm/data/audio",
        "asm/data/graphics", "asm/data/text", "asm/data/math",
        "asm/data/animation", "asm/data/effects", "asm/data/race",
        "asm/data/story", "asm/data/ui", "asm/data/credits",
        "asm/data/triggers", "asm/data/data",
        # asm/nonmatchings subdirs
        "asm/nonmatchings/core", "asm/nonmatchings/system",
        "asm/nonmatchings/audio", "asm/nonmatchings/graphics",
        "asm/nonmatchings/text", "asm/nonmatchings/math",
        "asm/nonmatchings/animation", "asm/nonmatchings/effects",
        "asm/nonmatchings/race", "asm/nonmatchings/story",
        "asm/nonmatchings/ui", "asm/nonmatchings/credits",
        "asm/nonmatchings/triggers", "asm/nonmatchings/data",
        # asm/matchings subdirs
        "asm/matchings/core", "asm/matchings/system",
        "asm/matchings/audio", "asm/matchings/graphics",
        "asm/matchings/text", "asm/matchings/math",
        "asm/matchings/animation", "asm/matchings/effects",
        "asm/matchings/race", "asm/matchings/story",
        "asm/matchings/ui", "asm/matchings/credits",
        "asm/matchings/triggers", "asm/matchings/data",
    ]
    for d in dirs_to_create:
        full = os.path.join(ROOT, d)
        os.makedirs(full, exist_ok=True)

    # Step 1: Move files
    move_src_files()
    move_misc_files()
    move_asm_data_files()
    move_asm_nonmatchings()
    move_asm_matchings()

    # Step 2: Update YAML
    update_yaml()

    # Step 3: Update INCLUDE_ASM paths in C files
    update_include_asm()

    # Step 4: Update #include paths for moved headers
    update_includes()

    # Step 5: Update Makefile
    update_makefile()

    print("\n=== Migration complete ===")
    print("Next steps:")
    print("  1. Run: python3 -m splat split snowboardkids2.yaml")
    print("  2. Run: ./tools/build-and-verify.sh")
    print("  3. Fix any remaining issues")


if __name__ == "__main__":
    main()
