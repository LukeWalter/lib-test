#include "gba.h"
#include "sound.h"

void setupSounds() {

    REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_TO_BOTH |
                     DSA_TIMER0 |
                     DSA_FIFO_RESET |
                     DSB_OUTPUT_RATIO_100 |
                     DSB_OUTPUT_TO_BOTH |
                     DSB_TIMER1 |
                     DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;
}

void playSoundA( const signed char* sound, int length, int loops) {
    dma[1].cnt = 0;

    int ticks = PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ;

    DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    REG_TM0CTL = 0;

    REG_TM0DATA = -ticks;
    REG_TM0CTL = TM_ON;

    soundA.data = sound;
    soundA.length = length;
    soundA.loops = loops;
    soundA.isPlaying = 1;
    soundA.duration = (VBLANK_FREQ * length) / SOUND_FREQ;
    soundA.vBlankCount = 0;
}


void playSoundB( const signed char* sound, int length, int loops) {

    dma[2].cnt = 0;

    int ticks = PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ;

    DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    REG_TM1CTL = 0;

    REG_TM1DATA = -ticks;
    REG_TM1CTL = TM_ON;

    soundB.data = sound;
    soundB.length = length;
    soundB.loops = loops;
    soundB.isPlaying = 1;
    soundB.duration = (VBLANK_FREQ * length) / SOUND_FREQ;
    soundB.vBlankCount = 0;
}

void setupInterrupts() {

	REG_IMASTER = 0;

    REG_IENABLE |= VBLANK_INTERRUPT;
    REG_DISPSTAT |= VBLANK_IENABLE;

    REG_IHANDLER = &interruptHandler;
	REG_IMASTER = 1;
}

void interruptHandler() {

	REG_IMASTER = 0;

	if(REG_IFLAG & VBLANK_INTERRUPT) {
        if (soundA.isPlaying) {

            soundA.vBlankCount = soundA.vBlankCount + 1;
            if (soundA.vBlankCount > soundA.duration) {
                if (soundA.loops) {
                    playSoundA(soundA.data, soundA.length, soundA.loops);
                } else {
                    soundA.isPlaying = 0;
                    dma[1].cnt = 0;
                    REG_TM0CTL = TM_OFF;
                }
            }
        }

        if (soundB.isPlaying) {

            soundB.vBlankCount = soundB.vBlankCount + 1;
            if (soundB.vBlankCount > soundB.duration) {
                if (soundB.loops) {
                    playSoundB(soundB.data, soundB.length, soundB.loops);
                } else {
                    soundB.isPlaying = 0;
                    dma[2].cnt = 0;
                    REG_TM1CTL = TM_OFF;
                }
            }
		}


		REG_IFLAG = REG_IFLAG;
	}

	REG_IMASTER = 1;
}

void pauseSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;
    REG_TM0CTL = TM_OFF;
    REG_TM1CTL = TM_OFF;
}

void unpauseSounds() {
    soundA.isPlaying = 1;
    soundB.isPlaying = 1;
    REG_TM0CTL = TM_ON;
    REG_TM1CTL = TM_ON;
}

void stopSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;
    REG_TM0CTL = TM_OFF;
    REG_TM1CTL = TM_OFF;
    dma[1].cnt = 0;
    dma[2].cnt = 0;
}