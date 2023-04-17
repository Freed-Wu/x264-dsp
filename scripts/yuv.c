#if 0
gcc "$0" -o .deps/a && exec .deps/a "$@"
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../yuv.h"
#define FILENAME ".deps/a.yuv"
extern uint8_t yuv[];
/**
 * compare hashes of FILENAME and original yuv to check if yuv.h is correct
 *
 */
int main(int argc, char *argv[])
{
	FILE *f = fopen(FILENAME, "wb");
	if (f == NULL) {
		perror(FILENAME);
		exit(1);
	}
	if (fwrite(yuv, sizeof(uint8_t), sizeof(yuv)/sizeof(uint8_t), f) != sizeof(yuv)/sizeof(uint8_t)) {
		perror(FILENAME);
		exit(2);
	}
	return fclose(f);
}
