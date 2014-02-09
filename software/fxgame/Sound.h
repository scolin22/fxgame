#ifndef SOUND_H_
#define SOUND_H_

#include <stdio.h>
#include <stdlib.h>
#include <altera_up_avalon_audio_and_video_config.h>
#include <altera_up_avalon_audio.h>
#include <altera_up_avalon_audio_regs.h>
#include <sys/alt_irq.h>

#define SIZE 1280

//Sound buffer object
typedef struct {
    int read;   //index where data starts reading
    int write;    //index where data can be written
    short int values[SIZE];    //array of elements
    alt_up_audio_dev *audio;
} SoundBuffer;

void initSound(SoundBuffer *sb);
void initSoundFX();
void initSoundBG();
void refreshSoundBG();

#endif
