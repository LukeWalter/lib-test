# 1 "gba.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "gba.c"
# 1 "gba.h" 1




typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
# 67 "gba.h"
extern volatile unsigned short *videoBuffer;
# 88 "gba.h"
typedef struct {
 u16 tilemap[1024];

} SB;



typedef struct {
 u16 tileimg[8192];

} CB;





void setPixel3(int col, int row, unsigned short color);
void drawRect3(int col, int row, int width, int height, volatile unsigned short color);
void fillScreen3(volatile unsigned short color);
void drawImage3(int col, int row, int width, int height, const unsigned short *image);
void drawFullscreenImage3(const unsigned short *image);


void setPixel4(int col, int row, unsigned char colorIndex);
void drawRect4(int col, int row, int width, int height, volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char colorIndex);
void drawImage4(int col, int row, int width, int height, const unsigned short *image);
void drawFullscreenImage4(const unsigned short *image);


void waitUntilVBlank();
void flipPage();





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



extern OBJ_ATTR shadowOAM[];
extern AFFINE_MATRIX* affine;
# 159 "gba.h"
enum MODE { REGULAR, AFFINE, HIDE, DOUBLEAFFINE };






enum SHAPE { SQUARE, WIDE, TALL };
# 175 "gba.h"
enum SIZE { TINY, SMALL, MEDIUM, LARGE };
# 194 "gba.h"
void hideSprites();
void updateOAM();


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
# 235 "gba.h"
extern unsigned short oldButtons;
extern unsigned short buttons;
# 247 "gba.h"
typedef volatile struct {

    volatile const void* src;
    volatile void* dst;
    volatile unsigned int cnt;

} DMA;


extern DMA* dma;
# 289 "gba.h"
void DMANow(int channel, volatile const void* src, volatile void* dst, unsigned int cnt);
# 325 "gba.h"
typedef void (*ihp)(void);
# 345 "gba.h"
int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB);
# 2 "gba.c" 2


unsigned volatile short *videoBuffer = (unsigned short *)0x6000000;


DMA *dma = (DMA *)0x40000B0;

OBJ_ATTR shadowOAM[128];
AFFINE_MATRIX* affine = (AFFINE_MATRIX*) shadowOAM;


void setPixel3(int col, int row, unsigned short color) {
    videoBuffer[((row) * (240) + (col))] = color;

}


void setPixel4(int col, int row, unsigned char colorIndex) {

    volatile unsigned short pixelData = videoBuffer[((row) * (240) + (col)) / 2];

    if (col & 1) {
        pixelData &= 0x00FF;
        pixelData |= colorIndex << 8;

    } else {
        pixelData &= 0xFF00;
        pixelData |= colorIndex;

    }

    videoBuffer[((row) * (240) + (col)) / 2] = pixelData;

}


void drawRect3(int col, int row, int width, int height, volatile unsigned short color) {

    for (int r = 0; r < height; r++) {
        DMANow(3, &color, &videoBuffer[((row + r) * (240) + (col))], (2 << 23) | width);

    }

}


void drawRect4(int col, int row, int width, int height, volatile unsigned char colorIndex) {

    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);

    for (int r = 0; r < height; r++) {


        if (width == 1) {
            setPixel4(col, row + r, colorIndex);

        } else if (width == 2) {
            setPixel4(col, row + r, colorIndex);
            setPixel4(col + 1, row + r, colorIndex);

        } else if ((col & 1) && (width & 1)) {
            setPixel4(col, row + r, colorIndex);
            DMANow(3, &pixelData, &videoBuffer[((row + r) * (240) + (col + 1)) / 2], (2 << 23) | (width - 1) / 2);

        } else if (width & 1) {
            DMANow(3, &pixelData, &videoBuffer[((row + r) * (240) + (col)) / 2], (2 << 23) | (width - 1) / 2);
            setPixel4(col + width - 1, row + r, colorIndex);

        } else if (col & 1) {
            setPixel4(col, row + r, colorIndex);
            DMANow(3, &pixelData, &videoBuffer[((row + r) * (240) + (col + 1)) / 2], (2 << 23) | (width - 2) / 2);
            setPixel4(col + width - 1, row + r, colorIndex);

        } else {
            DMANow(3, &pixelData, &videoBuffer[((row + r) * (240) + (col)) / 2], (2 << 23) | width / 2);

        }

    }

}


void fillScreen3(volatile unsigned short color) {
    unsigned short c = color;
    DMANow(3, &c, videoBuffer, (2 << 23) | (240 * 160));

}


void fillScreen4(volatile unsigned char colorIndex) {
    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);
    DMANow(3, &pixelData, videoBuffer, (2 << 23) | (240 * 160) / 2);

}


void drawImage3(int col, int row, int width, int height, const unsigned short *image) {

    for (int r = 0; r < height; r++) {
        DMANow(3, &image[((r) * (width) + (0))], &videoBuffer[((row + r) * (240) + (col))], width);

    }

}


void drawImage4(int col, int row, int width, int height, const unsigned short *image) {

    for (int i = 0; i < height; i++) {
        DMANow(3, &image[((i) * (width / 2) + (0))], &videoBuffer[((row + i) * (240) + (col)) / 2], width / 2);

    }

}


void drawFullscreenImage3(const unsigned short *image) {
    DMANow(3, image, videoBuffer, 240 * 160);

}


void drawFullscreenImage4(const unsigned short *image) {
    DMANow(3, image, videoBuffer, 240 * 160 / 2);

}


void waitUntilVBlank() {
    while ((*(volatile unsigned short*) 0x4000006) > 160);
    while ((*(volatile unsigned short*) 0x4000006) < 160);

}


void flipPage() {

    if ((*(volatile unsigned short*) 0x4000000) & (1 << 4)) {
        videoBuffer = ((unsigned short*) 0x600A000);

    } else {
        videoBuffer = ((unsigned short*) 0x6000000);

    }

    (*(volatile unsigned short*) 0x4000000) ^= (1 << 4);

}


void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt) {


    dma[channel].cnt = 0;


    dma[channel].src = src;
    dma[channel].dst = dst;


    dma[channel].cnt = cnt | (1 << 31);

}


int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB) {
    return rowA < rowB + heightB - 1 && rowA + heightA - 1 > rowB && colA < colB + widthB - 1 && colA + widthA - 1 > colB;

}


void hideSprites() {

    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = (((HIDE) & 3) << 8);

    }

}


void updateOAM() {
    DMANow(3, shadowOAM, ((OBJ_ATTR*) 0x7000000), 512);

}
