#include "gba.h"

// The start of the video memory
unsigned volatile short *videoBuffer = (unsigned short *)0x6000000;

// The start of DMA registers
DMA *dma = (DMA *)0x40000B0;

OBJ_ATTR shadowOAM[128];
AFFINE_MATRIX* affine = (AFFINE_MATRIX*) shadowOAM;

// Set a pixel on the screen in Mode 3
void setPixel3(int x, int y, unsigned short color) {
    videoBuffer[OFFSET(x, y, SCREENWIDTH)] = color;

} // setPixel3

// Set a pixel on the screen in Mode 4
void setPixel4(int x, int y, unsigned char colorIndex) {

    volatile unsigned short pixelData = videoBuffer[OFFSET(x, y, SCREENWIDTH) / 2];
    
    if (x & 1) {
        pixelData &= 0x00FF;
        pixelData |= colorIndex << 8;
    
    } else {
        pixelData &= 0xFF00;
        pixelData |= colorIndex;
    
    } // if
    
    videoBuffer[OFFSET(x, y, SCREENWIDTH) / 2] = pixelData;

} // setPixel4

// Draw a rectangle at the specified location and size in Mode 3
void drawRect3(int x, int y, int width, int height, volatile unsigned short color) {

    for (int h = 0; h < height; h++) {
        DMANow(3, &color, &videoBuffer[OFFSET(x, y + h, SCREENWIDTH)], DMA_SOURCE_FIXED | width);
        
    } // for

} // drawRect3

// Draw a rectangle at the specified location and size in Mode 4
void drawRect4(int x, int y, int width, int height, volatile unsigned char colorIndex) {

    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);
    
    for (int h = 0; h < height; h++) {

        // Ensure we don't DMA 0 copies
        if (width == 1) {
            setPixel4(x, y + h, colorIndex);

        } else if (width == 2) {
            setPixel4(x, y + h, colorIndex);
            setPixel4(x + 1, y + h, colorIndex);
        
        } else if ((x & 1) && (width & 1)) { // Odd width odd col
            setPixel4(x, y + h, colorIndex);
            DMANow(3, &pixelData, &videoBuffer[OFFSET(x + 1, y + h, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 1) / 2);
        
        } else if (width & 1) { // Even col odd width
            DMANow(3, &pixelData, &videoBuffer[OFFSET(x, y + h, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 1) / 2);
            setPixel4(x + width - 1, y + h, colorIndex);
            
        } else if (x & 1) { // Odd col even width
            setPixel4(x, y + h, colorIndex);
            DMANow(3, &pixelData, &videoBuffer[OFFSET(x + 1, y + h, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 2) / 2);
            setPixel4(x + width - 1, y + h, colorIndex);
        
        } else { // Even col even width
            DMANow(3, &pixelData, &videoBuffer[OFFSET(x, y + h, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | width / 2);
        
        } // if

    } // for

} // drawRect4

// Fill the entire screen with a single color in Mode 3
void fillScreen3(volatile unsigned short color) {
    unsigned short c = color;
    DMANow(3, &c, videoBuffer, DMA_SOURCE_FIXED | (SCREENWIDTH * SCREENHEIGHT));

} // fillScreen3

// Fill the entire screen with a single color in Mode 4
void fillScreen4(volatile unsigned char colorIndex) {
    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);
    DMANow(3, &pixelData, videoBuffer, DMA_SOURCE_FIXED | (SCREENWIDTH * SCREENHEIGHT) / 2);

} // fillScreen4

// Draw an image at the specified location and size in Mode 3
void drawImage3(int x, int y, int width, int height, const unsigned short *image) {

    for (int h = 0; h < height; h++) {
        DMANow(3, &image[OFFSET(0, h, width)], &videoBuffer[OFFSET(x, y + h, SCREENWIDTH)], width);
    
    } // for

} // drawImage3

// Draw an image at the specified location and size in Mode 4 (must be even col and width)
void drawImage4(int x, int y, int width, int height, const unsigned short *image) {
    
    for (int h = 0; h < height; h++) {
        DMANow(3, &image[OFFSET(0, h, width / 2)], &videoBuffer[OFFSET(x, y + h, SCREENWIDTH) / 2], width / 2);
    
    } // for

} // drawImage4

// Fill the entire screen with an image in Mode 3
void drawFullscreenImage3(const unsigned short *image) {
    DMANow(3, image, videoBuffer, SCREENWIDTH * SCREENHEIGHT);

} // drawFullscreenImage3

// Fill the entire screen with an image in Mode 4
void drawFullscreenImage4(const unsigned short *image) {
    DMANow(3, image, videoBuffer, SCREENWIDTH * SCREENHEIGHT / 2);

} // drawFullscreenImage4

// Pause code execution until vertical blank begins
void waitUntilVBlank() {
    while (REG_VCOUNT > 160);
    while (REG_VCOUNT < 160);

} // waitUntilVBlank

// Flips the page
void flipPage() {

    if (REG_DISPCTL & DISP_BACKBUFFER) {
        videoBuffer = BACKBUFFER;
    
    } else {
        videoBuffer = FRONTBUFFER;
    
    } // if
    
    REG_DISPCTL ^= DISP_BACKBUFFER;

} // flipPage

// Set up and begin a DMA transfer
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt) {
    
    // Turn DMA off
    dma[channel].cnt = 0;

    // Set source and destination
    dma[channel].src = src;
    dma[channel].dst = dst;

    // Set control and begin
    dma[channel].cnt = cnt | DMA_ON;

} // DMANow

// Return true if the two rectangular areas are overlapping
int collision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {    
    return y1 < y2 + height2 - 1 && y1 + height1 - 1 > y2 && x1 < x2 + width2 - 1 && x1 + width1 - 1 > x2;

} // collision

void hide(ANISPRITE* sprite) {
    sprite->attributes->attr0 = ATTR0_OM(HIDE);

} // hide

// Hides all sprites in the shadowOAM; Must DMA the shadowOAM into the OAM after calling this function
void hideSprites() {

    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_OM(HIDE);
    
    } // for

} // hideSprites

void setAffineMatrix(int matrix, u16 a, u16 b, u16 c, u16 d) {
    affine[matrix].a = a;
    affine[matrix].b = b;
    affine[matrix].c = c;
    affine[matrix].d = d;

} // setAffineMatrix

// Copies the values of shadowOAM into object attribute memory
void updateOAM() {
    DMANow(3, shadowOAM, OAM, 512);

} // updateOAM