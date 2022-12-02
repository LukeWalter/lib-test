#include <stdlib.h>
#include "gba.h"
#include "print.h"
#include "ss.h"

void initialize();
void initSprite();
void initMatrices();
void updateGame();
void drawGame();
void saveGame();
void getSave();

unsigned short buttons;
unsigned short oldButtons;

OBJ_ATTR shadowOAM[128];
AFFINE_MATRIX* affine;

ANISPRITE sprite;

typedef struct {
    ANISPRITE sprite;
    u8 clear; // Game Pak ROM bits initialized to 1

} SAVEDATA;

SAVEDATA* saveFile;
u8* transfer;

int main() {

    initialize();

    while (1) {

        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_START)) {

            mgba_printf("%x", sprite.attributes->attr0);
            mgba_printf("%x", sprite.attributes->attr1);
            mgba_printf("%x", sprite.attributes->attr2);
            mgba_printf("%u", sprite.attributes->fill);

        } // if

        updateGame();
        drawGame();

        if (BUTTON_PRESSED(BUTTON_SELECT)) {
            saveGame();
        
        } // if

    } // while

} // main

void initialize() {

    mgba_open();

    REG_DISPCTL = MODE0 | BG2_ENABLE | SPRITE_ENABLE;

    DMANow(3, ssPal, SPRITE_PALETTE, ssPalLen / 2);
    DMANow(3, ssTiles, SPRITEBLOCK, ssTilesLen / 2);

    hideSprites();

    buttons = BUTTONS;
    oldButtons = 0;

    initMatrices();
    getSave();

} // initialize

void initSprite() {

    sprite.width = 16;
    sprite.height = 16;

    sprite.worldX = 0;
    sprite.dx = 0;
    sprite.worldY = 0;
    sprite.dy = 0;

    sprite.prevAniState = 0;
    sprite.aniState = 0;
    sprite.currFrame = 0;
    sprite.aniCounter = 0;
    sprite.numFrames = 1;

    sprite.attributes = &shadowOAM[0];
    sprite.hide = 0;

    sprite.affineMatrix = 0;

} // initSprite

void initMatrices() {

    // Identity matrix
    affine[0].a = 1 << 8;
    affine[0].b = 0;
    affine[0].c = 0;
    affine[0].d = 1 << 8;

    // Scale down x2
    affine[1].a = 1 << 9;
    affine[1].b = 0;
    affine[1].c = 0;
    affine[1].d = 1 << 9;

    // Scale up x2
    affine[2].a = 1 << 7;
    affine[2].b = 0;
    affine[2].c = 0;
    affine[2].d = 1 << 7;

} // initMatrices

void getSave() {

    saveFile = malloc(sizeof(SAVEDATA));
    transfer = (u8*) saveFile;

    for (int i = 0; i < (int) sizeof(SAVEDATA); i++) {
        // mgba_printf("%u", GAMEPAK_ROM[i]);
        transfer[i] = GAMEPAK_ROM[i];

    } // for

    if (saveFile->clear) {
        mgba_printf("Start new save...");
        initSprite();

    } else {
        mgba_printf("You saved! Good job!");
        sprite = saveFile->sprite;

    } // if

    free(saveFile);
    
} // getSave

void updateGame() {

    sprite.dx = 0;
    sprite.dy = 0;

    if (BUTTON_HELD(BUTTON_UP)) {
        sprite.dy--;

    } // if

    if (BUTTON_HELD(BUTTON_DOWN)) {
        sprite.dy++;

    } // if

    if (BUTTON_HELD(BUTTON_LEFT)) {
        sprite.dx--;

    } // if

    if (BUTTON_HELD(BUTTON_RIGHT)) {
        sprite.dx++;

    } // if

    sprite.worldX += sprite.dx;
    sprite.worldY += sprite.dy;

} // updateGame

void drawGame() {

    sprite.attributes->attr0 = ATTR0_ROW(sprite.worldY) | ATTR0_SHAPE(SQUARE);
    sprite.attributes->attr1 = ATTR1_COL(sprite.worldX) | ATTR1_SIZE(SMALL);
    sprite.attributes->attr2 = ATTR2_TILEID(sprite.aniState * 2, sprite.currFrame * 2);

    if (BUTTON_HELD(BUTTON_A)) {
        sprite.attributes->attr0 |= ATTR0_OM(DOUBLEAFFINE);
        sprite.attributes->attr1 |= ATTR1_AFFINE(sprite.affineMatrix);        

    } // if

    waitUntilVBlank();
    updateOAM();

} // drawGame

void saveGame() {

    saveFile = malloc(sizeof(SAVEDATA));

    saveFile->sprite = sprite;
    saveFile->clear = 0;

    mgba_printf("(%d, %d)", sprite.worldX, sprite.worldY);

    transfer = (u8*) saveFile;

    for (int i = 0; i < (int) sizeof(SAVEDATA); i++) {
        // mgba_printf("%d", transfer[i]);
        GAMEPAK_ROM[i] = transfer[i];

    } // for

    free(saveFile);
    mgba_printf("Game saved! File size: %u bytes", sizeof(SAVEDATA));

} // saveGame