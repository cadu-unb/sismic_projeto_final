#include <msp430.h>
#include <stdint.h>

/*
 * Take 'A' as example.
 * 'A' use 5 byte to denote:
 *     0x7C, 0x12, 0x11, 0x12, 0x7C
 *
 * and we represent it in base 2:
 *     0x7C: 01111100
 *     0x12: 00010010
 *     0x11: 00010001
 *     0x12: 00010010
 *     0x7C: 01111100
 * where 1 is font color, and 0 is background color
 *
 * So it's 'A' if we look it in counter-clockwise for 90 degree.
 * In general case, we also add a background line to seperate from other character:
 *     0x7C: 01111100
 *     0x12: 00010010
 *     0x11: 00010001
 *     0x12: 00010010
 *     0x7C: 01111100
 *     0x00: 00000000
 *
 **/

// standard ascii 5x7 font
const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},  // 0x00 (nul)
    {0x3E, 0x5B, 0x4F, 0x5B, 0x3E},  // 0x01 (soh)
    {0x3E, 0x6B, 0x4F, 0x6B, 0x3E},  // 0x02 (stx)
    {0x1C, 0x3E, 0x7C, 0x3E, 0x1C},  // 0x03 (etx)
    {0x18, 0x3C, 0x7E, 0x3C, 0x18},  // 0x04 (eot)
    {0x1C, 0x57, 0x7D, 0x57, 0x1C},  // 0x05 (enq)
    {0x1C, 0x5E, 0x7F, 0x5E, 0x1C},  // 0x06 (ack)
    {0x00, 0x18, 0x3C, 0x18, 0x00},  // 0x07 (bel)
    {0xFF, 0xE7, 0xC3, 0xE7, 0xFF},  // 0x08 (bs)
    {0x00, 0x18, 0x24, 0x18, 0x00},  // 0x09 (tab)
    {0xFF, 0xE7, 0xDB, 0xE7, 0xFF},  // 0x0A (lf)
    {0x30, 0x48, 0x3A, 0x06, 0x0E},  // 0x0B (vt)
    {0x26, 0x29, 0x79, 0x29, 0x26},  // 0x0C (np)
    {0x40, 0x7F, 0x05, 0x05, 0x07},  // 0x0D (cr)
    {0x40, 0x7F, 0x05, 0x25, 0x3F},  // 0x0E (so)
    {0x5A, 0x3C, 0xE7, 0x3C, 0x5A},  // 0x0F (si)
    {0x7F, 0x3E, 0x1C, 0x1C, 0x08},  // 0x10 (dle)
    {0x08, 0x1C, 0x1C, 0x3E, 0x7F},  // 0x11 (dc1)
    {0x14, 0x22, 0x7F, 0x22, 0x14},  // 0x12 (dc2)
    {0x5F, 0x5F, 0x00, 0x5F, 0x5F},  // 0x13 (dc3)
    {0x06, 0x09, 0x7F, 0x01, 0x7F},  // 0x14 (dc4)
    {0x00, 0x66, 0x89, 0x95, 0x6A},  // 0x15 (nak)
    {0x60, 0x60, 0x60, 0x60, 0x60},  // 0x16 (syn)
    {0x94, 0xA2, 0xFF, 0xA2, 0x94},  // 0x17 (etb)
    {0x08, 0x04, 0x7E, 0x04, 0x08},  // 0x18 (can)
    {0x10, 0x20, 0x7E, 0x20, 0x10},  // 0x19 (em)
    {0x08, 0x08, 0x2A, 0x1C, 0x08},  // 0x1A (eof)
    {0x08, 0x1C, 0x2A, 0x08, 0x08},  // 0x1B (esc)
    {0x1E, 0x10, 0x10, 0x10, 0x10},  // 0x1C (fs)
    {0x0C, 0x1E, 0x0C, 0x1E, 0x0C},  // 0x1D (gs)
    {0x30, 0x38, 0x3E, 0x38, 0x30},  // 0x1E (rs)
    {0x06, 0x0E, 0x3E, 0x0E, 0x06},  // 0x1F (us)
    {0x00, 0x00, 0x00, 0x00, 0x00},  // 0x20
    {0x00, 0x00, 0x5F, 0x00, 0x00},  // 0x21 !
    {0x00, 0x07, 0x00, 0x07, 0x00},  // 0x22 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14},  // 0x23 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},  // 0x24 $
    {0x23, 0x13, 0x08, 0x64, 0x62},  // 0x25 %
    {0x36, 0x49, 0x56, 0x20, 0x50},  // 0x26 &
    {0x00, 0x08, 0x07, 0x03, 0x00},  // 0x27 '
    {0x00, 0x1C, 0x22, 0x41, 0x00},  // 0x28 (
    {0x00, 0x41, 0x22, 0x1C, 0x00},  // 0x29 )
    {0x2A, 0x1C, 0x7F, 0x1C, 0x2A},  // 0x2A *
    {0x08, 0x08, 0x3E, 0x08, 0x08},  // 0x2B +
    {0x00, 0x80, 0x70, 0x30, 0x00},  // 0x2C ,
    {0x08, 0x08, 0x08, 0x08, 0x08},  // 0x2D -
    {0x00, 0x00, 0x60, 0x60, 0x00},  // 0x2E .
    {0x20, 0x10, 0x08, 0x04, 0x02},  // 0x2F /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0x30 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},  // 0x31 1
    {0x72, 0x49, 0x49, 0x49, 0x46},  // 0x32 2
    {0x21, 0x41, 0x49, 0x4D, 0x33},  // 0x33 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},  // 0x34 4
    {0x27, 0x45, 0x45, 0x45, 0x39},  // 0x35 5
    {0x3C, 0x4A, 0x49, 0x49, 0x31},  // 0x36 6
    {0x41, 0x21, 0x11, 0x09, 0x07},  // 0x37 7
    {0x36, 0x49, 0x49, 0x49, 0x36},  // 0x38 8
    {0x46, 0x49, 0x49, 0x29, 0x1E},  // 0x39 9
    {0x00, 0x00, 0x14, 0x00, 0x00},  // 0x3A :
    {0x00, 0x40, 0x34, 0x00, 0x00},  // 0x3B ;
    {0x00, 0x08, 0x14, 0x22, 0x41},  // 0x3C <
    {0x14, 0x14, 0x14, 0x14, 0x14},  // 0x3D =
    {0x00, 0x41, 0x22, 0x14, 0x08},  // 0x3E >
    {0x02, 0x01, 0x59, 0x09, 0x06},  // 0x3F ?
    {0x3E, 0x41, 0x5D, 0x59, 0x4E},  // 0x40 @
    {0x7C, 0x12, 0x11, 0x12, 0x7C},  // 0x41 A
    {0x7F, 0x49, 0x49, 0x49, 0x36},  // 0x42 B
    {0x3E, 0x41, 0x41, 0x41, 0x22},  // 0x43 C
    {0x7F, 0x41, 0x41, 0x41, 0x3E},  // 0x44 D
    {0x7F, 0x49, 0x49, 0x49, 0x41},  // 0x45 E
    {0x7F, 0x09, 0x09, 0x09, 0x01},  // 0x46 F
    {0x3E, 0x41, 0x41, 0x51, 0x73},  // 0x47 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},  // 0x48 H
    {0x00, 0x41, 0x7F, 0x41, 0x00},  // 0x49 I
    {0x20, 0x40, 0x41, 0x3F, 0x01},  // 0x4A J
    {0x7F, 0x08, 0x14, 0x22, 0x41},  // 0x4B K
    {0x7F, 0x40, 0x40, 0x40, 0x40},  // 0x4C L
    {0x7F, 0x02, 0x1C, 0x02, 0x7F},  // 0x4D M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},  // 0x4E N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},  // 0x4F O
    {0x7F, 0x09, 0x09, 0x09, 0x06},  // 0x50 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},  // 0x51 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},  // 0x52 R
    {0x26, 0x49, 0x49, 0x49, 0x32},  // 0x53 S
    {0x03, 0x01, 0x7F, 0x01, 0x03},  // 0x54 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},  // 0x55 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},  // 0x56 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F},  // 0x57 W
    {0x63, 0x14, 0x08, 0x14, 0x63},  // 0x58 X
    {0x03, 0x04, 0x78, 0x04, 0x03},  // 0x59 Y
    {0x61, 0x59, 0x49, 0x4D, 0x43},  // 0x5A Z
    {0x00, 0x7F, 0x41, 0x41, 0x41},  // 0x5B [
    {0x02, 0x04, 0x08, 0x10, 0x20},  // 0x5C backslash
    {0x00, 0x41, 0x41, 0x41, 0x7F},  // 0x5D ]
    {0x04, 0x02, 0x01, 0x02, 0x04},  // 0x5E ^
    {0x40, 0x40, 0x40, 0x40, 0x40},  // 0x5F _
    {0x00, 0x03, 0x07, 0x08, 0x00},  // 0x60 `
    {0x20, 0x54, 0x54, 0x78, 0x40},  // 0x61 a
    {0x7F, 0x28, 0x44, 0x44, 0x38},  // 0x62 b
    {0x38, 0x44, 0x44, 0x44, 0x28},  // 0x63 c
    {0x38, 0x44, 0x44, 0x28, 0x7F},  // 0x64 d
    {0x38, 0x54, 0x54, 0x54, 0x18},  // 0x65 e
    {0x00, 0x08, 0x7E, 0x09, 0x02},  // 0x66 f
    {0x18, 0xA4, 0xA4, 0x9C, 0x78},  // 0x67 g
    {0x7F, 0x08, 0x04, 0x04, 0x78},  // 0x68 h
    {0x00, 0x44, 0x7D, 0x40, 0x00},  // 0x69 i
    {0x20, 0x40, 0x40, 0x3D, 0x00},  // 0x6A j
    {0x7F, 0x10, 0x28, 0x44, 0x00},  // 0x6B k
    {0x00, 0x41, 0x7F, 0x40, 0x00},  // 0x6C l
    {0x7C, 0x04, 0x78, 0x04, 0x78},  // 0x6D m
    {0x7C, 0x08, 0x04, 0x04, 0x78},  // 0x6E n
    {0x38, 0x44, 0x44, 0x44, 0x38},  // 0x6F o
    {0xFC, 0x18, 0x24, 0x24, 0x18},  // 0x70 p
    {0x18, 0x24, 0x24, 0x18, 0xFC},  // 0x71 q
    {0x7C, 0x08, 0x04, 0x04, 0x08},  // 0x72 r
    {0x48, 0x54, 0x54, 0x54, 0x24},  // 0x73 s
    {0x04, 0x04, 0x3F, 0x44, 0x24},  // 0x74 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},  // 0x75 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},  // 0x76 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},  // 0x77 w
    {0x44, 0x28, 0x10, 0x28, 0x44},  // 0x78 x
    {0x4C, 0x90, 0x90, 0x90, 0x7C},  // 0x79 y
    {0x44, 0x64, 0x54, 0x4C, 0x44},  // 0x7A z
    {0x00, 0x08, 0x36, 0x41, 0x00},  // 0x7B {
    {0x00, 0x00, 0x77, 0x00, 0x00},  // 0x7C |
    {0x00, 0x41, 0x36, 0x08, 0x00},  // 0x7D }
    {0x02, 0x01, 0x02, 0x04, 0x02},  // 0x7E ~
    {0x3C, 0x26, 0x23, 0x26, 0x3C},  // 0x7F
    {0x1E, 0xA1, 0xA1, 0x61, 0x12},  // 0x80
    {0x3A, 0x40, 0x40, 0x20, 0x7A},  // 0x81
    {0x38, 0x54, 0x54, 0x55, 0x59},  // 0x82
    {0x21, 0x55, 0x55, 0x79, 0x41},  // 0x83
    {0x22, 0x54, 0x54, 0x78, 0x42},  // 0x84
    {0x21, 0x55, 0x54, 0x78, 0x40},  // 0x85
    {0x20, 0x54, 0x55, 0x79, 0x40},  // 0x86
    {0x0C, 0x1E, 0x52, 0x72, 0x12},  // 0x87
    {0x39, 0x55, 0x55, 0x55, 0x59},  // 0x88
    {0x39, 0x54, 0x54, 0x54, 0x59},  // 0x89
    {0x39, 0x55, 0x54, 0x54, 0x58},  // 0x8A
    {0x00, 0x00, 0x45, 0x7C, 0x41},  // 0x8B
    {0x00, 0x02, 0x45, 0x7D, 0x42},  // 0x8C
    {0x00, 0x01, 0x45, 0x7C, 0x40},  // 0x8D
    {0x7D, 0x12, 0x11, 0x12, 0x7D},  // 0x8E
    {0xF0, 0x28, 0x25, 0x28, 0xF0},  // 0x8F
    {0x7C, 0x54, 0x55, 0x45, 0x00},  // 0x90
    {0x20, 0x54, 0x54, 0x7C, 0x54},  // 0x91
    {0x7C, 0x0A, 0x09, 0x7F, 0x49},  // 0x92
    {0x32, 0x49, 0x49, 0x49, 0x32},  // 0x93
    {0x3A, 0x44, 0x44, 0x44, 0x3A},  // 0x94
    {0x32, 0x4A, 0x48, 0x48, 0x30},  // 0x95
    {0x3A, 0x41, 0x41, 0x21, 0x7A},  // 0x96
    {0x3A, 0x42, 0x40, 0x20, 0x78},  // 0x97
    {0x00, 0x9D, 0xA0, 0xA0, 0x7D},  // 0x98
    {0x3D, 0x42, 0x42, 0x42, 0x3D},  // 0x99
    {0x3D, 0x40, 0x40, 0x40, 0x3D},  // 0x9A
    {0x3C, 0x24, 0xFF, 0x24, 0x24},  // 0x9B
    {0x48, 0x7E, 0x49, 0x43, 0x66},  // 0x9C
    {0x2B, 0x2F, 0xFC, 0x2F, 0x2B},  // 0x9D
    {0xFF, 0x09, 0x29, 0xF6, 0x20},  // 0x9E
    {0xC0, 0x88, 0x7E, 0x09, 0x03},  // 0x9F
    {0x20, 0x54, 0x54, 0x79, 0x41},  // 0xA0
    {0x00, 0x00, 0x44, 0x7D, 0x41},  // 0xA1
    {0x30, 0x48, 0x48, 0x4A, 0x32},  // 0xA2
    {0x38, 0x40, 0x40, 0x22, 0x7A},  // 0xA3
    {0x00, 0x7A, 0x0A, 0x0A, 0x72},  // 0xA4
    {0x7D, 0x0D, 0x19, 0x31, 0x7D},  // 0xA5
    {0x26, 0x29, 0x29, 0x2F, 0x28},  // 0xA6
    {0x26, 0x29, 0x29, 0x29, 0x26},  // 0xA7
    {0x30, 0x48, 0x4D, 0x40, 0x20},  // 0xA8
    {0x38, 0x08, 0x08, 0x08, 0x08},  // 0xA9
    {0x08, 0x08, 0x08, 0x08, 0x38},  // 0xAA
    {0x2F, 0x10, 0xC8, 0xAC, 0xBA},  // 0xAB
    {0x2F, 0x10, 0x28, 0x34, 0xFA},  // 0xAC
    {0x00, 0x00, 0x7B, 0x00, 0x00},  // 0xAD
    {0x08, 0x14, 0x2A, 0x14, 0x22},  // 0xAE
    {0x22, 0x14, 0x2A, 0x14, 0x08},  // 0xAF
    {0x55, 0x00, 0x55, 0x00, 0x55},  // 0xB0
    {0xAA, 0x55, 0xAA, 0x55, 0xAA},  // 0xB1
    {0xFF, 0x55, 0xFF, 0x55, 0xFF},  // 0xB2
    {0x00, 0x00, 0x00, 0xFF, 0x00},  // 0xB3
    {0x10, 0x10, 0x10, 0xFF, 0x00},  // 0xB4
    {0x14, 0x14, 0x14, 0xFF, 0x00},  // 0xB5
    {0x10, 0x10, 0xFF, 0x00, 0xFF},  // 0xB6
    {0x10, 0x10, 0xF0, 0x10, 0xF0},  // 0xB7
    {0x14, 0x14, 0x14, 0xFC, 0x00},  // 0xB8
    {0x14, 0x14, 0xF7, 0x00, 0xFF},  // 0xB9
    {0x00, 0x00, 0xFF, 0x00, 0xFF},  // 0xBA
    {0x14, 0x14, 0xF4, 0x04, 0xFC},  // 0xBB
    {0x14, 0x14, 0x17, 0x10, 0x1F},  // 0xBC
    {0x10, 0x10, 0x1F, 0x10, 0x1F},  // 0xBD
    {0x14, 0x14, 0x14, 0x1F, 0x00},  // 0xBE
    {0x10, 0x10, 0x10, 0xF0, 0x00},  // 0xBF
    {0x00, 0x00, 0x00, 0x1F, 0x10},  // 0xC0
    {0x10, 0x10, 0x10, 0x1F, 0x10},  // 0xC1
    {0x10, 0x10, 0x10, 0xF0, 0x10},  // 0xC2
    {0x00, 0x00, 0x00, 0xFF, 0x10},  // 0xC3
    {0x10, 0x10, 0x10, 0x10, 0x10},  // 0xC4
    {0x10, 0x10, 0x10, 0xFF, 0x10},  // 0xC5
    {0x00, 0x00, 0x00, 0xFF, 0x14},  // 0xC6
    {0x00, 0x00, 0xFF, 0x00, 0xFF},  // 0xC7
    {0x00, 0x00, 0x1F, 0x10, 0x17},  // 0xC8
    {0x00, 0x00, 0xFC, 0x04, 0xF4},  // 0xC9
    {0x14, 0x14, 0x17, 0x10, 0x17},  // 0xCA
    {0x14, 0x14, 0xF4, 0x04, 0xF4},  // 0xCB
    {0x00, 0x00, 0xFF, 0x00, 0xF7},  // 0xCC
    {0x14, 0x14, 0x14, 0x14, 0x14},  // 0xCD
    {0x14, 0x14, 0xF7, 0x00, 0xF7},  // 0xCE
    {0x14, 0x14, 0x14, 0x17, 0x14},  // 0xCF
    {0x10, 0x10, 0x1F, 0x10, 0x1F},  // 0xD0
    {0x14, 0x14, 0x14, 0xF4, 0x14},  // 0xD1
    {0x10, 0x10, 0xF0, 0x10, 0xF0},  // 0xD2
    {0x00, 0x00, 0x1F, 0x10, 0x1F},  // 0xD3
    {0x00, 0x00, 0x00, 0x1F, 0x14},  // 0xD4
    {0x00, 0x00, 0x00, 0xFC, 0x14},  // 0xD5
    {0x00, 0x00, 0xF0, 0x10, 0xF0},  // 0xD6
    {0x10, 0x10, 0xFF, 0x10, 0xFF},  // 0xD7
    {0x14, 0x14, 0x14, 0xFF, 0x14},  // 0xD8
    {0x10, 0x10, 0x10, 0x1F, 0x00},  // 0xD9
    {0x00, 0x00, 0x00, 0xF0, 0x10},  // 0xDA
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  // 0xDB
    {0xF0, 0xF0, 0xF0, 0xF0, 0xF0},  // 0xDC
    {0xFF, 0xFF, 0xFF, 0x00, 0x00},  // 0xDD
    {0x00, 0x00, 0x00, 0xFF, 0xFF},  // 0xDE
    {0x0F, 0x0F, 0x0F, 0x0F, 0x0F},  // 0xDF
    {0x38, 0x44, 0x44, 0x38, 0x44},  // 0xE0
    {0xFC, 0x4A, 0x4A, 0x4A, 0x34},  // 0xE1
    {0x7E, 0x02, 0x02, 0x06, 0x06},  // 0xE2
    {0x02, 0x7E, 0x02, 0x7E, 0x02},  // 0xE3
    {0x63, 0x55, 0x49, 0x41, 0x63},  // 0xE4
    {0x38, 0x44, 0x44, 0x3C, 0x04},  // 0xE5
    {0x40, 0x7E, 0x20, 0x1E, 0x20},  // 0xE6
    {0x06, 0x02, 0x7E, 0x02, 0x02},  // 0xE7
    {0x99, 0xA5, 0xE7, 0xA5, 0x99},  // 0xE8
    {0x1C, 0x2A, 0x49, 0x2A, 0x1C},  // 0xE9
    {0x4C, 0x72, 0x01, 0x72, 0x4C},  // 0xEA
    {0x30, 0x4A, 0x4D, 0x4D, 0x30},  // 0xEB
    {0x30, 0x48, 0x78, 0x48, 0x30},  // 0xEC
    {0xBC, 0x62, 0x5A, 0x46, 0x3D},  // 0xED
    {0x3E, 0x49, 0x49, 0x49, 0x00},  // 0xEE
    {0x7E, 0x01, 0x01, 0x01, 0x7E},  // 0xEF
    {0x2A, 0x2A, 0x2A, 0x2A, 0x2A},  // 0xF0
    {0x44, 0x44, 0x5F, 0x44, 0x44},  // 0xF1
    {0x40, 0x51, 0x4A, 0x44, 0x40},  // 0xF2
    {0x40, 0x44, 0x4A, 0x51, 0x40},  // 0xF3
    {0x00, 0x00, 0xFF, 0x01, 0x03},  // 0xF4
    {0xE0, 0x80, 0xFF, 0x00, 0x00},  // 0xF5
    {0x08, 0x08, 0x6B, 0x6B, 0x08},  // 0xF6
    {0x36, 0x12, 0x36, 0x24, 0x36},  // 0xF7
    {0x06, 0x0F, 0x09, 0x0F, 0x06},  // 0xF8
    {0x00, 0x00, 0x18, 0x18, 0x00},  // 0xF9
    {0x00, 0x00, 0x10, 0x10, 0x00},  // 0xFA
    {0x30, 0x40, 0xFF, 0x01, 0x01},  // 0xFB
    {0x00, 0x1F, 0x01, 0x01, 0x1E},  // 0xFC
    {0x00, 0x19, 0x1D, 0x17, 0x12},  // 0xFD
    {0x00, 0x3C, 0x3C, 0x3C, 0x3C},  // 0xFE
    {0x00, 0x00, 0x00, 0x00, 0x00}   // 0xFF
};

const uint32_t font20x28[][20] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x0, 0x0, 0x70, 0x3f0, 0x1fe0, 0xff00, 0x7fe00, 0x3fce00, 0x1fe0e00, 0x7e00e00, 0x7e00e00, 0x1fe0e00, 0x3fce00, 0x7fe00, 0xff00, 0x1fe0, 0x3f0, 0x70, 0x0, 0x0},                                     // a 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0xfe3f80, 0xfe3f80, 0xfe3f80, 0x0, 0x0},    // b 
    {0x0, 0x0, 0xffff80, 0xffff80, 0xffff80, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x0, 0x0},    // c 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0xffff80, 0xffff80, 0xffff80, 0x0, 0x0},    // d 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x0, 0x0}, // e 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x0, 0x0}, // f 
    {0x0, 0x0, 0xffff80, 0xffff80, 0xffff80, 0x7000070, 0x7000070, 0x7000070, 0x700e070, 0x700e070, 0x700e070, 0x700e070, 0x700e070, 0x700e070, 0x700e070, 0x700ff80, 0x700ff80, 0x700ff80, 0x0, 0x0},    // g 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0},                               // h 
    {0x0, 0x0, 0x0, 0x6000030, 0x6000030, 0x6000030, 0x6000030, 0x6000030, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x6000030, 0x6000030, 0x6000030, 0x6000030, 0x6000030, 0x0, 0x0, 0x0},             // i 
    {0x0, 0x0, 0x3f80, 0x3f80, 0x3f80, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x7ffff80, 0x7ffff80, 0x7ffff80, 0x0, 0x0},                                                            // j 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x1c000, 0x1c000, 0x1c000, 0xe3800, 0xe3800, 0xe3800, 0x700700, 0x700700, 0x700700, 0x78000f0, 0x78000f0, 0x78000f0, 0x0, 0x0, 0x0},                      // k 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x0, 0x0},                                                                  // l 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x7800000, 0x1e00000, 0x780000, 0x1e0000, 0x38000, 0x38000, 0x1e0000, 0x780000, 0x1e00000, 0x7800000, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0},         // m 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x3c00000, 0x1f00000, 0x7c0000, 0x1f0000, 0x7c000, 0x1f000, 0x7c00, 0x1f00, 0x7c0, 0x1e0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0},                     // n 
    {0x0, 0x0, 0xffff80, 0xffff80, 0xffff80, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0x7000070, 0xffff80, 0xffff80, 0xffff80, 0x0, 0x0},       // o 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0x701c000, 0xfe0000, 0xfe0000, 0xfe0000, 0x0, 0x0},    // p 
    {0x0, 0x0, 0xffff80, 0xffff80, 0xffff80, 0x70003f0, 0x70003f0, 0x70003f0, 0x7001c70, 0x7001c70, 0x7001c70, 0x70003f0, 0x70003f0, 0x7000380, 0x700007e, 0xffff7e, 0xffff7e, 0xffff00, 0x0, 0x0},       // q 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c000, 0x701e000, 0x701e000, 0x701e000, 0x701dc00, 0x701dc00, 0x701dc00, 0x701c380, 0x701c380, 0x701c380, 0xfe0070, 0xfe0070, 0xfe0070, 0x0, 0x0},    // r 
    {0x0, 0x0, 0xffc0f0, 0xffc0f0, 0xffc0f0, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x701c070, 0x781ff80, 0x781ff80, 0x781ff80, 0x0, 0x0},    // s 
    {0x0, 0x0, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x0, 0x0}, // t 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0},                                                   // u 
    {0x0, 0x0, 0x7e00000, 0x7fc0000, 0x7fc000, 0xff800, 0xff00, 0x1fc0, 0x3f0, 0x30, 0x30, 0x3e0, 0x1fc0, 0xff00, 0xff800, 0x7fc000, 0x7fc0000, 0x7e00000, 0x0, 0x0},                                     // v 
    {0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0xf0, 0x3c0, 0xf00, 0x3c00, 0xe000, 0xe000, 0x3c00, 0xf00, 0x3c0, 0xf0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0},                                       // w 
    {0x0, 0x0, 0x7f007f0, 0x7f007f0, 0x7f007f0, 0xe3800, 0xe3800, 0xe3800, 0x1c000, 0x1c000, 0x1c000, 0x1c000, 0xe3800, 0xe3800, 0xe3800, 0x7f007f0, 0x7f007f0, 0x7f007f0, 0x0, 0x0},                     // x 
    {0x0, 0x0, 0x7e00000, 0x7e00000, 0x7e00000, 0x1e0000, 0x1e0000, 0x1e0000, 0x1fff0, 0x1fff0, 0x1fff0, 0x1fff0, 0x1e0000, 0x1e0000, 0x1e0000, 0x7e00000, 0x7e00000, 0x7e00000, 0x0, 0x0},               // y 
    {0x0, 0x0, 0x70007f0, 0x7000ff0, 0x7001ff0, 0x7003e70, 0x7007c70, 0x700f870, 0x701f070, 0x703e070, 0x707c070, 0x70f8070, 0x71f0070, 0x73e0070, 0x77c0070, 0x7f80070, 0x7f00070, 0x7e00070, 0x0, 0x0}, // z 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
    {0x0, 0x0, 0x0, 0x780, 0x780, 0x780, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x3ff80, 0x3ff80, 0x3ff80, 0x0, 0x0, 0x0},                                       // a 
    {0x0, 0x0, 0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x3f80, 0x3f80, 0x3f80, 0x0, 0x0, 0x0, 0x0},                                              // b 
    {0x0, 0x0, 0x0, 0x1ff80, 0x1ff80, 0x1ff80, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0xe0070, 0x0, 0x0, 0x0, 0x0},                                             // c 
    {0x0, 0x0, 0x0, 0x0, 0x3f80, 0x3f80, 0x3f80, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x1c070, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x0, 0x0, 0x0, 0x0},                                              // d 
    {0x0, 0x0, 0x0, 0x3ff80, 0x3ff80, 0x3ff80, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x1c3870, 0x3c000, 0x3c000, 0x3c000, 0x0, 0x0, 0x0},                                 // e 
    {0x0, 0x0, 0x0, 0x1c000, 0x1c000, 0x1c000, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x701c000, 0x701c000, 0x701c000, 0x7000000, 0x7000000, 0x7000000, 0x7000000, 0x0, 0x0, 0x0, 0x0},                         // f 
    {0x0, 0x0, 0x0, 0x0, 0xfe000, 0xfe000, 0xfe000, 0x701c1c, 0x701c1c, 0x701c1c, 0x701c1c, 0x701c1c, 0x701c1c, 0xfffe0, 0xfffe0, 0xfffe0, 0x0, 0x0, 0x0, 0x0},                                           // g 
    {0x0, 0x0, 0x0, 0x0, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0x1ff0, 0x1ff0, 0x1ff0, 0x0, 0x0, 0x0, 0x0},                                                    // h 
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1cfff0, 0x1cfff0, 0x1cfff0, 0x1cfff0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},                                                                             // i 
    {0x0, 0x0, 0x0, 0x0, 0x70, 0x70, 0x70, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0x71fff0, 0x71fff0, 0x71fff0, 0x0, 0x0, 0x0, 0x0},                                                                               // j 
    {0x0, 0x0, 0x0, 0x0, 0xffff0, 0xffff0, 0xffff0, 0x1c00, 0x1c00, 0x1c00, 0xe380, 0xe380, 0xe380, 0x70070, 0x70070, 0x70070, 0x0, 0x0, 0x0, 0x0},                                                       // k 
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x3000030, 0x3000030, 0x3000030, 0x3fffff0, 0x3fffff0, 0x3fffff0, 0x3fffff0, 0x30, 0x30, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0},                                                    // l 
    {0x0, 0x0, 0x1fff0, 0x1fff0, 0x3fff0, 0xf0000, 0xe0000, 0xe0000, 0x30000, 0x1f800, 0x1f800, 0x30000, 0xe0000, 0xe0000, 0xf0000, 0x3fff0, 0x1fff0, 0x1fff0, 0x0, 0x0},                                 // m 
    {0x0, 0x0, 0x0, 0x0, 0x3fff0, 0x3fff0, 0x3fff0, 0x1c000, 0x1c000, 0x1c000, 0x1c000, 0x1c000, 0x1c000, 0x3ff0, 0x3ff0, 0x3ff0, 0x0, 0x0, 0x0, 0x0},                                                    // n 
    {0x0, 0x0, 0x0, 0xff80, 0xff80, 0xff80, 0x70070, 0x70070, 0x70070, 0x70070, 0x70070, 0x70070, 0x70070, 0x70070, 0xff80, 0xff80, 0xff80, 0x0, 0x0, 0x0},                                               // o 
    {0x0, 0x0, 0x0, 0x0, 0xfffff, 0xfffff, 0xfffff, 0x701c00, 0x701c00, 0x701c00, 0x701c00, 0x701c00, 0x701c00, 0xfe000, 0xfe000, 0xfe000, 0x0, 0x0, 0x0, 0x0},                                           // p 
    {0x0, 0x0, 0x0, 0x0, 0xfe000, 0xfe000, 0xfe000, 0x701c00, 0x701c00, 0x701c00, 0x701c00, 0x701c18, 0x701c18, 0xfffff, 0xfffff, 0xfffff, 0x18, 0x18, 0x0, 0x0},                                         // q 
    {0x0, 0x0, 0x0, 0x0, 0x1ffff0, 0x1ffff0, 0x1ffff0, 0x1c000, 0x1c000, 0x1c000, 0x1c000, 0xe0000, 0xe0000, 0xe0000, 0xe0000, 0xe0000, 0x0, 0x0, 0x0, 0x0},                                              // r 
    {0x0, 0x0, 0x0, 0x0, 0x1c070, 0x1c070, 0x1c070, 0xe3870, 0xe3870, 0xe3870, 0xe3870, 0xe3870, 0xe3870, 0xe3870, 0xe0780, 0xe0780, 0xe0780, 0x0, 0x0, 0x0},                                             // s 
    {0x0, 0x0, 0x0, 0x0, 0x1c0000, 0x1c0000, 0x1c0000, 0x7fffff0, 0x7fffff0, 0x7fffff0, 0x1c0070, 0x1c0070, 0x1c0070, 0x1c0070, 0x70, 0x0, 0x0, 0x0, 0x0, 0x0},                                           // t 
    {0x0, 0x0, 0x0, 0x0, 0xfff80, 0xfff80, 0xfff80, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xffff0, 0xffff0, 0xffff0, 0x0, 0x0, 0x0, 0x0},                                                                   // u 
    {0x0, 0x0, 0x0, 0x0, 0xfc000, 0xfff00, 0xfffc0, 0x3fc0, 0xf0, 0x30, 0x30, 0xf0, 0x3fc0, 0xfffc0, 0xfff00, 0xfc000, 0x0, 0x0, 0x0, 0x0},                                                               // v 
    {0x0, 0x0, 0xfff80, 0xfff80, 0xfffc0, 0xf0, 0x70, 0x70, 0xc0, 0x1f80, 0x1f80, 0xc0, 0x70, 0x70, 0xf0, 0xfffc0, 0xfff80, 0xfff80, 0x0, 0x0},                                                           // w 
    {0x0, 0x0, 0x0, 0x0, 0x1fc7f0, 0x1fc7f0, 0x1fc7f0, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x1fc7f0, 0x1fc7f0, 0x1fc7f0, 0x0, 0x0, 0x0, 0x0},                                                 // x 
    {0x0, 0x0, 0x0, 0x0, 0x7fff80, 0x7fff80, 0x7fff80, 0x38e, 0x38e, 0x38e, 0x38e, 0x38e, 0x38e, 0x7ffff0, 0x7ffff0, 0x7ffff0, 0x0, 0x0, 0x0, 0x0},                                                       // y 
    {0x0, 0x0, 0x0, 0xe0070, 0xe00f0, 0xe01f0, 0xe03f0, 0xe0770, 0xe0e70, 0xe1c70, 0xe7870, 0xe7070, 0xee070, 0xfc070, 0xfc070, 0xf0070, 0xe0070, 0x0, 0x0, 0x0}                                          // z 
};
