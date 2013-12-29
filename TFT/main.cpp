#include <iostream>
#include "tft.h"
#include "mem.h"
#include "gpio.h"
#include "beep.h"
#include "conv.h"

void init(void)
{
	mem_open(GPIO_BASE);
	HWREG(GPCCON) = 0x11111111;
	HWREG(GPCPUD) = 0x0000;
	HWREG(GPCDAT) = 0x00;
	HWREG(GPLCON1) &= 0xFFFFFFF0;
	HWREG(GPLCON1) |= 0x00000001;
	HWREG(GPLPUD) &= ~(_BV(8 * 2 + 1) | _BV(8 * 2));
	HWREG(GPLDAT) &= ~(_BV(8));
	tft.setBGLight(false);
	beep(true);
	usleep(500000);
	tft.setBGLight(true);
	beep(false);
	tft.init();
	tft /= tft.FlipLandscape;
	tft.setBackground(0x667F);
	tft.setForeground(0x0000);
}

int main(void)
{
	init();

	tft.clean();
	tft *= 1;
	tft << "*** TFT library testing ***\n";
	tft << "STDOUT output, orientation, FG/BG colour, BG light\n";
	tft *= 3;
	tft << "Font size test\n";
	tft *= 1;
	tft.setXY(300, 38);
	tft << "Change text position & word warp test\n";
	tft.frame(115, 56, 200, 10, 2, 0xF800);
	tft << "Draw frame test\n";
	tft.rectangle(118, 68, 180, 6, 0x07E0);
	tft << "Draw rectangle test\n";
	tft.point(120, 76, 0x001F);
	tft.point(122, 76, 0x001F);
	tft.point(124, 76, 0x001F);
	tft.point(126, 76, 0x001F);
	tft.point(128, 76, 0x001F);
	tft.point(130, 76, 0x001F);
	tft << "Draw points test\n";
	tft.line(200, 100, 300, 200, 0xF800);
	tft.line(300, 210, 200, 110, 0x001F);
	tft.line(200, 200, 300, 100, 0xF800);
	tft.line(300, 110, 200, 210, 0x001F);

	tft.line(100, 100, 300, 200, 0xF800);
	tft.line(300, 210, 100, 110, 0x001F);
	tft.line(100, 200, 300, 100, 0xF800);
	tft.line(300, 110, 100, 210, 0x001F);

	tft.line(200, 0, 300, 200, 0xF800);
	tft.line(300, 210, 200, 10, 0x001F);
	tft.line(200, 200, 300, 0, 0xF800);
	tft.line(300, 10, 200, 210, 0x001F);

	tft.line(100, 150, 300, 150, 0xF800);
	tft.line(300, 160, 100, 160, 0x001F);
	tft.line(250, 0, 250, 200, 0xF800);
	tft.line(260, 200, 260, 0, 0x001F);
	tft << "Draw lines test\n";

	uint16_t clr = 0;
	while (1) {
		tft.rectangle(20, 160, 60, 60, clr);
		clr += 0x0841;
	}

	mem_close();
	return 0;
}
