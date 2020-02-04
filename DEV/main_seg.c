#include "../Segmentation/build.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2) errx(1, "Mauvais arguements");

	int debug = strtoul(argv[1], NULL, 10);

	build("test.png", "text.txt", debug);

	return 0;
}
