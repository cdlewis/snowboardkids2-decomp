#!/usr/bin/env python3
"""
Wrap f3dex2 macro calls in PERM_IGNORE().
Reads temp.c and outputs temp_clean.c with all gbi macro calls wrapped.
"""

import re
import sys

# All f3dex2 macros from gbi.h
GBI_MACROS = {
    # gSP* macros
    "gSP1Quadrangle", "gSP1Triangle", "gSP2Triangles",
    "gSPBranchLessZ", "gSPBranchLessZraw", "gSPBranchLessZrg", "gSPBranchList",
    "gSPClearGeometryMode", "gSPClipRatio", "gSPCullDisplayList",
    "gSPDisplayList", "gSPDmaRead", "gSPDmaWrite",
    "gSPEndDisplayList", "gSPFogFactor", "gSPFogPosition",
    "gSPForceMatrix", "gSPGeometryMode", "gSPInsertMatrix",
    "gSPLight", "gSPLightColor",
    "gSPLoadGeometryMode", "gSPLoadUcode", "gSPLoadUcodeEx", "gSPLoadUcodeL",
    "gSPLookAt", "gSPLookAtX", "gSPLookAtY",
    "gSPModifyVertex",
    "gSPMatrix",
    "gSPNoOp",
    "gSPNumLights", "gSPPerspNormalize", "gSPPopMatrix", "gSPPopMatrixN",
    "gSPScisTextureRectangle", "gSPSegment",
    "gSPSetGeometryMode", "gSPSetLights0", "gSPSetLights1", "gSPSetLights2",
    "gSPSetLights3", "gSPSetLights4", "gSPSetLights5", "gSPSetLights6",
    "gSPSetLights7", "gSPSetOtherMode",
    "gSPSprite2DDraw", "gSPSprite2DScaleFlip",
    "gSPTexture", "gSPTextureL", "gSPTextureRectangle", "gSPTextureRectangleFlip",
    "gSPViewport",

    # gDP* macros
    "gDPFillRectangle", "gDPFullSync",
    "gDPLoadBlock", "gDPLoadMultiBlock", "gDPLoadMultiBlock_4b",
    "gDPLoadMultiBlock_4bS", "gDPLoadMultiBlockS", "gDPLoadMultiTile",
    "gDPLoadMultiTile_4b",
    "gDPLoadSync",
    "gDPLoadTextureBlock", "_gDPLoadTextureBlock",
    "gDPLoadTextureBlock_4b", "_gDPLoadTextureBlock_4b",
    "gDPLoadTextureBlock_4bS", "gDPLoadTextureBlockS",
    "_gDPLoadTextureBlockTile", "gDPLoadTextureBlockYuv", "gDPLoadTextureBlockYuvS",
    "gDPLoadTextureTile", "gDPLoadTextureTile_4b",
    "gDPLoadTile", "gDPLoadTileGeneric",
    "gDPLoadTLUT", "gDPLoadTLUTCmd", "gDPLoadTLUT_pal16", "gDPLoadTLUT_pal256",
    "gDPNoOp", "gDPNoOpTag",
    "gDPNoParam", "gDPParam",
    "gDPPipelineMode", "gDPPipeSync",
    "gDPScisFillRectangle",
    "gDPSetAlphaCompare", "gDPSetAlphaDither",
    "gDPSetBlendColor", "gDPSetBlendMask",
    "gDPSetColor", "gDPSetColorDither", "gDPSetColorImage",
    "gDPSetCombine", "gDPSetCombineKey", "gDPSetCombineLERP", "gDPSetCombineMode",
    "gDPSetConvert",
    "gDPSetCycleType",
    "gDPSetDepthImage", "gDPSetDepthSource",
    "gDPSetEnvColor",
    "gDPSetFillColor", "gDPSetFogColor",
    "gDPSetKeyGB", "gDPSetKeyR",
    "gDPSetMaskImage",
    "gDPSetOtherMode",
    "gDPSetPrimColor", "gDPSetPrimDepth",
    "gDPSetRenderMode",
    "gDPSetScissor", "gDPSetScissorFrac",
    "gDPSetTextureConvert", "gDPSetTextureDetail", "gDPSetTextureFilter",
    "gDPSetTextureImage", "gDPSetTextureLOD", "gDPSetTextureLUT", "gDPSetTexturePersp",
    "gDPSetTile", "gDPSetTileSize",
    "gDPTextureRectangle", "gDPTextureRectangleFlip",
    "gDPTileSync",
    "gDPWord",

    # Other gbi macros
    "gColor", "gFactor", "gMoveWd", "gPosition", "gSetImage",
}

# Create regex pattern to match any gbi macro call
# Match: macroName( ... );
# Need to handle multi-line calls
GBI_PATTERN = re.compile(r'\b(' + '|'.join(re.escape(m) for m in GBI_MACROS) + r')\s*\(')

# Pattern to detect function definitions (opening brace on same line)
FUNCTION_DEF_PATTERN = re.compile(r'\b(?:static\s+)?(?:\w+\s+)+\*?\s*\w+\s*\([^)]*\)\s*\{')
# Pattern to detect function declarations (brace on next line)
FUNCTION_DECL_PATTERN = re.compile(r'\b(?:static\s+)?(?:\w+\s+)+\*?\s*\w+\s*\([^)]*\)\s*$')


def wrap_gbi_macros(content):
    """
    Wrap all gbi macro calls in PERM_IGNORE() only when inside function bodies.
    Handles multi-line macro calls.
    """
    lines = content.split('\n')
    result = []
    i = 0
    brace_depth = 0
    in_function = False
    expecting_brace = False  # Track when we saw a function decl on previous line
    in_latedefine = False  # Track if we're in the #pragma _permuter latedefine section

    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # Skip the #pragma _permuter latedefine section entirely
        if '#pragma _permuter latedefine start' in stripped:
            in_latedefine = True
            result.append(line)
            i += 1
            continue
        if '#pragma _permuter latedefine end' in stripped:
            in_latedefine = False
            result.append(line)
            i += 1
            continue
        if in_latedefine:
            result.append(line)
            i += 1
            continue

        # Track brace depth to know if we're inside a function
        open_braces = line.count('{')
        close_braces = line.count('}')
        new_brace_depth = brace_depth + open_braces - close_braces

        # Check if this line is a function definition
        # (a line that looks like a function signature and ends with opening brace)
        is_function_def = FUNCTION_DEF_PATTERN.search(line) is not None

        # Check if this line is a function declaration (brace on next line)
        # Must end with ) but not ;, not be a pragma, and not be an extern declaration
        is_function_decl = (FUNCTION_DECL_PATTERN.search(stripped) is not None
                            and not stripped.endswith(';')
                            and not stripped.startswith('#pragma')
                            and not stripped.startswith('extern')
                            and 'typedef' not in stripped)

        # If we saw a function decl on previous line and now we see a brace at depth 0, we're entering a function
        if expecting_brace and open_braces > 0 and brace_depth == 0:
            in_function = True
            expecting_brace = False
        elif is_function_decl and brace_depth == 0:
            expecting_brace = True

        if is_function_def and brace_depth == 0:
            in_function = True

        # Update brace depth after processing the line
        old_brace_depth = brace_depth
        brace_depth = new_brace_depth
        # Only reset in_function when we exit a function (brace depth goes from >0 to 0)
        if old_brace_depth > 0 and brace_depth == 0:
            in_function = False
            expecting_brace = False

        # Check if this line contains a gbi macro call
        match = GBI_PATTERN.search(line)

        if match and not stripped.startswith('//') and in_function:
            macro_name = match.group(1)
            # Find the start of the macro call
            call_start = match.start()

            # Check if this is already wrapped in PERM_IGNORE
            if 'PERM_IGNORE' in line[:call_start]:
                result.append(line)
                i += 1
                continue

            # Extract leading whitespace and content before the macro call
            before_macro = line[:call_start]
            leading_ws = before_macro.rstrip()
            after_call_start = line[call_start:]

            # Build the complete macro call (may span multiple lines)
            macro_call = after_call_start
            paren_count = after_call_start.count('(') - after_call_start.count(')')
            j = i + 1

            # Find the closing semicolon
            while paren_count > 0 or ';' not in macro_call:
                if j >= len(lines):
                    break
                macro_call += '\n' + lines[j]
                paren_count += lines[j].count('(') - lines[j].count(')')
                j += 1

            # Now we have the complete macro call
            # Find the semicolon position
            semicolon_pos = macro_call.find(';')
            if semicolon_pos == -1:
                # No semicolon found, just append and continue
                result.append(line)
                i += 1
                continue

            before_semi = macro_call[:semicolon_pos + 1]
            after_semi = macro_call[semicolon_pos + 1:]

            # Wrap the macro call in PERM_IGNORE
            if after_semi.strip():
                # There's content after the semicolon on the same line
                wrapped = f"{before_macro}PERM_IGNORE({before_semi}){after_semi}"
            else:
                wrapped = f"{before_macro}PERM_IGNORE({before_semi})"

            # Handle indentation for multi-line calls
            if wrapped.count('\n') > 0:
                wrapped_lines = wrapped.split('\n')
                result.append(wrapped_lines[0])
                for wrapped_line in wrapped_lines[1:]:
                    result.append(leading_ws + ' ' * 4 + wrapped_line)  # Indent continuation
            else:
                result.append(wrapped)

            # Add any lines that were part of the macro call (already handled above)
            i = j
        else:
            result.append(line)
            i += 1

    return '\n'.join(result)


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]

    try:
        with open(input_file, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: {input_file} not found")
        sys.exit(1)

    wrapped_content = wrap_gbi_macros(content)

    with open(input_file, 'w') as f:
        f.write(wrapped_content)

    print(f"Wrapped gbi macros in {input_file}")


if __name__ == '__main__':
    main()
