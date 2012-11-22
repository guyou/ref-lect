#include "mirror.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc <= 2) {
		fprintf (stderr, "Usage: %s delay tags...\n", argv[0]);
		return 1;
	}
	int delay = atoi (argv[1]);
	int i;
	for (i = 2 ; i < argc ; i++) {
		printf ("check returns %d\n", wait_token (NULL, argv[i], delay));
	}
	return 0;
}
