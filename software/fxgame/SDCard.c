#include "sdcard.h"

int initSD(int *connected) {
    alt_up_sd_card_dev *device_reference = NULL;
    device_reference = alt_up_sd_card_open_dev("/dev/sd_card");

    if (device_reference != NULL) {
        if (DEBUG) printf("Device reference found\n");
        if ((*connected == 0) && alt_up_sd_card_is_Present()) {
            *connected = 1;
            if (DEBUG) printf("Card connected.\n");
            if (alt_up_sd_card_is_FAT16()) {
                if (DEBUG) printf("FAT16 file system detected.\n");
                printf("SD card initialized\n");
                return 0;
            } else {
                if (DEBUG) printf("Unknown file system.\n");
                return -1;
            }
        } else if ((*connected == 1) && (alt_up_sd_card_is_Present() == false)) {
            *connected = 0;
            if (DEBUG) printf("Card is not connected.\n");
            return 1;
        }
    } else {
        if (DEBUG) printf("Device reference not found\n");
        return -1;
    }
    return 0;
}

int readDir(char *dir, char *file) {
    //*dir and *file point to dir and file
    int res = alt_up_sd_card_find_first(dir, file);
    if (DEBUG) printf("Operation res: %d\n", res);
    if (res == 0) return 1; //success
    if (res == -1) return 0; //not found
    return -1; //error
}

int readDirMore(char *file) {
    //*file point to dir and file
    int res = alt_up_sd_card_find_next(file);
    if (DEBUG) printf("Operation res: %d\n", res);
    if (res == 0) return 1; //success
    if (res == -1) return 0; //not found
    return -1; //error
}

int writeFile(char *file, bool create, char *content) {
    FILE *fp;
    fp = alt_up_sd_card_fopen(file, false);
    if (DEBUG) printf("File %s opened %d\n", file, fp);

    int i = 0;
    while (content[i] != NULL) {
        int res = alt_up_sd_card_write(fp, content[i++]);
        if (res == false) return -1; //write error
    }

    int res = alt_up_sd_card_fclose(fp);
    if (DEBUG) printf("File %s closed %d\n", file, res);
    if (!res) return -1; //not closed
    return 0; //success
}

int readFile(char *file, bool create, char *result) {
    FILE *fp;
    fp = alt_up_sd_card_fopen(file, false);
    if (DEBUG) printf("File %s opened %d\n", file, fp);

    int index = 0;
    int res = 0;
    //does loop need conditions?
    while ((res != -1) && (res != -2)) {
        res = alt_up_sd_card_read(fp);
        if (res == -2) {
            if (DEBUG) printf("File %s cannot be read any more\n", file);
            break; //end of file
        } else if (res == -1) {
            if (DEBUG) printf("File %s is an invalid handle\n", file);
            break; //end or content or error
        } else {
            result[index++] = res;
            if (DEBUG) printf("%c", (char) res);
        }
    }

    res = alt_up_sd_card_fclose(fp);
    if (DEBUG) printf("File %s closed %d\n", file, res);
    if (!res) return -1; //not closed
    return 0; //success
}

// close file after done reading to reset next byte pointer
int readFileBytes(char *file, int bytes, char *result, FILE *fp) {
    if (DEBUG) printf("File %s opened %d\n", file, fp);
    if (fp == -2) {
        if (DEBUG) printf("FILE ALREADY OPEN\n");
        return -1;
    }

    int index = 0;
    uint8_t res = 0;
    while (index < bytes) {
        res = alt_up_sd_card_read(fp);
        if (res == -2) {
            if (DEBUG) printf("File %s cannot be read any more\n", file);
            result[index] = NULL;
            return 1; //end of file
        }
        if (res == -1) {
            printf("File %s is an invalid handle\n", file);
            result[index] = NULL;
            return -1; //end or content or error
        }
        result[index++] = res & 0xFF;
        // if (DEBUG) printf("%02X ", res);
    }
    result[index] = NULL;
    return 0; //success
}

int closeFile(char *file, FILE* fp) {
    FILE *status;
    status = alt_up_sd_card_fopen(file, false);
    int res = 0;
    if (status == -2) {
        res = alt_up_sd_card_fclose(fp);
    }
    if (DEBUG) printf("File %s closed %d\n", file, res);
    if (!res) return -1; //not closed
    return 0; //success
}
