#include "mirror.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	for (i = 1 ; i < argc ; i++) {
		printf ("check returns %d\n", check_token (argv[i]));
	}
}
