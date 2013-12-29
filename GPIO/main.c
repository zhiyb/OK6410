#include <stdio.h>
#include "mem.h"
#include "gpio.h"

const unsigned char num[16] = {
//	0,	1,	2,	3,	4,
	0x3F,	0x06,	0x5B,	0x4F,	0x66,
//	5,	6,	7,	8,	9,
	0x6D,	0x7D,	0x27,	0x7F,	0x6F,
//	A,	B,	C,	D,	E,	F,
	0x77,	0x7C,	0x39,	0x5E,	0x79,	0x71,
};

int main(void)
{
	unsigned char i = 0;
	mem_open(GPIO_BASE);
	HWREG(GPCCON) = 0x11111111;
	HWREG(GPCDAT) = 0x00;
	HWREG(GPCPUD) = 0x0000;
	HWREG(USER_LED) = 0x0F;

	// Beep
	HWREG(GPFCON) = _BV(31);
	usleep(1000000);
	HWREG(GPFCON) = 0;

loop:
	HWREG(USER_LED) = (~i) & 0x0F;
	HWREG(GPCDAT) = num[i++];
	if (i == 16)
		i = 0;
	//usleep(1000000);
	goto loop;

	mem_close();
	return 0;
}
