#include <stdio.h>

extern char* buf;

void func()
{
	printf("%s", buf);
}

