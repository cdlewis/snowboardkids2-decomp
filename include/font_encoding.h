#pragma once

/*
 * Text encoding format:
 * - 16-bit big-endian values
 * - Bits 12-15: Character width in pixels (0 = default 12, 8 = small)
 * - Bits 0-11: Character index into font table (0-93)
 *
 * Control codes use 0xFF in the high byte.
 *
 * Usage: u8 text[] = { _("Hello World") };
 *
 * The textconv tool (run during build) converts _("string") to hex byte arrays.
 * An END terminator is automatically appended.
 * This macro is just a placeholder for IDE/clang-check - the build process
 * runs textconv BEFORE the preprocessor, so this never gets expanded.
 *
 * Digit sizing is context-aware:
 *   - After lowercase: small digits (e.g., "by 8:30" -> small 8)
 *   - After uppercase: big digits (e.g., "LEVEL 1" -> big 1)
 *   - Explicit override: {0B}-{9B} for big, {0s}-{9s} for small
 *
 * See tools/charmap.txt for the full character mapping.
 */
#define _(str) 0
#define PAD2 0x00, 0x00
