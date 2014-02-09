#include "Sound.h"

void initSound(SoundBuffer *sb) {
    alt_up_av_config_dev *av_config = NULL;
    av_config = alt_up_av_config_open_dev("/dev/av_config");
    printf("Init av_config: %p\n", av_config);

    while (!alt_up_av_config_read_ready(av_config))
        //stall while av_config is not ready
        ;

    sb->read = 0;
    sb->write = 1;
    sb->audio = alt_up_audio_open_dev("/dev/audio");

    printf("Init audio: %p\n", sb->audio);



    /*    typedef struct {
            int read;   //index where data starts reading
            int write;    //index where data can be written
            short int values[SIZE];    //array of elements
            alt_up_audio_dev *audio;
        } */
}

//Init fx sounds
void initSoundFX() {

}

//Init bg sound
void initSoundBG() {

}

//Refresh bg sound
void refreshSoundBG() {

}
