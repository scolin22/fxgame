#include <stdio.h>
#include <stdlib.h>
#include <altera_up_sd_card_avalon_interface.h>

#define DEBUG 0

int main(void) {
	printf("Welcome to Fruit Explosion\n");

	int connected = 0;
	while (connected == 0) {
		initSD(&connected);
	}

	char folder[] = "/";
	int fileNameLength = 100;
	char file[fileNameLength];

	//Turn pointers into addresses with &
	readDir(&folder, &file);
	printf("File: %s\n", file);

	char put[] = "yay write file api is working, but starts from beginning\n";
	writeFile(&file, false, &put);

	int size = 255;
	char get[size];
	//read (do a read line api, and read next line, and read from byte, if can do a read from line?)
	readFile(&file, false, &get);
	printf("get[]: %s\n", get);

	readDirMore(&file);
	printf("File: %s\n", file);

	readDirMore(&file);
	printf("File: %s\n", file);

	readDirMore(&file);
	printf("File: %s\n", file);

	readDirMore(&file);
	printf("File: %s\n", file);

	char out[40];
	readFileBytes("HELLO.TXT", 5, &out);
	printf("out[]: %s\n", out);
	closeFile("HELLO.TXT");

	//reading one byte isn't working atm, need to pass in file handle, rather than file name.
	char oneByte = readFileByte("FILE.TXT");
	while ((oneByte != -1) && (oneByte != 1)) {
		printf("oneByte: %c", oneByte);
		oneByte = readFileByte("FILE.TXT");
	}
	closeFile("FILE.TXT");
	printf("\n");

    return 0;
}
