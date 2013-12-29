#ifndef BEEP_H
#define BEEP_H

#include "mem.h"
#include "gpio.h"

static inline void beep(bool e)
{
	if (e) {
		HWREG(GPFCON) |= _BV(31);
		HWREG(GPFCON) &= ~_BV(30);
	} else
		HWREG(GPFCON) &= ~(_BV(31) | _BV(30));
}

#endif
