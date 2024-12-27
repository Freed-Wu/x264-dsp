#include <stdio.h>
#include <stdlib.h>

#include "extras/getopt.h"

static char shortopts[] = "0:1:2:";
static struct option longopts[] = {
    {"option0", required_argument, NULL, '0'},
    {"option1", required_argument, NULL, '1'},
    {"option2", required_argument, NULL, '2'},
    {NULL, 0, NULL, 0}};

int main(int argc, char *argv[]) {
	int c;
	char *results[3] = {};
	while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		switch (c) {
		case '0':
			results[0] = optarg;
			break;
		case '1':
			results[1] = optarg;
			break;
		case '2':
			results[2] = optarg;
			break;
		default:
			return -1;
		}
	}
	for (c = 0; c < 3; ++c)
		printf("%s\n", results[c]);
	return EXIT_SUCCESS;
}
