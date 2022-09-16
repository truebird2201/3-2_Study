#include "Common.h"

int main(int argc, char* argv[])
{
	u_short i = 0x0100;
	if (((char*)&i)[0])
		printf("BIG_ENDIAN\n");
	else
		printf("LITTLE_ENDIAN\n");
}
