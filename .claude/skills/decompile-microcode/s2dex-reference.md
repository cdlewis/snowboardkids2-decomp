S2DEX Display List Commands

The S2DEX microcode enables Super NES-like sprite and background rendering on the N64. It is designed for 2D expressions — sprites, rectangles, and background planes — and does not support 3D primitive drawing. All commands are 8 bytes long (one Gfx word), except G_SELECT_DL which is 16 bytes (two Gfx words).

S2DEX is derived from the F3DEX GBI. This project uses the F3DEX_GBI_2 variant, so opcodes listed here are the F3DEX_GBI_2 values.

Macros are defined in `lib/ultralib/include/PR/gs2dex.h`. The `g*` variants take a display list pointer as the first argument; the `gs*` variants are for static display list initialization.

S2DEX Commands (F3DEX_GBI_2)

01: G_OBJ_RECTANGLE

Draws a 2D object as an axis-aligned rectangle. The object is defined by a uObjSprite structure.

01 00 00 00 [SS SS SS SS]

S    Segmented address of uObjSprite structure

Macros:
  gSPObjRectangle(pkt, mptr)     — dynamic display list
  gsSPObjRectangle(mptr)         — static display list

Encoding: gDma0p(pkt, G_OBJ_RECTANGLE, mptr, 0)

02: G_OBJ_SPRITE

Draws a 2D object as a sprite. Unlike rectangles, sprites support rotation via the 2D matrix.

02 00 00 00 [SS SS SS SS]

S    Segmented address of uObjSprite structure

Macros:
  gSPObjSprite(pkt, mptr)
  gsSPObjSprite(mptr)

Encoding: gDma0p(pkt, G_OBJ_SPRITE, mptr, 0)

04: G_SELECT_DL

Conditionally selects a display list based on a status flag. This is a two-word (16-byte) command — the first word uses G_RDPHALF_0 (0xE4) to store part of the data.

Word 1 (G_RDPHALF_0):
E4 [SS] [LL LL] [FF FF FF FF]

Word 2 (G_SELECT_DL):
04 00 [HH HH] [MM MM MM MM]

S    STATE ID (multiplied by 4: 0, 4, 8, or 12)
L    Low 16 bits of display list address
H    High 16 bits of display list address
F    Flag value
M    Mask value

The command tests (status[sid] & mask) against flag. If they match, the specified display list is executed.

Macros:
  gSPSelectDL(pkt, mptr, sid, flag, mask)       — with return (G_DL_PUSH)
  gSPSelectBranchDL(pkt, mptr, sid, flag, mask) — without return (G_DL_NOPUSH)
  gsSPSelectDL(mptr, sid, flag, mask)
  gsSPSelectBranchDL(mptr, sid, flag, mask)

Encoding: Two gDma1p calls — see gs2dex.h for full expansion.

05: G_OBJ_LOADTXTR

Loads texture data into TMEM. The texture descriptor specifies block, tile, or TLUT loading.

05 00 00 17 [SS SS SS SS]

S    Segmented address of uObjTxtr structure

The uObjTxtr union has three variants:
  uObjTxtrBlock_t (type = G_OBJLT_TXTRBLOCK = 0x00001033)
  uObjTxtrTile_t  (type = G_OBJLT_TXTRTILE  = 0x00fc1034)
  uObjTxtrTLUT_t  (type = G_OBJLT_TLUT      = 0x00000030)

Macros:
  gSPObjLoadTxtr(pkt, tptr)
  gsSPObjLoadTxtr(tptr)

Encoding: gDma0p(pkt, G_OBJ_LOADTXTR, tptr, 23)

06: G_OBJ_LDTX_SPRITE

Compound command: loads texture into TMEM and draws a sprite. Takes a uObjTxSprite structure (48 bytes = uObjTxtr + uObjSprite).

06 00 00 2F [SS SS SS SS]

S    Segmented address of uObjTxSprite structure

Macros:
  gSPObjLoadTxSprite(pkt, tptr)
  gsSPObjLoadTxSprite(tptr)

Encoding: gDma0p(pkt, G_OBJ_LDTX_SPRITE, tptr, 47)

07: G_OBJ_LDTX_RECT

Compound command: loads texture into TMEM and draws an axis-aligned rectangle. Takes a uObjTxSprite structure.

07 00 00 2F [SS SS SS SS]

S    Segmented address of uObjTxSprite structure

Macros:
  gSPObjLoadTxRect(pkt, tptr)
  gsSPObjLoadTxRect(tptr)

Encoding: gDma0p(pkt, G_OBJ_LDTX_RECT, tptr, 47)

08: G_OBJ_LDTX_RECT_R

Compound command: loads texture into TMEM and draws a rotated rectangle. Takes a uObjTxSprite structure.

08 00 00 2F [SS SS SS SS]

S    Segmented address of uObjTxSprite structure

Macros:
  gSPObjLoadTxRectR(pkt, tptr)
  gsSPObjLoadTxRectR(tptr)

Encoding: gDma0p(pkt, G_OBJ_LDTX_RECT_R, tptr, 47)

09: G_BG_1CYC

Draws a background rectangle using 1-cycle mode. Supports scaling via uObjScaleBg.

09 00 00 00 [SS SS SS SS]

S    Segmented address of uObjBg structure

Macros:
  gSPBgRect1Cyc(pkt, mptr)
  gsSPBgRect1Cyc(mptr)

Encoding: gDma0p(pkt, G_BG_1CYC, mptr, 0)

0A: G_BG_COPY

Draws a background rectangle using copy mode. Non-scalable, uses uObjBg_t directly.

0A 00 00 00 [SS SS SS SS]

S    Segmented address of uObjBg structure

Macros:
  gSPBgRectCopy(pkt, mptr)
  gsSPBgRectCopy(mptr)

Encoding: gDma0p(pkt, G_BG_COPY, mptr, 0)

0B: G_OBJ_RENDERMODE

Sets the rendering mode for subsequent 2D objects.

0B 00 00 00 [00 00 00 MM]

M    Render mode flags (OR combination)

Render mode flags:
  G_OBJRM_NOTXCLAMP    = 0x01  — disable texture clamping
  G_OBJRM_XLU          = 0x02  — translucent (ignored by hardware)
  G_OBJRM_ANTIALIAS    = 0x04  — anti-alias (ignored by hardware)
  G_OBJRM_BILERP       = 0x08  — bilinear interpolation
  G_OBJRM_SHRINKSIZE_1 = 0x10  — shrink size level 1
  G_OBJRM_SHRINKSIZE_2 = 0x20  — shrink size level 2
  G_OBJRM_WIDEN        = 0x40  — widen

Macros:
  gSPObjRenderMode(pkt, mode)
  gsSPObjRenderMode(mode)

Encoding: gImmp1(pkt, G_OBJ_RENDERMODE, mode)

DA: G_OBJ_RECTANGLE_R

Draws a 2D object as a rotated rectangle. Uses the current 2D matrix for rotation.

DA 00 00 00 [SS SS SS SS]

S    Segmented address of uObjSprite structure

Macros:
  gSPObjRectangleR(pkt, mptr)
  gsSPObjRectangleR(mptr)

Encoding: gDma0p(pkt, G_OBJ_RECTANGLE_R, mptr, 0)

DC: G_OBJ_MOVEMEM

Sets the 2D object matrix. This opcode serves two purposes depending on the parameters:

Matrix (full 24-byte uObjMtx):
DC 00 00 17 [SS SS SS SS]

Sub-matrix (8-byte uObjSubMtx, translation and scale only):
DC 02 00 07 [SS SS SS SS]

S    Segmented address of uObjMtx or uObjSubMtx structure

For full matrix:
  gSPObjMatrix(pkt, mptr)       — sets A, B, C, D, X, Y, BaseScaleX, BaseScaleY
  gsSPObjMatrix(mptr)

For sub-matrix:
  gSPObjSubMatrix(pkt, mptr)    — sets X, Y, BaseScaleX, BaseScaleY only
  gsSPObjSubMatrix(mptr)

Full matrix encoding:    gDma1p(pkt, G_OBJ_MOVEMEM, mptr, 0, 23)
Sub-matrix encoding:     gDma1p(pkt, G_OBJ_MOVEMEM, mptr, 2, 7)

The difference is in bits 16-23 of word 0: 0x00 for full matrix, 0x02 for sub-matrix.

E4: G_RDPHALF_0

Internal command used as the first word of the G_SELECT_DL two-word sequence. Not used directly.

See G_SELECT_DL (opcode 04) for details.

S2DEX Data Structures

uObjSprite_t (24 bytes)

  Offset  Type   Name          Description
  0x00    s16    objX          x-coordinate of upper-left (s10.2)
  0x02    u16    scaleW        width scale (u5.10)
  0x04    u16    imageW        texture width (u10.5)
  0x06    u16    paddingX      unused, always 0
  0x08    s16    objY          y-coordinate of upper-left (s10.2)
  0x0A    u16    scaleH        height scale (u5.10)
  0x0C    u16    imageH        texture height (u10.5)
  0x0E    u16    paddingY      unused, always 0
  0x10    u16    imageStride   texture fold width (64-bit words)
  0x12    u16    imageAdrs     TMEM address (64-bit words)
  0x14    u8     imageFmt      texel format (G_IM_FMT_*)
  0x15    u8     imageSiz      texel size (G_IM_SIZ_*)
  0x16    u8     imagePal      palette number (0-7)
  0x17    u8     imageFlags    display flags (G_OBJ_FLAG_FLIPS, G_OBJ_FLAG_FLIPT)

uObjBg_t (40 bytes) — Non-scalable background

  Offset  Type   Name          Description
  0x00    u16    imageX        texture x-coordinate (u10.5)
  0x02    u16    imageW        texture width (u10.2)
  0x04    s16    frameX        frame upper-left x (s10.2)
  0x06    u16    frameW        frame width (u10.2)
  0x08    u16    imageY        texture y-coordinate (u10.5)
  0x0A    u16    imageH        texture height (u10.2)
  0x0C    s16    frameY        frame upper-left y (s10.2)
  0x0E    u16    frameH        frame height (u10.2)
  0x10    u64*   imagePtr      DRAM texture source address
  0x14    u16    imageLoad     G_BGLT_LOADBLOCK or G_BGLT_LOADTILE
  0x16    u8     imageFmt      texel format (G_IM_FMT_*)
  0x17    u8     imageSiz      texel size (G_IM_SIZ_*)
  0x18    u16    imagePal      palette number
  0x1A    u16    imageFlip     inversion flag (G_BG_FLAG_FLIPS)
  0x1C    u16    tmemW         TMEM width (set by guS2DInitBg)
  0x1E    u16    tmemH         TMEM loadable height (set by guS2DInitBg)
  0x20    u16    tmemLoadSH    SH value (set by guS2DInitBg)
  0x22    u16    tmemLoadTH    TH/stride value (set by guS2DInitBg)
  0x24    u16    tmemSizeW     image line skip (set by guS2DInitBg)
  0x26    u16    tmemSize      loading skip (set by guS2DInitBg)

Fields from 0x1C onward are set by guS2DInitBg() — no need to set them manually.

uObjScaleBg_t (40 bytes) — Scalable background

  Same as uObjBg_t through offset 0x1A, then:
  0x1C    u16    scaleW        x-direction scale (u5.10)
  0x1E    u16    scaleH        y-direction scale (u5.10)
  0x20    s32    imageYorig    drawing start point on image (s20.5)
  0x24    u8[4]  padding

uObjMtx_t (24 bytes) — 2D affine matrix

  Offset  Type   Name          Description
  0x00    s32    A             matrix element (s15.16)
  0x04    s32    B             matrix element (s15.16)
  0x08    s32    C             matrix element (s15.16)
  0x0C    s32    D             matrix element (s15.16)
  0x10    s16    X             translation x (s10.2)
  0x12    s16    Y             translation y (s10.2)
  0x14    u16    BaseScaleX    base x scale (u5.10)
  0x16    u16    BaseScaleY    base y scale (u5.10)

uObjSubMtx_t (8 bytes) — Partial matrix (translation + scale only)

  Offset  Type   Name          Description
  0x00    s16    X             translation x (s10.2)
  0x02    s16    Y             translation y (s10.2)
  0x04    u16    BaseScaleX    base x scale (u5.10)
  0x06    u16    BaseScaleY    base y scale (u5.10)

uObjTxtrBlock_t (24 bytes) — Block texture load

  Offset  Type   Name          Description
  0x00    u32    type          G_OBJLT_TXTRBLOCK (0x00001033)
  0x04    u64*   image         DRAM texture source address
  0x08    u16    tmem          TMEM word address (8-byte words)
  0x0A    u16    tsize         texture size (use GS_TB_TSIZE macro)
  0x0C    u16    tline         1-line width (use GS_TB_TLINE macro)
  0x0E    u16    sid           STATE ID (0, 4, 8, or 12)
  0x10    u32    flag          STATE flag
  0x14    u32    mask          STATE mask

uObjTxtrTile_t (24 bytes) — Tile texture load

  Offset  Type   Name          Description
  0x00    u32    type          G_OBJLT_TXTRTILE (0x00fc1034)
  0x04    u64*   image         DRAM texture source address
  0x08    u16    tmem          TMEM word address (8-byte words)
  0x0A    u16    twidth        texture width (use GS_TT_TWIDTH macro)
  0x0C    u16    theight       texture height (use GS_TT_THEIGHT macro)
  0x0E    u16    sid           STATE ID (0, 4, 8, or 12)
  0x10    u32    flag          STATE flag
  0x14    u32    mask          STATE mask

uObjTxtrTLUT_t (24 bytes) — TLUT (palette) load

  Offset  Type   Name          Description
  0x00    u32    type          G_OBJLT_TLUT (0x00000030)
  0x04    u64*   image         DRAM palette source address
  0x08    u16    phead         palette start (256-511, use GS_PAL_HEAD macro)
  0x0A    u16    pnum          palette count - 1 (use GS_PAL_NUM macro)
  0x0C    u16    zero          always 0
  0x0E    u16    sid           STATE ID (0, 4, 8, or 12)
  0x10    u32    flag          STATE flag
  0x14    u32    mask          STATE mask

uObjTxSprite (48 bytes) — Combined texture + sprite

  Offset  Type         Name     Description
  0x00    uObjTxtr     txtr     texture loading descriptor (24 bytes)
  0x18    uObjSprite   sprite   sprite descriptor (24 bytes)

Helper Macros

GS_TB_TSIZE(pix, siz)   — calculate tsize for uObjTxtrBlock_t
GS_TB_TLINE(pix, siz)   — calculate tline for uObjTxtrBlock_t
GS_TT_TWIDTH(pix, siz)  — calculate twidth for uObjTxtrTile_t
GS_TT_THEIGHT(pix, siz) — calculate theight for uObjTxtrTile_t
GS_PAL_HEAD(head)        — calculate phead for uObjTxtrTLUT_t (head + 256)
GS_PAL_NUM(num)          — calculate pnum for uObjTxtrTLUT_t (num - 1)
GS_CALC_DXT(line)        — calculate DXT value
GS_PIX2TMEM(pix, siz)   — convert pixel count to TMEM words
GS_PIX2DXT(pix, siz)    — convert pixel count to DXT value

Status and Render Mode

gSPSetStatus(pkt, sid, val) — set general status for conditional DL selection
  sid: STATE ID (0, 4, 8, or 12)
  val: status value

gSPObjRenderMode(pkt, mode) — set 2D object render mode
  mode: OR combination of G_OBJRM_* flags

Initialization

guS2DInitBg(bg) — initialize a uObjBg structure. Sets the tmem* fields (0x1C-0x26) based on the image parameters. Must be called before using gSPBgRect1Cyc or gSPBgRectCopy with a non-scalable background.
