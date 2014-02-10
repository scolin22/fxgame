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

static void audio_out(SoundBuffer *sb, int length) {
    volatile int read = sb->read;
    volatile int max = sb->read + length;

    //printf("isr: sb->write %d, sb->read %d\n", sb->write, sb->read);

    while (read < max) {
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read]);
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read+1]);
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read+2]);
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read+3]);
        IOWR_ALT_UP_AUDIO_LEFTDATA(sb->audio->base, sb->mix[read+4]);

        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read]);
        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read+1]);
        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read+2]);
        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read+3]);
        IOWR_ALT_UP_AUDIO_RIGHTDATA(sb->audio->base, sb->mix[read+4]);

        read += 5;
    }
}

static void audio_isr(void *context, alt_u32 id) {
//    printf("In IRS\n");

//    SoundBuffer *sb;
//    sb = (SoundBuffer *) context;
    volatile SoundBuffer *sb2 = (volatile SoundBuffer *) context;

    //printf("sb2->read %d\n", sb2->read);
    audio_out(sb2, 95);
    sb2->read = (sb2->read + 95) % SIZE;

    //printf("Out IRS\n");
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

void initFX(char *filename, short int *storage) {
    int size = sizeWav(filename);
    printf("Size %d words (16-bit .wav)\n", size);

    storage = (short int *) malloc(size * sizeof(short int));
    wav fp = openWav(filename);
    int i = 0;
    short int data1, data2;

    for(i = 0; i < size; i++) {
        data1 = alt_up_sd_card_read(fp);
        data2 = alt_up_sd_card_read(fp);
        short int res = (data2 << 8) | data1;
        (storage)[i] = res / 2;
    }
    alt_up_sd_card_fclose(fp);
    printf("%s storage filled\n\n", filename);
}

//Init fx sounds
void initSoundFX(SoundBuffer *sb) {
/*    initFX("alive.wav", sb->alive);
    initFX("death.wav", sb->death);
    initFX("drop.wav", sb->drop);
    initFX("end.wav", sb->end);
    initFX("explode.wav", sb->explode);
    initFX("powerup.wav", sb->powerup);
    initFX("start.wav", sb->start);*/
}

//Init bg sound
int initSoundBG(SoundBuffer *sb) {
    //open bg.wav forever
    sb->fp = openWav("bg.wav");
    printf("sb->write %d, sb->read %d\n", sb->write, sb->read);

    //read in bg to fill mix
    short int data1, data2;
    while (sb->write != sb->read) {
        data1 = alt_up_sd_card_read(sb->fp);
        if (data1 < 0) {
            printf("early break");
            break; //reached EOF
        }
        data2 = alt_up_sd_card_read(sb->fp);
        short int res = (short int) (data2 << 8) | data1;
        sb->mix[sb->write] = res / 2;
        sb->write = (sb->write + 1) % SIZE;
    }

    printf("out: sb->write %d, sb->read %d\n", sb->write, sb->read);
    return 1;
}

void initSoundFinal(SoundBuffer *sb) {
    int set = alt_irq_register(AUDIO_IRQ, (void *) sb, audio_isr);
    printf("IRQ registered: %d\n", set);

    alt_up_audio_reset_audio_core(sb->audio);
    printf("Audio reset\n");

    alt_up_audio_enable_write_interrupt(sb->audio);
    printf("Interrupt enabled\n");
}

//Refresh bg sound
void refreshSoundBG(SoundBuffer *sb) {
    //check if mix can be written
    //write to mix

    //printf("out: sb->write %d, sb->read %d\n", sb->write, sb->read);
    short int data1, data2;
    while (sb->write != sb->read) {
        data1 = alt_up_sd_card_read(sb->fp);
        if (data1 < 0) {
            printf("early break\n");
            alt_up_sd_card_fclose(sb->fp);
            sb->fp = openWav("bg.wav");
            break; //reached EOF
        }
        data2 = alt_up_sd_card_read(sb->fp);
        short int res = (short int) (data2 << 8) | data1;
        sb->mix[sb->write] = res / 2;
        sb->write = (sb->write + 1) % SIZE;
    }
    //printf("sb->write %d, sb->read %d\n", sb->write, sb->read);
}
