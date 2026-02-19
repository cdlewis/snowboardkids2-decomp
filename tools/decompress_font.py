#!/usr/bin/env python3
"""
Decompress font asset and export as PNG image.

Compression format (LZ77-like):
- Read 2 bytes at a time
- If first byte == 0: literal - output second byte
- Otherwise:
  - Lower nibble of first byte + second byte = 12-bit offset
  - Upper nibble of first byte = count of bytes to copy from offset

Text encoding format:
- Bits 12-15: Character width in pixels (0 = 12)
- Bits 0-11: Character index into font table
- Control codes: 0xFFFB=spacing, 0xFFFD=newline, 0xFFFF=end
"""

import os
import struct
import sys

# Try to import PIL for image export
try:
    from PIL import Image
    HAS_PIL = True
except ImportError:
    HAS_PIL = False


def decompress_lz77(compressed_data: bytes, expected_size: int) -> bytes:
    """Decompress LZ77-compressed data."""
    output = bytearray()
    src_offset = 0
    data_len = len(compressed_data)

    while src_offset < data_len and len(output) < expected_size:
        byte1 = compressed_data[src_offset]
        byte2 = compressed_data[src_offset + 1] if src_offset + 1 < data_len else 0

        if byte1 == 0:
            # Literal byte
            output.append(byte2)
        else:
            # Copy from offset
            # Lower nibble of byte1 + byte2 = 12-bit offset
            offset = ((byte1 & 0x0F) << 8) | byte2
            # Upper nibble of byte1 = count
            count = (byte1 >> 4) & 0x0F

            for _ in range(count):
                if len(output) > offset:
                    output.append(output[-offset])
                else:
                    output.append(0)  # Fallback if offset is invalid

        src_offset += 2

    return bytes(output[:expected_size])


def parse_sprite_data(data: bytes):
    """Parse sprite sheet data structure.

    SpriteSheetData:
    - 0x00: s32 textureBase
    - 0x04: s32 numFrames
    - 0x08: SpriteFrameEntry frames[]

    SpriteFrameEntry (16 bytes each):
    - 0x00: s32 textureOffset
    - 0x04: u16 paletteIndex
    - 0x06: u16 width
    - 0x08: u16 height
    - 0x0A: u16 paletteTableIndex
    - 0x0C: u16 formatIndex
    - 0x0E: u16 pad
    """
    print(f"Data size: {len(data)} bytes (0x{len(data):X})")
    print(f"First 64 bytes: {data[:64].hex()}")

    # Parse as big-endian (N64 is big-endian)
    texture_base = struct.unpack('>I', data[0:4])[0]
    num_frames = struct.unpack('>I', data[4:8])[0]

    print(f"\nSprite header (big-endian):")
    print(f"  textureBase: 0x{texture_base:08X}")
    print(f"  numFrames: {num_frames}")

    if num_frames > 0 and num_frames < 1000:  # Sanity check
        print(f"\nParsing {num_frames} frame entries:")
        frames = []
        for i in range(num_frames):  # Parse all frames
            offset = 8 + i * 16
            if offset + 16 > len(data):
                break

            tex_offset = struct.unpack('>I', data[offset:offset+4])[0]
            pal_index = struct.unpack('>H', data[offset+4:offset+6])[0]
            width = struct.unpack('>H', data[offset+6:offset+8])[0]
            height = struct.unpack('>H', data[offset+8:offset+10])[0]
            pal_table = struct.unpack('>H', data[offset+10:offset+12])[0]
            fmt_index = struct.unpack('>H', data[offset+12:offset+14])[0]

            frames.append({
                'index': i,
                'textureOffset': tex_offset,
                'paletteIndex': pal_index,
                'width': width,
                'height': height,
                'paletteTableIndex': pal_table,
                'formatIndex': fmt_index
            })

            if i < 10:  # Print first 10 frames
                print(f"  Frame {i}: offset=0x{tex_offset:X}, size={width}x{height}, "
                      f"palIdx={pal_index}, palTable={pal_table}, fmt={fmt_index}")

        if num_frames > 10:
            print(f"  ... and {num_frames - 10} more frames")

        return {
            'textureBase': texture_base,
            'numFrames': num_frames,
            'frames': frames
        }

    # If numFrames doesn't make sense, try analyzing the raw data differently
    print(f"\nnumFrames looks invalid, analyzing raw data patterns...")

    # Look for patterns that could be width/height pairs
    for i in range(0, min(64, len(data) - 4), 2):
        val1 = struct.unpack('>H', data[i:i+2])[0]
        val2 = struct.unpack('>H', data[i+2:i+4])[0]
        # Check if these could be reasonable dimensions (< 512)
        if 8 <= val1 <= 256 and 8 <= val2 <= 256:
            print(f"  Offset 0x{i:X}: {val1}x{val2}")

    return None


def export_sprite_frames(data: bytes, sprite_info: dict, output_dir: str):
    """Export individual sprite frames as images."""
    if not HAS_PIL:
        print("PIL not available, cannot export images")
        return

    if not sprite_info or 'frames' not in sprite_info:
        print("No sprite frame info available")
        return

    # Format indices: 0 = CI4 (4-bit), others = CI8 (8-bit)
    # Palette modes: 2 = CI mode

    all_frames = []
    for frame in sprite_info['frames']:  # Export all frames
        width = frame['width']
        height = frame['height']
        tex_offset = frame['textureOffset']
        fmt_idx = frame['formatIndex']

        # Calculate texture size
        if fmt_idx == 0:  # CI4 (4-bit)
            texture_size = (width * height) // 2
        else:  # CI8 (8-bit)
            texture_size = width * height

        # Get texture data
        texture_start = sprite_info['textureBase'] + tex_offset
        if texture_start + texture_size > len(data):
            print(f"  Frame {frame['index']}: texture out of bounds")
            continue

        texture_data = data[texture_start:texture_start + texture_size]

        # Create grayscale image (no palette interpretation)
        img = Image.new('L', (width, height))

        if fmt_idx == 0:  # CI4
            for y in range(height):
                for x in range(width):
                    byte_idx = (y * width + x) // 2
                    if byte_idx < len(texture_data):
                        byte = texture_data[byte_idx]
                        if x % 2 == 0:
                            pixel = (byte >> 4) & 0x0F
                        else:
                            pixel = byte & 0x0F
                        # Scale 4-bit to 8-bit
                        img.putpixel((x, y), pixel * 17)
        else:  # CI8
            for y in range(height):
                for x in range(width):
                    idx = y * width + x
                    if idx < len(texture_data):
                        img.putpixel((x, y), texture_data[idx])

        all_frames.append((frame['index'], img))

        output_path = os.path.join(output_dir, f"frame_{frame['index']:03d}.png")
        img.save(output_path)

    print(f"  Exported {len(all_frames)} individual frame images")

    # Create combined grid image
    if all_frames:
        cols = 16  # 16 columns
        rows = (len(all_frames) + cols - 1) // cols
        cell_width = max(img.width for _, img in all_frames)
        cell_height = max(img.height for _, img in all_frames)

        # Add small padding between cells
        padding = 2
        grid_img = Image.new('L', (
            cols * (cell_width + padding) + padding,
            rows * (cell_height + padding) + padding
        ), color=128)  # Gray background

        for i, (idx, frame_img) in enumerate(all_frames):
            col = i % cols
            row = i // cols
            x = padding + col * (cell_width + padding)
            y = padding + row * (cell_height + padding)
            grid_img.paste(frame_img, (x, y))

        grid_path = os.path.join(output_dir, "font_texture.png")
        grid_img.save(grid_path)
        print(f"  Created combined grid image: {grid_path} ({grid_img.width}x{grid_img.height})")


def export_as_image(data: bytes, width: int, height: int, output_path: str, format_type: str = 'IA8'):
    """Export decompressed data as image.

    N64 texture formats:
    - IA8: 8-bit intensity/alpha (1 byte per pixel)
    - RGBA16: 16-bit RGBA (2 bytes per pixel)
    - CI8: 8-bit color index
    """
    if not HAS_PIL:
        print(f"PIL not available. Raw data would be {width}x{height} = {width*height} bytes")
        print(f"Actual data size: {len(data)} bytes")
        return False

    if format_type == 'IA8':
        # 1 byte per pixel: 4 bits intensity, 4 bits alpha
        # Or sometimes just 8-bit grayscale
        pixels_needed = width * height
        if len(data) < pixels_needed:
            print(f"Warning: Data too small for {width}x{height} IA8 image")
            # Pad with zeros
            data = data + b'\x00' * (pixels_needed - len(data))

        img = Image.new('L', (width, height))  # Grayscale
        for y in range(height):
            for x in range(width):
                idx = y * width + x
                if idx < len(data):
                    # Treat as grayscale
                    img.putpixel((x, y), data[idx])

        img.save(output_path)
        print(f"Exported grayscale image to {output_path}")
        return True

    elif format_type == 'RGBA16':
        # 2 bytes per pixel: rrrrrggg ggbbbbba
        pixels_needed = width * height * 2
        if len(data) < pixels_needed:
            print(f"Warning: Data too small for {width}x{height} RGBA16 image")
            data = data + b'\x00' * (pixels_needed - len(data))

        img = Image.new('RGBA', (width, height))
        for y in range(height):
            for x in range(width):
                idx = (y * width + x) * 2
                if idx + 1 < len(data):
                    val = (data[idx] << 8) | data[idx + 1]
                    r = ((val >> 11) & 0x1F) << 3
                    g = ((val >> 6) & 0x1F) << 3
                    b = ((val >> 1) & 0x1F) << 3
                    a = 255 if (val & 1) else 0
                    img.putpixel((x, y), (r, g, b, a))

        img.save(output_path)
        print(f"Exported RGBA16 image to {output_path}")
        return True

    return False


def export_full_texture(data: bytes, output_dir: str, name: str):
    """Try to export the full texture as an image with different format assumptions."""
    if not HAS_PIL:
        print("PIL not available, cannot export images")
        return

    os.makedirs(output_dir, exist_ok=True)

    # 0x22E8 = 8936 bytes
    # Try various dimensions and formats
    test_configs = [
        # (width, height, format_name, description)
        (128, 70, 'IA8', '128x70 grayscale'),
        (64, 140, 'IA8', '64x140 grayscale'),
        (128, 71, 'IA8', '128x71 grayscale'),
        (96, 93, 'IA8', '96x93 grayscale'),
        (112, 80, 'IA8', '112x80 grayscale'),
        # Try with header offset (skip first 8 bytes)
        (128, 69, 'IA8', '128x69 (skip 8-byte header)'),
        (64, 138, 'IA8', '64x138 (skip 8-byte header)'),
    ]

    for width, height, fmt, desc in test_configs:
        expected_size = width * height
        if abs(len(data) - expected_size) <= 20:
            output_path = os.path.join(output_dir, f"{name}_{width}x{height}.png")
            print(f"\nTrying {desc} ({expected_size} bytes, have {len(data)})")

            # Pad data if needed
            texture_data = data
            if len(texture_data) < expected_size:
                texture_data = data + b'\x00' * (expected_size - len(data))

            img = Image.new('L', (width, height))
            for y in range(height):
                for x in range(width):
                    idx = y * width + x
                    if idx < len(texture_data):
                        img.putpixel((x, y), texture_data[idx])

            img.save(output_path)
            print(f"  Created: {output_path}")


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_dir = os.path.dirname(script_dir)

    # Input file
    input_path = os.path.join(project_dir, 'assets', 'font_main.bin')

    # Expected decompressed size from loadTextRenderAsset
    expected_size = 0x22E8  # 8936 bytes

    # Output directory
    output_dir = os.path.join(project_dir, 'build')

    print(f"Reading compressed data from: {input_path}")
    with open(input_path, 'rb') as f:
        compressed_data = f.read()

    print(f"Compressed size: {len(compressed_data)} bytes")
    print(f"Expected decompressed size: {expected_size} bytes (0x{expected_size:X})")

    # Decompress
    decompressed = decompress_lz77(compressed_data, expected_size)
    print(f"Actual decompressed size: {len(decompressed)} bytes")

    # Save raw decompressed data
    raw_output = os.path.join(output_dir, 'font_texture_raw.bin')
    os.makedirs(output_dir, exist_ok=True)
    with open(raw_output, 'wb') as f:
        f.write(decompressed)
    print(f"Saved raw decompressed data to: {raw_output}")

    # Analyze sprite structure
    print("\n=== Sprite Data Analysis ===")
    sprite_info = parse_sprite_data(decompressed)

    # Try to export as individual frames if sprite parsing succeeded
    if sprite_info and sprite_info.get('frames'):
        print("\n=== Exporting Sprite Frames ===")
        export_sprite_frames(decompressed, sprite_info, output_dir)

    # Also try exporting as full texture
    print("\n=== Full Texture Export ===")
    export_full_texture(decompressed, output_dir, 'font_texture')


if __name__ == '__main__':
    main()
