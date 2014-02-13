#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "Sound.h"

//Open .wav file and discard first 44 bytes of data
wav openWav(char *filename) {
    wav fp = alt_up_sd_card_fopen(filename, false);
    printf("File %s opened %d\n", filename, fp);
    int i;
    for(i = 0; i < 44; i++){
        alt_up_sd_card_read(fp);
    }
    return fp;
}

//Get size of .wav data
int sizeWav(char *filename) {
    wav fp = openWav(filename);
    short int data;
    int size = 0;
    while(1) {
        data = alt_up_sd_card_read(fp);
        if (data < 0) break; //reached EOF
        alt_up_sd_card_read(fp);
        size++;
    }
    alt_up_sd_card_fclose(fp);
    return size;
}

void audio_out(SoundBuffer *sb, int length) {
    int read = sb->read;
    int max = sb->read + length;

    while (read < max) {
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read]);
        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read]);
        read++;
    }
}

void audio_isr(void *context, alt_u32 id) {
    SoundBuffer *sb = (SoundBuffer *) context;
    audio_out(sb, 95);
    sb->read = (sb->read + 95) % SIZE;
}

void refreshBGFile(SoundBuffer *sb) {
    printf("Refresh file\n");
    alt_up_sd_card_fclose(sb->fp);
    sb->fp = openWav("bg.wav");
}

void initSound(SoundBuffer *sb) {
    alt_up_av_config_dev *av_config = NULL;
    av_config = alt_up_av_config_open_dev("/dev/av_config");
    printf("Init av_config: %p\n", av_config);

    while (!alt_up_av_config_read_ready(av_config))
        //stall while av_config is not ready
        ;

    sb->read = 0;
    sb->write = 1;
    sb->mix = (short int *) malloc(SIZE * sizeof(short int));
    sb->audio = alt_up_audio_open_dev("/dev/audio");
    printf("Init audio: %p\n", sb->audio);
}

short int* initFX(char *filename, int *size) {
    *size = sizeWav(filename);
    printf("Size %d words (16-bit .wav)\n", *size);

    short int *storage = (short int *) malloc(*size * sizeof(short int));
    wav fp = openWav(filename);
    int i = 0;
    short int data1, data2;

    for(i = 0; i < *size; i++) {
        data1 = alt_up_sd_card_read(fp);
        data2 = alt_up_sd_card_read(fp);
        short int res = (data2 << 8) | data1;
        (storage)[i] = res / 2;
    }
    alt_up_sd_card_fclose(fp);
    printf("Filled at %p with %d\n\n", storage, (storage)[0]);
    return storage;
}

//Init fx sounds
void initSoundFX(SoundBuffer *sb) {
    sb->alive = initFX("alive.wav", &(sb->aliveSize));
    sb->death = initFX("death.wav", &(sb->deathSize));
    sb->drop = initFX("drop.wav", &(sb->dropSize));
    sb->end = initFX("end.wav", &(sb->endSize));
    sb->explode = initFX("explode.wav", &(sb->explodeSize));
    sb->powerup = initFX("powerup.wav", &(sb->powerupSize));
    sb->start = initFX("start.wav", &(sb->startSize));
}

//Init bg sound
void initSoundBG(SoundBuffer *sb) {
    //open bg.wav forever
    sb->fp = openWav("bg.wav");

    //read in bg to fill mix
    short int data1, data2;
    while (sb->write != sb->read) {
        data1 = alt_up_sd_card_read(sb->fp);
        data2 = alt_up_sd_card_read(sb->fp);
        short int res = (data2 << 8) | data1;
        sb->mix[sb->write] = res / 2;
        sb->write = (sb->write + 1) % SIZE;
    }
}

void initSoundFinal(SoundBuffer *sb) {
    int set = alt_irq_register(AUDIO_IRQ, (void *) sb, audio_isr);
    printf("IRQ registered: %d\n", set);

    alt_up_audio_reset_audio_core(sb->audio);
    printf("Audio reset\n");

    alt_up_audio_enable_write_interrupt(sb->audio);
    printf("Interrupt enabled\n");
}

//Refresh bg sound if mix can be written to
void refreshSoundBG(SoundBuffer *sb) {
	return;
    short int data1, data2;
    while (sb->write != sb->read) {
        data1 = alt_up_sd_card_read(sb->fp);
        if (data1 < 0) {
            refreshBGFile(sb);
            data1 = alt_up_sd_card_read(sb->fp);
        }
        data2 = alt_up_sd_card_read(sb->fp);
        if (data2 < 0) {
            refreshBGFile(sb);
            data2 = alt_up_sd_card_read(sb->fp);
        }
        short int res = (data2 << 8) | data1;
        sb->mix[sb->write] = res / 2;
        sb->write = (sb->write + 1) % SIZE;
    }
}

void addSound(SoundBuffer *sb, char *action) {
    short int *file;
    int size;
    int offset = 0;

    if (strcmp(action, "ALIVE") == 0) {
        file = sb->alive;
        size = sb->aliveSize;
    } else if (strcmp(action, "DEATH") == 0) {
        file = sb->death;
        size = sb->deathSize;
    } else if (strcmp(action, "DROP") == 0) {
        file = sb->drop;
        size = sb->dropSize;
    } else if (strcmp(action, "END") == 0) {
        file = sb->end;
        size = sb->endSize;
    } else if (strcmp(action, "EXPLODE") == 0) {
        file = sb->explode;
        size = sb->explodeSize;
    } else if (strcmp(action, "POWERUP") == 0) {
        file = sb->powerup;
        size = sb->powerupSize;
    } else if (strcmp(action, "START") == 0) {
        file = sb->start;
        size = sb->startSize;
        offset = 1;
    } else {
        printf("No action associated with this sound\n" );
        return;
    }

    printf("Add at %p with value %X\n", file, file[0]);

    int i;
    int write = sb->read;   //write to where are we about to read
    int reset = write;
    for(i = 0; i < size; i++) {
        sb->mix[write] += (file)[i];
        write = (write + 1) % SIZE;
    }
    if (offset == 1) {
        sb->read = reset;
        sb->write = write;
    }
}
