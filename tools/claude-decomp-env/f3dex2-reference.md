Fast3DEX2 Display List Commands

The Fast3DEX2 display list commands are microcode that are passed to the RSP for rendering. They are all 8 bytes long.

Fast3DEX2 Commands

00: G_SPNOOP

Stalls the signal processor (the RSP), and as a consequence, the RDP too. This should only be used for debugging purposes.

00 00 00 00 00 00 00 00

01: G_VTX

Fills the vertex buffer with vertex information (ex. coordinates, color values). Max amount of vertices to load in F3DEX2 is 0x20 (32).

01 0[N N]0 [II] [SS SS SS SS]

N	Number of vertices to write
I	Where to start writing vertices inside the vertex buffer (start = II - N*2)
S	Segmented address to load vertices from
Example: Load 4 vertices from 0x0E000780 and put them into the vertex buffer, starting at position 0.

01 00 40 08 0E 00 07 80
02: G_MODIFYVTX

Modifies a four-byte portion of the vertex specified.

02 ww nn nn vv vv vv vv

w	Enumerated set of values specifying what to change
n	Vertex buffer index of vertex to modify (*2)
v	New value to insert
The portion modified is specified by w, and the new value is given in v. Lighting calculations (if enabled) and position transformations are not calculated by the RSP after use of this command, so modifications modify final color and vertices.

The valid values for w have names as follows:

G_MWO_POINT_RGBA = 0x10 Modifies the color of the vertex
G_MWO_POINT_ST = 0x14 Modifies the texture coordinates
G_MWO_POINT_XYSCREEN = 0x18 Modifies the X and Y position
G_MWO_POINT_ZSCREEN = 0x1C Modifies the Z position (lower four nybbles of v should always be zero for this modification)
The exact nature of these values is unclear. The SDK documentation describes them as “byte offsets”, however they don't match offsets in the vertex structure.

03: G_CULLDL

03 00 vv vv 00 00 ww ww

v	Vertex buffer index of first vertex for bounding volume (*0x02)
w	Vertex buffer index of last vertex for bounding volume (*0x02)
This command takes the vertices in the vertex buffer from vfirst through vlast as describing the volume of the object being rendered (called the “bounding value”). If the bounding volume does not intersect with the current viewing volume (aka if the bounding volume is entirely offscreen), then the display list ends, equivalent to a B8 opcode. Otherwise, the display list continues as though nothing happened (equivalent to 00, then).

For F3DEX2, Each of vfirst and vlast must be in the range 0 ≤ x ≤ 31. Additionally, vfirst < vlast must be true (thus a minimum of two vertices must be specified, and range of vertices cannot be reversed). It's not specified what the behavior is when all the vertices are coplanar.

04: G_BRANCH_Z

Tests the Z value of the vertex at a buffer index against a given z-value.

E1 00 00 00 [dd dd dd dd]

04 [aa a][b bb] [zz zz zz zz]

d	Address of display list to branch to
a	Vertex buffer index of vertex to test(*5)
b	Vertex buffer index of vertex to test(*2)
z	Z value to test against
If the given vertex's Z value ≤ z, then the processor switches over to the display list at address d (equivalent to 06 jump with no return stored). Otherwise continues through the display list. Useful for LOD-related model processing, where several occurrences of this can be stacked to branch to progressively closer versions of the model.

Note that the 04 opcode actually pulls the address of the branching display list from the high half of the “RDP word” (16 bytes used for generic value storage, as far as is known). The italicized opcode given above is set by the “basic function” listed for this opcode, immediately before the actual 04 opcode.

Although not stated in the documentation, the vertex buffer index is presumably limited to the range 0 ≤ index ≤ 31 just like other buffer indices. It's unknown why the opcode needs the index twice, and multiplied by different amounts.

Also, z is usually calculated for the N64 programmer (only the *Zraw form specifies a raw z value). The formula used in these cases is as shown, written as C code:

#include <stdint.h>

int32_t FTOFIX32(float x) {

  return (int32_t)( x * 0x00010000);
}

/* note: parameter types taken from docs on gSPBranchLessZrg(…) */
uint32_t calcZVal(float zval, int32_t flag, float near, float far, int32_t zmin, int32_t zmax) {
    float part1 = (flag == G_BZ_PERSP)
                ? (1.0f - near / zval) / (1.0f - near / far)
                : (zval - near) / (far - near);
    return (uint32_t)FTOFIX32(part1) * ((zmax - zmin) & ~1) + FTOFIX32((float)zmin);
}

Where zval is the Z value the programmer is thinking of comparing against (whereas the “real” Z value handed to the opcode is calculated by this function), near and far denote the distances of the near and far clipping planes, respectively, from the viewpoint. zmin and zmax are the minimum and maximum possible Z values, respectively. flag decides on the projection, taking either of these enumerations:

G_BZ_PERSP = 0
G_BZ_ORTHO = 1
As their names would suggest, PERSP handles perspective projection, ORTHO handles orthographic.
Note that in a default setup, zmin is 0, and zmax is 0x03FF.

This formula cannot be reversed for the value provided in the microcode, since the necessary information is absent. (Since the values are provided in the macro call generating this opcode, there's no guarantee any of the values match what they're “supposed” to be, e.g. near could be set to something far different from the location of the actual near clipping plane.)

Note: This command works differently in F3DZEX.

05: G_TRI1

Renders one triangle according to the vertices inside the vertex buffer

05 [AA] [BB] [CC] 00 00 00 00

A	First vertex to use for the triangle (* 0x02)
B	Second vertex to use for the triangle (* 0x02)
C	Third vertex to use for the triangle (* 0x02)
Example: Render a triangle using the vertex data specified at the vertex buffer positions 0, 1 (0x02 / 0x02) and 2 (0x04 / 0x02).

05 00 02 04 00 00 00 00
06: G_TRI2

Renders two triangles according to the vertices inside the vertex buffer

06 [AA] [BB] [CC] 00 [DD] [EE] [FF]

A	First vertex to use for the triangle (* 0x02)
B	Second vertex to use for the triangle (* 0x02)
C	Third vertex to use for the triangle (* 0x02)
D	Fourth vertex to use for the triangle (* 0x02)
E	Fifth vertex to use for the triangle (* 0x02)
F	Sixth vertex to use for the triangle (* 0x02)
Example: Render a triangle using the vertex data specified at the vertex buffer positions 0, 1 (2/2) and 2 (4/2), along with a second triangle at vertex buffer positions 3 (6/2), 4 (8/2), and 5 (0x0A/2).

06 00 02 04 00 06 08 0A
07: G_QUAD

Renders one quad according to the vertices inside the vertex buffer

07 [AA] [BB] [CC] 00 [DD] [EE] [FF]

A	First vertex to use for the quad (*2)
B	Second vertex to use for the quad (*2)
C	Third vertex to use for the quad (*2)
D	Fourth vertex to use for the quad (*2)
E	Fifth vertex to use for the quad (*2)
F	Sixth vertex to use for the quad (*2)
Example: Render a quad using the vertex data specified at the vertex buffer positions 0, 1 (2/2), 2 (4/2), 0, 2 (4/2) and 3 (6/2).

07 00 02 04 00 00 04 06
Note: This opcode is functionally equivalent to opcode 06. The macro for this opcode (and thus presumably also the opcode itself) is labeled as no longer supported in Oct 1999 SDK documentation.

D3: G_SPECIAL_3

Appears to be an explicitly reserved command.

D3 ?? ?? ?? ?? ?? ?? ??

D4: G_SPECIAL_2

Appears to be an explicitly reserved command.

D4 ?? ?? ?? ?? ?? ?? ??

D5: G_SPECIAL_1

Appears to be an explicitly reserved command.

D5 ?? ?? ?? ?? ?? ?? ??

D6: G_DMA_IO

Does a DMA between DMEM/IMEM.

D6 [xx x][s ss] [rrrrrrrr]

xx = fmmm mmmm mmm0 (binary)

f	Chooses between read or write
m	((Address in DMEM/IMEM)/8) & 0x3FF
s	(Presumably) size of data to transfer (-1)
r	DRAM address
s bytes are presumably transferred in the process. f determines the type of transfer. Apparently:

f == 0: Read from DMEM/IMEM to DRAM
f == 1: Write DRAM to DMEM/IMEM
The exact nature of this command is unclear, since none of this opcode's macros are documented, and the only available comment suggests this is a debugging tool only. Therefore, you should not expect to see this in production code.

D7: G_TEXTURE

Sets the texture scaling factor.

D7 00 [xx] [nn] [ss ss] [tt tt]

xx = 00LL Lddd

n	Enable/Disable Tile Descriptor (2=enable, 0=disable)
s	Scaling factor for S axis (horizontal)
t	Scaling factor for T axis (vertical)
L	Maximum number of mipmap levels aside from the first
d	Tile descriptor to enable/disable
Note that scaling factors are binary fractional values with an implied 0.; for example, a scaleS of 0x8000 means 0b0.1000_0000_0000_0000, or 0d0.5.

Examples:

D7 00 00 02 FF FF FF FF : Standard - at start for extremely close to 1 scaling enabled on tile desc 0.
D7 00 00 02 80 00 80 00 : Standard - at start for 0.5 scaling enabled on tile desc 0.
D7 00 00 02 0F 80 07 C0 : Scale for environment mapping 64x32 and enable tile desc 0.
D7 00 00 00 FF FF FF FF : Disable tile descriptor 0 (end of DL).
D8: G_POPMTX

Pops num matrices from the stack specified by modelview matrix stack.

D8380002 aaaaaaaa

a	(The number of matrices to pop) * 64
D9: G_GEOMETRYMODE

Enables and disables certain geometry parameters (ex. lighting, front-/backface culling, Z-buffer).

D9 [CC CC CC] [SS SS SS SS]

C	~(Various parameters to clear)
S	Various parameters to set
Note: Parameters that are cleared have their bits inverted in the opcode. FF FF FF, for example, would disable nothing.

Parameters:
See RSP Geometry Mode under RCP Structs.

DA: G_MTX

Apply transformation matrix. Used in HUD text printing code to translate textures, as well as copious other transformations including animations.

DA 38 00 [PP] [AA AA AA AA]

P	Parameters ^ G_MTX_PUSH
A	Segmented address of vectors
Parameters (can be OR'd together):

0x04	projection (default: model view)
0x02	load (default: multiply)
0x01	push (default: no push)
Example: Push matrix and and multiply by matrix at 0x00213DF8; Multiply by matrix at 0x00213DB8

DA 38 00 00 00 21 3D F8
DA 38 00 01 00 21 3D B8
DB: G_MOVEWORD

Loads a new 32-bit value data to the location specified by index and offset.

DB ii oo oo dd dd dd dd

i	Index into DMEM pointer table(?)
o	Offset from the indexed base address(?)
d	New 32-bit value
i is an index into a table of DMEM addresses. The enumerations given for this are:

G_MW_MATRIX	0x00	Allows direct modification of part of the current MP (modelview * projection) matrix (which is calculated upon loading of either kind of matrix), modifying two elements' integer or fractional parts.
G_MW_NUMLIGHT	0x02	Specifies the number of directional/diffuse lights. (The lighting system always has an ambient light.)
G_MW_CLIP	0x04	Specifies the ratio between the clipping and scissoring boxes.
G_MW_SEGMENT	0x06	Sets up the physical address of a segment.
G_MW_FOG	0x08	Specifies the multiplier and offset for fog effects.
G_MW_LIGHTCOL	0x0A	Changes the color of a light without changing the direction
G_MW_FORCEMTX	0x0C	Tells the RCP if the MP matrix has been forcibly changed(?)
G_MW_PERSPNORM	0x0E	Sets amount by which to scale the W axis value before dividing X, Y, and Z by it.
The offset is, as the name suggests, an offset from the address that index resolves to. Enumerations are also provided for this value, for each of the enumerated index values. (XXX where to put them? It's a bit of a list.)

DC: G_MOVEMEM

Takes a block of memory from an address and puts it in the location pointed to by an index and an offset.

DC [nn] [oo] [ii] [aa aa aa aa]

n	(((Size in bytes of memory to be moved) » 3)+1)*8
o	Offset from indexed base address (*8)
i	Index into table of DMEM addresses
a	Segmented address of memory
i is an index into a table of addresses of DMEM. Given enumerations for i are:

G_MV_MMTX = 2
G_MV_PMTX = 6
G_MV_VIEWPORT = 8
G_MV_LIGHT = 10
G_MV_POINT = 12
G_MV_MATRIX = 14
Note however that only VIEWPORT, LIGHT, and MATRIX are used by any of the macros given to programmers.

Example: Loads diffuse RGBA from 0x0 in Segment 0x0E; loads ambient RGBA from 0x08 in Segment 0x0E

DC 86 00 10 0E 00 00 00
DC 88 00 10 0E 00 00 08
DD: G_LOAD_UCODE

Loads a new microcode executable into the RSP.

E1 00 00 00 [dd dd dd dd]
DD 00 [ss ss] [tt tt tt tt]

d points to the start of the data section and t to the start of the text section, with s specifying the size of the data section. After loading the new microcode, the RCP is reset as appropriate.

Note that d is stored in the high half of the “RDP word”, the opcode for which is shown in italics before the actual DD opcode.

DE: G_DL

Signifies the start of a Display List. May be used to link data and branch the current DL.

DE [AA] 00 00 [BB BB BB BB]

AA	00 = store return address, 01 = don't store (end DL after branch)
B	Segmented Address to branch to
Example: Loads a display list from 0xA50 in segment 0x07.

DE 00 00 00 07 00 0A 50
DF: G_ENDDL

Terminates the current Display List

DF 00 00 00 00 00 00 00

E0: G_NOOP

Does nothing. Seemingly different from 00; judging by the names and position of this opcode and 00, this opcode stalls the RDP, whereas the other stalls the RSP.

E0 00 00 00 00 00 00 00

E1: G_RDPHALF_1

This opcode sets the higher half of the generic RDP word.

E1 00 00 00 [hh hh hh hh]

h	New value of high word
Note: the referenced function also generates opcode F1. See the section on F1 for info on that.

E2: G_SetOtherMode_L

Modifies various bits of the lower half of the RDP Other Modes word.

E2 00 [ss] [nn] [dd dd dd dd]

s	32 - (Amount mode-bits are shifted by) - (Number of bits affected)
n	(Number of mode-bits affected) - 1
d	Mode-bits
The operation performed by the opcode can be expressed as such, assuming LO to stand for the lower word of the RDP other modes:

LO = LO & ~(((1 « n) - 1) « s) | d

Or, in English, it clears all the bits designated to be modified (determined by s and n), and then sets those bits according to data. Important to note that mode-bits are preshifted. That is, it is up to the caller to shift data appropriately. The enumerations provided for data are pre-shifted to match this requirement.
The enumerations provided for s are:

G_MDSFT_ALPHACOMPARE = 0
G_MDSFT_ZSRCSEL = 2
G_MDSFT_RENDERMODE = 3
The structure of the lower modes is provided in the RCP structs page.

E3: G_SetOtherMode_H

Modifies various bits of the lower half of the RDP Other Modes word.

E3 00 [ss] [nn] [dd dd dd dd]

s	32 - (Amount mode-bits are shifted by) - (Number of bits affected)
n	(Number of mode-bits affected) - 1
d	Mode-bits
Like with E2, data is preshifted, and the opcode's procedure can be formulised in the same way (where OH stands for the high word of the other modes):

OH = OH & ~(((1 « n) - 1) « s) | d

The enumerations provided for s are:

G_MDSFT_BLENDMASK = 0
G_MDSFT_ALPHADITHER = 4
G_MDSFT_RGBDITHER = 6
G_MDSFT_COMBKEY = 8
G_MDSFT_TEXTCONV = 9
G_MDSFT_TEXTFILT = 12
G_MDSFT_TEXTLUT = 14
G_MDSFT_TEXTLOD = 16
G_MDSFT_TEXTDETAIL = 17
G_MDSFT_TEXTPERSP = 19
G_MDSFT_CYCLETYPE = 20
G_MDSFT_COLORDITHER = 22
G_MDSFT_PIPELINE = 23
The structure of the higher modes is provided in the RCP structs page.

E4: G_TEXRECT

Draws a textured 2D rectangle on the screen

E4 [xx x][y yy] 0[I] [XX X][Y YY]
E1 00 00 00 [SS SS] [TT TT]
F1 00 00 00 [DD DD] [EE EE]

xxx	Lower-right corner X coordinate
yyy	Lower-right corner Y coordinate
I	Tile descriptor to use for rectangle
XXX	Upper-left corner X coordinate
YYY	Upper-left corner Y coordinate
SSSS	Texture S coordinate at upper-left corner
TTTT	Texture T coordinate at upper-left corner
DDDD	Change in S coordinate over change in X coordinate
EEEE	Change in T coordinate over change in Y coordinate
E5: G_TEXRECTFLIP

Draws a textured 2D rectangle on the screen

E5 [xx x][y yy] 0[I] [XX X][Y YY]
E1 00 00 00 [SS SS] [TT TT]
F1 00 00 00 [DD DD] [EE EE]

xxx	Lower-right corner X coordinate
yyy	Lower-right corner Y coordinate
I	Tile descriptor to use for rectangle
XXX	Upper-left corner X coordinate
YYY	Upper-left corner Y coordinate
SSSS	Texture S coordinate at upper-left corner
TTTT	Texture T coordinate at upper-left corner
DDDD	Change in T coordinate over change in X coordinate
EEEE	Change in S coordinate over change in Y coordinate
This is similar to opcode E4, with the only difference being that the S and T coordinates of a texture are flipped in rendering, so that S coordinates are along the Y axis and T along the X axis. This essentially flips the texture about the diagonal line (XXX,YYY),(xxx,yyy).

E6: G_RDPLOADSYNC

This forces a wait for a texture to load, in order to synchronize with pixel rendering. This ensures that loading a new texture won't disrupt the rendering of primitives mid-render.

E6 00 00 00 00 00 00 00

E7: G_RDPPIPESYNC

This is not the “start of display list” command. There is no such thing.

Waits for the RDP to finish rendering its currently-rendering primitive, before updating RDP attributes. This avoids altering the rendering of a primitive in the middle of its render.

E7 00 00 00 00 00 00 00

Historically, several model viewers have interpreted this command as somehow starting a display list. This is not true. Display lists have no command that starts a display list. To find display lists, the most accurate way would be to find the end display list command (B8) and work backwards, until an invalid display list command is encountered, or to parse byte-code that points to display lists.

E8: G_RDPTILESYNC

Forces a wait for rendering to finish before updating tile descriptor attributes, so as to not disrupt rendering of primitives mid-render.

E8 00 00 00 00 00 00 00

E9: G_RDPFULLSYNC

Generates an interrupt for the main CPU when the RDP has finished doing everything. This is typically the last opcode before the “end display list” opcode (B8).

E9 00 00 00 00 00 00 00

EA: G_SETKEYGB

Sets the center, scale, and width parameters for the green and blue components for chroma key (see EB for red).

EA [ww w][x xx] [cc] [ss] [dd] [tt]

w	Scaled width of half the key window for green
x	Scaled width of half the key window for blue
c	Intensity of active key for green
s	Reciprocal of size of soft edge, normalized to 0..0xFF, for green
d	Intensity of active key for blue
t	Reciprocal of size of soft edge, normalized to 0..0xFF, for blue
w and x are in an unsigned fixed-point 4.8 format. The other parameters are normal 8-bit unsigned integers.

EB: G_SETKEYR

Sets the scale, center, and width parameters for the red component for chroma key (see EA for blue and green).

EB 00 00 00 0[w ww] [cc] [ss]

w	Scaled with of half the key window for red
c	Intensity of active key for red
s	Reciprocal of size of soft edge, normalized to 0..0xFF, for blue
Sets the scale, center, and width parameters for the red component for chroma key (see EA for blue and green).

w is an unsigned fixed-point 4.8 number. c and s are 8-bit unsigned integers.

EC: G_SETCONVERT

Sets the terms need to perform conversion from YUV to RGB.

   E    C  00aa aaaa  aaab bbbb  bbbb cccc
cccc cddd  dddd ddee  eeee eeef  ffff ffff

a	K0 term of conversion matrix
b	K1 term of conversion matrix
c	K2 term of conversion matrix
d	K3 term of conversion matrix
e	K4 term of conversion matrix
f	K5 term of conversion matrix
All of the parameters are signed 9-bit numbers, with range -256 ≤ k ≤ 255.

ED: G_SETSCISSOR

Sets the scissoring rectangle.

ED [xx x][y yy] 0[m] [vv v][w ww]

x	Upper-left X coordinate of rectangle
y	Upper-left Y coordinate of rectangle
m	Interpolation mode setting
v	Lower-right X coordinate of rectangle
w	Lower-right Y coordinate of rectangle
Valid values for m are:

G_SC_NON_INTERLACE = 0
G_SC_EVEN_INTERLACE = 2
G_SC_ODD_INTERLACE = 3
NON_INTERLACE draws all scanlines, while EVEN and ODD draw only even or odd scanlines, respectively.

EE: G_SETPRIMDEPTH

This sets the Z value for the entire primitive to be rendered, when the source for its depth component is set to be taken from this value. (This could be thought of as the depth version of “flat shading”.)

EE 00 00 00 zz zz dd dd

z	Z value for primitive
d	Delta Z value for primitive
z and d are signed 16-bit values. z sets the actual depth for the primitive, while d helps rendering of anti-aliased and decal objects (this is usually 0).

EF: G_RDPSetOtherMode

Sets the other mode bits of the RDP.

EF [HH HH HH] [LL LL LL LL]

H	Settings for other mode higher word bits
L	Settings for other mode lower word bits
H is similar to using E3, and L is similar to using E2.

The difference from either E3 or E2 is that this command sets all the bits in both halves of the RDP word (that is, this opcode has no way of limiting what bits are actually affected).

The RSP Geometry Mode word bits' parameters can be found under the RCP structs page.

F0: G_LOADTLUT

Tells the RDP to load a number of colors from the texture loaded to a given tile descriptor. The tile descriptor's attributes and other state related to textures inform the RDP how to load the palette, and where to load it to.

F0 00 00 00 0[t] [cc c]0 00

t	Tile descriptor to load from
c	((colour count-1) & 0x3FF) « 2
(Note: the count is quadrupled likely due to how palettes are stored in TMEM. Not sure where details on that would be appropriate, though.)

F1: G_RDPHALF_2

This opcode sets the low half of the generic RDP word.

F1 00 00 00 [LL LL LL LL]

L	New value of low word
Note: the referenced function also generates opcode E1. See the section on E1 for info on that.

F2: G_SETTILESIZE

Sets the texture coordinates and size

F2 [SS S][T TT] 0[I] [WW W][H HH]

SSS	Upper-left corner of texture to load, S-axis
TTT	Upper-left corner of texture to load, T-axis
I	Tile descriptor to load into
W	(width - 1) « 2
H	(height - 1) « 2
Examples:

F2 00 00 00 00 07 C0 7C : 32x32 textures
F2 00 00 00 00 0F C0 7C : 64x32
F2 00 00 00 00 07 C0 FC : 32x64
F3: G_LOADBLOCK

Determines how much data to load after SETTIMG

F3 [SS S][T TT] 0[I] [XX X][D DD]

SSS	Upper-left corner of texture to load, S-axis
TTT	Upper-left corner of texture to load, T-axis
I	Tile descriptor to load into
XXX	Number of texels to load to TMEM, minus one
DDD	dxt
Some notes on dxt:

dxt is an unsigned fixed-point 1.11 number, meaning a range of 0 ≤ n ≤ 1.999512. This number has units, namely lines•words-1, where a “word” is 64 bits in length, and a line is one row of the texture. (It may be more easily understood when inverted to get its reciprocal: words/lines. dxt is the reciprocal of the number of 64-bit chunks it takes to get a row of texture.)

When loading an image through this opcode, an internal counter is incremented by dxt for every 64 bits of texture read. When this counter is high enough to turn over to the next integer value (e.g. 0.9 + 0.15 → 1.05), the current line number is incremented. This is important as odd-numbered rows have their values swapped for interleaved access (the first line read in is line 0, and thus isn't swapped).

Note that if dxt is not a power of two (i.e. only one bit in the value is 'on'), then an error in the counter will accumulate over time, and eventually cause errors in determining where next lines are (when exactly this happens depends on the value and the type of texture being read in).

Examples:

F3 00 00 00 07 7F F1 00 : 32x64 16-bit Textures
F3 00 00 00 07 7F F0 80 : 64x32 16-bit Textures
F3 00 00 00 07 3F F1 00 : 32x32 16-bit Textures 
F3 00 00 00 07 FF F1 00 : 64x64 8-bit Textures 
F4: G_LOADTILE

Loads a rectangular portion of the texture being pointed to in DRAM for given tile descriptor into TMEM.

F4 [ss s][t tt] 0[i] [uu u][v vv]

s	Upper-left corner of tile, S-axis
t	Upper-left corner of tile, T-axis
i	Tile descriptor being loaded to
u	Lower-right corner of tile, S-axis
v	Lower-right corner of tile, T-axis
(s, t) specifies the upper-left corner of the texture in RAM, offset from its earlier-specified origin, and (u, v) specifies the lower-right corner of the texture to load.

All coordinate values are in unsigned fixed-point 10.2 format (range 0 ≤ n ≤ 1023.75).

F5: G_SETTILE

Sets the texture properties (ex. mirroring)

F5 [XX XX XX XX XX XX XX]

X	Bit-wise parameters
Bit-wise parameters:

  [F  5]  fffi i0nn nnnn nnnm mmmm mmmm

0000 0ttt pppp ccaa aass ssdd bbbb uuuu

fff	Sets color format
ii	Sets bit size of pixel
nnnnnnnnn	Number of 64-bit values per row
mmmmmmmmm	Offset of texture in TMEM
ttt	Tile descriptor being modified
pppp	Which palette to use for colors (if relevant)
cc	Clamp and Mirror flags for the T axis
aaaa	Sets how much of T axis is shown before wrapping
ssss	Sets the amount to shift T axis values after perspective division
dd	Clamp and Mirror flags for the S axis
bbbb	Sets how much of S axis is shown before wrapping
uuuu	Sets the amount to shift S axis values after perspective division
Color formats
0	RGBA	Color and alpha
1	YUV	Luminance and Chrominance
2	CI	Index and look-up palette
3	IA	Grayscale and alpha
4	I	Grayscale
Bit sizes
0	4-bit	I, IA, and CI
1	8-bit	I, IA, and CI
2	16-bit	RGBA, IA, and YUV
3	32-bit	RGBA
Or simply Bitsize = 4*2^(i)

Examples:

F5 10 00 00 07 00 00 00 : Settile for RGBA16, followed by another F5 command (rendertile)
F5 70 00 00 07 00 00 00 : Settile for IA16, followed by another F5 command (rendertile)
F5 10 10 00 07 01 40 50 : Rendertile for RGBA16 32x32 textures
F5 10 20 00 07 01 40 60 : Rendertile for RGBA16 64x32 textures
F5 70 10 00 00 01 40 50 : Rendertile for IA16 32x32 textures
F5 00 01 00 01 00 00 00 : Settile for palettes loading into TLUT0 in tile descriptor 1
F5 40 08 00 00 09 80 60 : Rendertile for CI4 64x64 textures with clamping on the T (vertical) axis
F6: G_FILLRECT

Draws a colored rectangle on the screen. Use command 0xF7 to set the color of the rectangle.

F6 [VV V][W WW] 00 [XX X][Y YY]

VVV	Lower-right corner of rectangle, X-axis
WWW	Lower-right corner of rectangle, Y-axis
XXX	Upper-left corner of rectangle, X-axis
YYY	Upper-left corner of rectangle, Y-axis
F7: G_SETFILLCOLOR

Sets the fill color for use in fill mode.

F7 00 00 00 [CC CC CC CC]

CCCCCCCC	Fill value for use in fill mode
F8: G_SETFOGCOLOR

Sets the fog color

F8 00 00 00 [RR] [GG] [BB] [AA]

R	red value
G	green value
B	blue value
A	alpha: Set to FF unless combiner uses 2-cycle mode
Example: set the fog for the following polygons to pure green

F8 00 00 00 00 FF 00 FF
F9: G_SETBLENDCOLOR

Sets the RDP blender's blend color, a general purpose color register available for various purposes in the blender.

F9 00 00 00 [RR] [GG] [BB] [AA]

R	red value
G	green value
B	blue value
A	alpha value
Example: make blend colour green and opaque

FB 00 00 00 00 FF 00 FF
FA: G_SETPRIMCOLOR

Sets the primitive color register of the RDP's color combiner. Also sets two additional values available to the color combiner, the minimum LOD level and an LOD fraction.

FA 00 [mm] [ff] [RR] [GG] [BB] [AA]

m	Minimum possible LOD value (clamped to this at minimum)
f	Primitive LOD fraction for mipmap filtering
R	red value
G	green value
B	blue value
A	alpha value
m and f are both fixed-point unsigned 0.8 numbers, meaning a range of 0 ≤ n ≤ 0.99609375. m defines the minimum possible value for LOD to have, when the LOD calculated for a particular part of the primitive is less than 1.0. In other words, the LOD of any part of the primitive is clamped at the lower end to max(m, LOD).

f specifies a fraction that the programmer can specify for use in the color combiner of the RDP. It's meant to offer a further refinement (XXX verify that, not 100% sure on it) on the linear filtering of two mipmaps.

FB: G_SETENVCOLOR

Sets the environment color for combiner

FB 00 00 00 [RR] [GG] [BB] [AA]

R	red value
G	green value
B	blue value
A	alpha value
Example: make everything loaded underneath the command red, and semi-transparent

FB 00 00 00 FF 00 00 8C
FC: G_SETCOMBINE

Performs combining operations (ex. multi-texturing). The N64's Color Combiner calculates the output color by combining the various source colors and source alphas using the following equation: outputcolor = ( A - B ) * C + D.

FC [XX XX XX XX XX XX XX]

X	Equation for blending. Mask is bitwise.
Bitwise mask for X:

[aaaa] [ccccc] [eee] [ggg] [iiii] [kkkkk] [bbbb] [jjjj] [mmm] [ooo] [ddd] [fff] [hhh] [lll] [nnn] [ppp]

a	variable 'a' for mode 1 (color)
b	variable 'b' for mode 1 (color)
c	variable 'c' for mode 1 (color)
d	variable 'd' for mode 1 (color)
e	variable 'e' for mode 1 (alpha)
f	variable 'f' for mode 1 (alpha)
g	variable 'g' for mode 1 (alpha)
h	variable 'h' for mode 1 (alpha)
i	variable 'a' for mode 2 (color)
j	variable 'b' for mode 2 (color)
k	variable 'c' for mode 2 (color)
l	variable 'd' for mode 2 (color)
m	variable 'e' for mode 2 (alpha)
n	variable 'f' for mode 2 (alpha)
o	variable 'g' for mode 2 (alpha)
p	variable 'h' for mode 2 (alpha)
More information here:

N64's Color Combiner

Examples:

FC 12 7F FF FF FF F8 38 : Standard usage for solid RGBA textures
FC 12 18 24 FF 33 FF FF : Standard usage for alpha RGBA textures
FD: G_SETTIMG

Sets the texture image offset

FD [xx] 00 00 [bb bb bb bb]

[xx] = fffi i000

b	Segmented address of texture
f	Texture format
i	Texture bit-size
Color formats
0	RGBA	Color and alpha
1	YUV	Luminance and Chrominance
2	CI	Index and look-up palette
3	IA	Grayscale and alpha
4	I	Grayscale
Bit sizes
0	4-bit	I, IA, and CI
1	8-bit	I, IA, and CI
2	16-bit	RGBA, IA, and YUV
3	32-bit	RGBA
Or simply Bitsize = 4*2^(i)

Example: Load texture position from 0x90 in RAM bank 0x04 for RGBA16

FD 10 00 00 04 00 00 90
FE: G_SETZIMG

Sets the location of the depth buffer

FE 00 00 00 [ii ii ii ii]

i	Segmented address of the depth buffer
Since the format of the depth buffer is fixed (14-bit Z value and 2-bit dZ value), those parameters are not here, unlike for FD and FF. The width of the depth buffer is shared with the color buffer (see FF), and thus isn't set here.

FF: G_SETCIMG

Sets the location of the color framebuffer, usually a framebuffer that's displayed to the screen.

FF [xx] 0[w ww] [ii ii ii ii]

xx = fffs s000 (binary)

w	Width of the color buffer (-1)
i	RAM address of color buffer
f	Format of color buffer to be pointed to
s	Bit size of pixels in color buffer to be pointed to
The color buffer is set to RAM location i, with a width of w. f and s are the typical image format enumerations, described in G_Settile.

The location is usually some place set as the framebuffer to show (which is not handled by display lists), but oftentimes will be set to a depth buffer. This is usually done to fill (aka clear) the depth buffer in fill mode.

