#ifndef ILI9341_H
#define ILI9341_H

#include <time.h>
#include "mem.h"
#include "gpio.h"

// 8-bit parallel interface I

#define TFT_IO	GPCDAT
#define TFT_CS	(1 << 0)	// Chip-select (0)
#define TFT_BLC	(1 << 1)	// Background light control (0)
#define TFT_RST	(1 << 2)	// Reset (0)
#define TFT_WR	(1 << 3)	// Parallel data write strobe (Rising)
#define TFT_RS	(1 << 4)	// Data(1) / Command(0) selection
#define TFT_RD	(1 << 5)	// Parallel data read strobe (Rising)
#define TFT_VSY	(1 << 6)	// VSYNC
#define TFT_FMK	(1 << 7)	// Frame mark

#define TFT_SWIO	GPLDAT
#define TFT_SWPIN	(1 << 8)
#define TFT_LOW()	HWREG(TFT_SWIO) = 0;
#define TFT_HIGH()	HWREG(TFT_SWIO) = TFT_SWPIN;
#define TFT_CTRL_NULL	(TFT_BLC | TFT_RST | TFT_WR | TFT_RS | TFT_RD)
#define TFT_CTRL(data)	do { \
	HWREG(TFT_IO) = 0x00; \
	TFT_HIGH(); \
	TFT_HIGH(); \
	HWREG(TFT_IO) = data; \
	TFT_LOW(); \
	TFT_LOW(); \
} while (0)
#define TFT_DATA(data)	do { \
	HWREG(TFT_IO) = 0x01; \
	TFT_HIGH(); \
	TFT_HIGH(); \
	HWREG(TFT_IO) = data; \
	TFT_LOW(); \
	TFT_LOW(); \
} while (0)

class ili9341
{
public:
	enum Orientation {Landscape, Portrait, FlipLandscape, FlipPortrait, \
		BMPLandscape, BMPPortrait, BMPFlipLandscape, BMPFlipPortrait};

	static inline void cmd(uint8_t dat);
	static inline void data(uint8_t dat);
	static inline void init(void);
	static inline void _setBGLight(bool ctrl);
	static inline void idle(bool e) {cmd(0x38 + e);}
	static inline void sleep(bool e) {cmd(0x10 + e);}
	static inline void inversion(bool e) {cmd(0x20 + e);}
	static inline void send(bool c, uint8_t dat);
	//static inline uint8_t recv(void);
	//static inline void mode(bool _recv);
	static inline void _setOrient(uint8_t o);
};

// Defined as inline to excute faster

inline void ili9341::cmd(uint8_t dat)
{
	HWREG(TFT_IO) = 0x02;
	TFT_HIGH();
	TFT_HIGH();
	HWREG(TFT_IO) = dat;
	TFT_LOW();
	TFT_LOW();
}

inline void ili9341::data(uint8_t dat)
{
	HWREG(TFT_IO) = 0x03;
	TFT_HIGH();
	TFT_HIGH();
	HWREG(TFT_IO) = dat;
	TFT_LOW();
	TFT_LOW();
}

inline void ili9341::_setBGLight(bool ctrl)
{
	TFT_CTRL(TFT_CTRL_NULL & ~(ctrl ? 0 : TFT_BLC));
}

inline void ili9341::_setOrient(uint8_t o)
{
	cmd(0x36);			// Memory Access Control
	switch (o) {
	case Landscape:
		data(0x28);		// Column Address Order, BGR
		break;
	case Portrait:
		data(0x48);		// Column Address Order, BGR
		break;
	case FlipLandscape:
		data(0xE8);		// Column Address Order, BGR
		break;
	case FlipPortrait:
		data(0x88);		// Column Address Order, BGR
		break;
	case BMPLandscape:
		data(0x68);		// Column Address Order, BGR
		break;
	case BMPFlipLandscape:
		data(0xA8);		// Column Address Order, BGR
		break;
	case BMPPortrait:
		data(0xD8);		// Column Address Order, BGR
		break;
	case BMPFlipPortrait:
		data(0x18);		// Column Address Order, BGR
		break;
	}
}

inline void ili9341::send(bool c, uint8_t dat)
{
	if (c)
		cmd(dat);
	else
		data(dat);
}

inline void ili9341::init(void)
{
	// Hardware reset
	TFT_CTRL(TFT_CTRL_NULL & ~TFT_RST);
	usleep(10);		// Min: 10us
	TFT_CTRL(TFT_CTRL_NULL);
	usleep(120000);
	cmd(0x28);		// Display OFF
	cmd(0x11);		// Sleep Out
	usleep(120000);
	cmd(0x34);		// Tearing Effect Line OFF
	cmd(0x38);		// Idle Mode OFF
	cmd(0x13);		// Normal Display Mode ON
	cmd(0x20);		// Display Inversion OFF
	cmd(0x3A);		// COLMOD: Pixel Format Set
	data(0x55);		// 16 bits/pixel
	cmd(0x36);		// Memory Access Control
	data(0x48);		// Column Adress Order, BGR
	cmd(0xB1);		// Frame Rate control, normal
	data(0x00);		// Faster
	data(0x18);
	cmd(0xB3);		// Frame Rate control, partial
	data(0x00);		// Faster
	data(0x18);
	cmd(0x29);		// Display On
	return;
	cmd(0x2C);		// Memory Write
	for (uint32_t r = 0; r < 320; r++)	// Black screen
		for (uint32_t c = 0; c < 240; c++) {
			data(0x00);
			data(0x00);
		}
	//cmd(0x29);		// Display On
}

#endif
