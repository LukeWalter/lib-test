#ifndef GBA_H
#define GBA_H

// Common Typedefs
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Common Macros
#define TRUE 1
#define FALSE 0
#define OFFSET(x, y, width) ((y) * (width) + (x))

// ================================= DISPLAY ==================================

// Display Control Register
#define REG_DISPCTL (*(volatile unsigned short*) 0x4000000)
#define MODE0 0
#define MODE3 3
#define MODE4 4
#define DISP_BACKBUFFER (1 << 4)
#define BG0_ENABLE (1<<8)
#define BG1_ENABLE (1<<9)
#define BG2_ENABLE (1<<10)
#define BG3_ENABLE (1<<11)
#define SPRITE_ENABLE (1<<12)
#define SPRITE_MODE_2D (0<<6)
#define SPRITE_MODE_1D (1<<6)

// Background Control Registers
#define REG_BG0CTL (*(volatile unsigned short*) 0x4000008)
#define REG_BG1CTL (*(volatile unsigned short*) 0x400000A)
#define REG_BG2CTL (*(volatile unsigned short*) 0x400000C)
#define REG_BG3CTL (*(volatile unsigned short*) 0x400000E)

#define BG_CHARBLOCK(num)   ((num) << 2)
#define BG_SCREENBLOCK(num) ((num) << 8)
#define BG_4BPP             (0<<7)
#define BG_8BPP             (1<<7)
#define BG_SIZE_SMALL       (0<<14)  // 32x32 tiles
#define BG_SIZE_WIDE        (1<<14)  // 64x32 tiles
#define BG_SIZE_TALL        (2<<14)  // 32x64 tiles
#define BG_SIZE_LARGE       (3<<14)  // 64x64 tiles

// Background Offset Registers
#define REG_BG0HOFF (*(volatile unsigned short*) 0x04000010)
#define REG_BG0VOFF (*(volatile unsigned short*) 0x04000012)

#define REG_BG1HOFF (*(volatile unsigned short*) 0x04000014)
#define REG_BG1VOFF (*(volatile unsigned short*) 0x04000016)

#define REG_BG2HOFF (*(volatile unsigned short*) 0x04000018)
#define REG_BG2VOFF (*(volatile unsigned short*) 0x0400001A)

#define REG_BG3HOFF (*(volatile unsigned short*) 0x0400001C)
#define REG_BG3VOFF (*(volatile unsigned short*) 0x0400001E)

// Display Status Registers
#define REG_VCOUNT (*(volatile unsigned short*) 0x4000006)

// Display Constants
#define SCREENHEIGHT 160
#define SCREENWIDTH  240

// Video Buffer
extern volatile unsigned short *videoBuffer;
#define FRONTBUFFER ((unsigned short*) 0x6000000)
#define BACKBUFFER ((unsigned short*) 0x600A000)

// Palettes
#define BG_PALETTE ((unsigned short*) 0x5000000)
#define SPRITE_PALETTE ((unsigned short*) 0x5000200)

// Color
#define RGB(R, G, B) (((R) & 31) | ((G) & 31) << 5 | ((B) & 31) << 10)
#define BLACK        RGB(0, 0, 0)
#define WHITE        RGB(31, 31, 31)
#define GRAY         RGB(15, 15, 15)
#define RED          RGB(31, 0, 0)
#define GREEN        RGB(0, 31, 0)
#define BLUE         RGB(0, 0, 31)
#define CYAN         RGB(0, 31, 31)
#define MAGENTA      RGB(31, 0, 31)
#define YELLOW       RGB(31, 31, 0)

// Character and Screen Blocks
typedef struct {
	u16 tilemap[1024];

} SB;

#define SCREENBLOCK ((SB*) 0x06000000)

typedef struct {
	u16 tileimg[8192];    

} CB;

#define CHARBLOCK ((CB*) 0x06000000)
#define SPRITEBLOCK ((CB*) 0x06010000)

// Mode 3 Drawing Functions
void setPixel3(int x, int y, unsigned short color);
void drawRect3(int x, int y, int width, int height, volatile unsigned short color);
void fillScreen3(volatile unsigned short color);
void drawImage3(int x, int y, int width, int height, const unsigned short *image);
void drawFullscreenImage3(const unsigned short *image);

// Mode 4 Drawing Functions
void setPixel4(int x, int y, unsigned char colorIndex);
void drawRect4(int x, int y, int width, int height, volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char colorIndex);
void drawImage4(int x, int y, int width, int height, const unsigned short *image);
void drawFullscreenImage4(const unsigned short *image);

// Miscellaneous Drawing Functions
void waitUntilVBlank();
void flipPage();


// ================================= SPRITES ==================================

// Sprite Attribute Struct
typedef struct {

    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 fill;
    
} OBJ_ATTR;

typedef struct {

    u16 fill0[3];
    u16 a;
    u16 fill1[3];
    u16 b;
    u16 fill2[3];
    u16 c;
    u16 fill3[3];
    u16 d;
    
} AFFINE_MATRIX;

// Object Attribute Memory
#define OAM ((OBJ_ATTR*) 0x7000000)
extern OBJ_ATTR shadowOAM[];
extern AFFINE_MATRIX* affine;

// Sprite Constants
#define ROWMASK 0xFF
#define COLMASK 0x1FF

// Attribute 0
#define ATTR0_ROW(row)     ((row) & ROWMASK) // Row
#define ATTR0_OM(mode)       (((mode) & 3) << 8) // Object Mode
enum MODE { REGULAR, AFFINE, HIDE, DOUBLEAFFINE }; // Object Mode Options
#define ATTR0_ALPHA        (1 << 10) // Enable Alpha Blending
#define ATTR0_WINDOW       (1 << 11) // Enable Object Window Mode
#define ATTR0_MOSAIC       (1 << 12) // Enable Mosaic Effect
#define ATTR0_4BPP         (0 << 13) // 4 Bits Per Pixel
#define ATTR0_8BPP         (1 << 13) // 8 Bits Per Pixel
#define ATTR0_SHAPE(shape) (((shape) & 3) << 14) // Shape
enum SHAPE { SQUARE, WIDE, TALL }; // Shape Options (See Sprite Dimension Chart)
                                                
// Attribute 1
#define ATTR1_COL(col)     ((col) & COLMASK) // Column
#define ATTR1_HFLIP        (1 << 12) // Horizontal Flip
#define ATTR1_VFLIP        (1 << 13) // Vertical Flip
#define ATTR1_AFFINE(matrix)  (((matrix) & 31) << 9)
#define ATTR1_SIZE(size)   (((size) & 3) << 14) // Size
enum SIZE { TINY, SMALL, MEDIUM, LARGE }; // Size Options (See Sprite Dimension Chart)

// Attribute 2
#define ATTR2_TILEID(col, row)  OFFSET(((col) & 31), ((row) & 31), 32)
#define ATTR2_PRIORITY(num)     (((num) & 3) << 10)
#define ATTR2_PALROW(row)       (((row) & 15) << 12)

// Sprite Dimension Chart
//        -------------------------------------
//        |  TINY  | SMALL  | MEDIUM | LARGE  |
// --------------------------------------------
// SQUARE |  8x8   | 16x16  | 32x32  | 64x64  |
// --------------------------------------------
//  WIDE  |  16x8  | 32x8   | 32x16  | 64x32  |
// --------------------------------------------
//  TALL  |  8x16  | 8x32   | 16x32  | 32x64  |
// --------------------------------------------

// Generic struct for animated sprite
typedef struct {

    int width;
    int height;
    int worldX;
    int dx;
    int worldY;
    int dy;
    int prevAniState;
    int aniState;
    int currFrame;
    int aniCounter;
    int numFrames;
    OBJ_ATTR* attributes;
    int hide;
    int affineMatrix;

} ANISPRITE;

// Sprite Functions
void hide(ANISPRITE* sprite);
void hideSprites();
void setAffineMatrix(int matrix, u16 a, u16 b, u16 c, u16 d);
void updateOAM();

// ========================== TILEMAP MODIFICATION ============================

#define TILEID(x, y) OFFSET((x), (y), 32)
#define TILEMAP(screenblock, x, y) SCREENBLOCK[(screenblock)].tilemap(TILEID((x), (y)))

// ================================== INPUT ===================================

// Button Register
#define REG_BUTTONS (*(volatile unsigned short*) 0x04000130)

// Button Masks
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT	(1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN	(1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

// Variables for Button Macros
extern unsigned short oldButtons;
extern unsigned short buttons;

// Button Macros
#define BUTTON_HELD(key) (~REG_BUTTONS & (key))
#define BUTTON_PRESSED(key) (!(~oldButtons & (key)) && (~REG_BUTTONS & (key)))
#define BUTTON_RELEASED(key) ((~oldButtons & (key)) && !(~REG_BUTTONS & (key)))


// =================================== DMA ====================================

// DMA Struct
typedef volatile struct {

    volatile const void* src;
    volatile void* dst;
    volatile unsigned int cnt;

} DMA;

// First DMA Register Address
extern DMA* dma;

// Destination Adjustment
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

// Source Adjustment
#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// VBlank and HBlank Repeats
#define DMA_REPEAT (1 << 25)

// Chunk Size
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Timing Mode
#define DMA_AT_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

// Interrupts
#define DMA_IENABLE (1 << 30)

// Begin DMA
#define DMA_ON (1 << 31)

// DMA Functions
void DMANow(int channel, volatile const void* src, volatile void* dst, unsigned int cnt);

// =================================== TIMERS ====================================

// Controllers
#define REG_TM0CTL (*(volatile unsigned short*) 0x4000102)
#define REG_TM1CTL (*(volatile unsigned short*) 0x4000106)
#define REG_TM2CTL (*(volatile unsigned short*) 0x400010A)
#define REG_TM3CTL (*(volatile unsigned short*) 0x400010E)

// Timer values
#define REG_TM0DATA (*(volatile unsigned short*) 0x4000100)
#define REG_TM1DATA (*(volatile unsigned short*) 0x4000104)
#define REG_TM2DATA (*(volatile unsigned short*) 0x4000108)
#define REG_TM3DATA (*(volatile unsigned short*) 0x400010C)

// Timer flags
#define TM_ON (1 << 7)
#define TM_OFF (0 << 7)
#define TM_IENABLE (1 << 6)
#define TM_CASCADE (1 << 2)
#define TM_FREQ_1 0
#define TM_FREQ_64 1
#define TM_FREQ_256 2
#define TM_FREQ_1024 3 

// =================================== INTERRUPTS ====================================

// Controller
#define REG_IMASTER (*(unsigned short*) 0x4000208)
// Enabler
#define REG_IENABLE (*(unsigned short*) 0x4000200)
// Flag
#define REG_IFLAG (*(volatile unsigned short*) 0x4000202)

// Interrupt handler function pointer
typedef void (*ihp)(void);
// Interrupt handler register
#define REG_IHANDLER (*(ihp*) 0x03007FFC)
// Display status register
#define REG_DISPSTAT (*(unsigned short*) 0x04000004)

// Interrupt constants for turning them on
#define VBLANK_IENABLE (1 << 3)

// Interrupt constants for checking which type of interrupt happened 
#define VBLANK_INTERRUPT (1 << 0)   
#define TM0_INTERRUPT (1 << 3)   
#define TM1_INTERRUPT (1 << 4)   
#define TM2_INTERRUPT (1 << 5)    
#define TM3_INTERRUPT (1 << 6)    
#define BUTTON_INTERRUPT (1 << 12)

// ============================== MISCELLANEOUS ===============================

// Collision function
int collision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

// Saving
#define GAMEPAK_ROM ((u8*)0x0E000000)

#endif