#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "otg.h"

const unsigned char num[16] = {
//	0,	1,	2,	3,	4,
	0x3F,	0x06,	0x5B,	0x4F,	0x66,
//	5,	6,	7,	8,	9,
	0x6D,	0x7D,	0x27,	0x7F,	0x6F,
//	A,	B,	C,	D,	E,	F,
	0x77,	0x7C,	0x39,	0x5E,	0x79,	0x71,
};

int main(int argc, char *argv[])
{
	int host;
	if (argc != 2)
		goto help;
	if (atoi(argv[1]) == 1)
		host = 1;
	else if (atoi(argv[1]) == 0)
		host = 0;
	else
		goto help;
	mem_open(OTG_BASE);
	HWREG(OPHYPWR) &= ~(_BV(4) | _BV(3));
	if (host)
		HWREG(OPHYCLK) |= _BV(6);
	else
		HWREG(OPHYCLK) &= ~_BV(6);
	printf("OPHYPWR: %01X\n", HWREG(OPHYPWR));
	printf("OPHYCLK: %02X\n", HWREG(OPHYCLK));
	mem_close();
	return 0;
help:
	printf("Usage: %s host\n" \
		"host -> 1: Use OTG as Host\n" \
		"host -> 0: Use OTG as Slave\n", argv[0]);
	puts("\nReport:");
	mem_open(OTG_BASE);
	printf("OPHYPWR: %01X\n", HWREG(OPHYPWR));
	printf("OPHYCLK: %02X\n", HWREG(OPHYCLK));
	mem_close();
	return 1;
}
