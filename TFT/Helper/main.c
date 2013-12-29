#include <avr/io.h>
#include <util/delay.h>

#define _NOP() __asm__ __volatile__("nop")

#define TFT_CS	(1 << 0)	// Chip-select (0)
#define TFT_BLC	(1 << 1)	// Background light control (0)
#define TFT_RST	(1 << 2)	// Reset (0)
#define TFT_WR	(1 << 3)	// Parallel data write strobe (Rising)
#define TFT_RS	(1 << 4)	// Data(1) / Command(0) selection
#define TFT_RD	(1 << 5)	// Parallel data read strobe (Rising)
#define TFT_VSY	(1 << 6)	// VSYNC
#define TFT_FMK	(1 << 7)	// Frame mark

#define TFT_CTRL_NULL	(TFT_BLC | TFT_RST | TFT_WR | TFT_RS | TFT_RD)

int __attribute__((noreturn)) main(void)
{
	//uint8_t pin;
	MCUCR |= 0x80;			// Disable JTAG
	MCUCR |= 0x80;			// Thanks to someone point that out

	DDRA = 0xFF & ~(TFT_VSY | TFT_FMK);
	DDRB = 0x80;
	DDRC = 0xFF;
	DDRD = 0x00;
	PORTA = 0xFF;
	PORTB = 0x7F;
	PORTC = 0xFF;
	PORTD = 0xFF;

loop:
	while (!(PINB & _BV(6)));
	//pin = PIND;
	//PORTB = 0xFF;
	switch (PIND) {
	case 0x00:	// Change Control PORT
		while (PINB & _BV(6));
		PORTC = PIND;
		PORTA = PIND;
	//case 0x01:	// Change Data PORT
		break;
	case 0x02:	// Send Command
		while (PINB & _BV(6));
		//PORTA = TFT_CTRL_NULL & ~(TFT_WR | TFT_RS);
		//PORTA = TFT_CTRL_NULL;
		PORTC = PIND;
		PORTA &= ~TFT_RS;
		PORTA &= ~TFT_WR;
		PORTA |= TFT_WR;
		PORTA |= TFT_RS;
		break;
	default:	//case 0x03:	// Send Data
		while (PINB & _BV(6));
		//PORTA = TFT_CTRL_NULL & ~(TFT_WR);
		//PORTA = TFT_CTRL_NULL;
		PORTC = PIND;
		PORTA &= ~TFT_WR;
		PORTA |= TFT_WR;
		break;
	}
	//PORTB = 0x7F;
	goto loop;
}
