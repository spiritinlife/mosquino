/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// For Mosquino, we will almost never not want this.
//#include <avr/power.h>
//#include <avr/sleep.h>

// Define so that libs can check for a specific board / family, rather than try to guess the board based on the CPU variant.
// family variant
#define MOSQUINO
// board variant. Aedes uses '644/etc. and features PGOOD, PFULL and DS1337 realtime clock. Yes, they are all named after mosquito species.
#define MOSQUINO_AEDES
// PCB revision. It is very unlikely anyone will ever need to reference this, but it's here. (if they do, it probably means I did something stupid.)
#define MOSQUINO_AEDES_4



#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8

// This definition (from Mosquino 0022) is deprecated now that Arduino officially provides an equivalent definition
#define MAX_VALID_PIN (NUM_DIGITAL_PINS - 1)

// FIXME: CHECKME below. Dyslexic analog pin mappings have long been a bane of Mosquino/mine.
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 16 : -1)

// On the '644, the PWM pins are: RB3, RB4, RD4, RD5, RD6, RD7.
// These correspond to Mosquino pin # 11, 15, 2, 1, 0, 3.

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1 || (p) == 2 || (p) == 3 || (p) == 11 || (p) == 15)


const static uint8_t TX1 = 6;
const static uint8_t RX1 = 7;

const static uint8_t SS   = 15;
const static uint8_t MOSI = 14;
const static uint8_t MISO = 13;
const static uint8_t SCK  = 12;

const static uint8_t POWER_GOOD = 24;
const static uint8_t POWER_FULL = 25;
const static uint8_t PIN_INT2 = 26;
const static uint8_t SCL = 27;
const static uint8_t SDA = 28;
const static uint8_t LED_BUILTIN = 29;
const static uint8_t RX0 = 30;
const static uint8_t TX0 = 31;

const static uint8_t A0 = 16;
const static uint8_t A1 = 17;
const static uint8_t A2 = 18;
const static uint8_t A3 = 19;
const static uint8_t A4 = 20;
const static uint8_t A5 = 21;
const static uint8_t A6 = 22;
const static uint8_t A7 = 23;


// NOTE: There is already an 'INT2' definition, so the INT2 *pin* must always be referenced as PIN_INT2
// For the rest, bridge the Arduino and Mosquino special-pin naming conventions
#define PIN_POWER_GOOD POWER_GOOD
#define PIN_POWER_FULL POWER_FULL
#define PIN_SCL SCL
#define PIN_SDA SDA
#define PIN_LED LED_BUILTIN
#define PIN_RX0 RX0
#define PIN_TX0 TX0

#define PIN_RX1  RX1
#define PIN_TX1  TX1

#define PIN_INT0 RX1
#define PIN_INT1 TX1

// Deprecated definitions
#define PIN_BUS_SENSE PIN_POWER_FULL
#define PIN_PGOOD PIN_POWER_GOOD
#define PIN_PFULL PIN_POWER_FULL



// On the '644, the low 4 bits of the PCICR register are interrupt enables for [bit=0, 1, 2, 3] = [PORTA, PORTB, PORTC, PORTD] respectively.
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 31) ? (&PCICR) : ((uint8_t *)0))

// Now this will get complicated because the Mosquino pin map is not remotely continuous with port pin #s.
// Easiest place to get the MPN -> PORT data is digital_pin_to_port_PGM[] table.
// PORTA (0) = 16-23
// PORTB (1) = 11-15, 24-26
// PORTC (2) = 4, 5, 8-10, 27-29
// PORTD (3) = 0-3, 6, 7, 30, 31
//#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))

#define digitalPinToPCICRbit(p) (((p) >= 16 && (p) <= 23) ? 0 : ((((p) >= 11 && (p) <= 15) || ((p) >= 24 && (p) <= 26)) ? 1 : (  (p)<=3 || (p)==6 || (p)==7 || (p)==30 || (p)==31) ? 3 : 2 ))

#define digitalPinToPCMSK(p) (((p) >= 16 && (p) <= 23) ? (&PCMSK0) : ((((p) >= 11 && (p) <= 15) || ((p) >= 24 && (p) <= 26)) ? (&PCMSK1) : (  (p)<=3 || (p)==6 || (p)==7 || (p)==30 || (p)==31) ? (&PCMSK3) : ((p<NUM_DIGITAL_PINS && p>0) ? (&PCMSK2) : (uint8_t *)0 )))

// Really no better way to do this; it's kind of a mess otherwise. digital_pin_to_PCMSK_bit[] table added below.
// Same contents as digital_pin_to_bit_mask_PGM[] but with the _BV removed.
// NOTE: 'extern' definition has also been added to Arduino.h.
#define digitalPinToPCMSKbit(p) (pgm_read_byte(digital_pin_to_PCMSK_bit + (p) ))



#ifdef ARDUINO_MAIN



// ATMEL ATMEGA644P / MOSQUINO
//
//                          +---\/---+
//     XCK0/T0 (D 24) PB0  1|        |40  PA0 (AI 7 / D 16)
//     CLKO/T1 (D 25) PB1  2|        |39  PA1 (AI 6 / D 17)
//   INT2/AIN0 (D 26) PB2  3|        |38  PA2 (AI 5 / D 18)
//   OC0A/AIN1 (D 11) PB3  4|        |37  PA3 (AI 4 / D 19)
//    OC0B/SS\ (D 15) PB4  5|        |36  PA4 (AI 3 / D 20)
//        MOSI (D 14) PB5  6|        |35  PA5 (AI 2 / D 21)
//        MISO (D 13) PB6  7|        |34  PA6 (AI 1 / D 22)
//         SCK (D 12) PB7  8|        |33  PA7 (AI 0 / D 23)
//                    RST  9|        |32  AREF
//                    VCC 10|        |31  GND
//                    GND 11|        |30  AVCC
//                  XTAL2 12|        |29  PC7 (D 10) TOSC2
//                  XTAL1 13|        |28  PC6 (D 9) TOSC1
//        RX0 (D 30)  PD0 14|        |27  PC5 (D 8) TDI
//        TX0 (D 31)  PD1 15|        |26  PC4 (D  5) TDO
//   RX1/INT0 (D  7)  PD2 16|        |25  PC3 (D  4) TMS
//   TX1/INT1 (D  6)  PD3 17|        |24  PC2 (D 29) TCK
//  XCK1/OC1B (D  2)  PD4 18|        |23  PC1 (D 28) SDA
//       OC1A (D  1)  PD5 19|        |22  PC0 (D 27) SCL
//       OC2B (D  0)  PD6 20|        |21  PD7 (D  3) OC2A
//                          +--------+
//

// Mosquino Special Pins (may not be available on headers)
// PGOOD\ (PB0) 24
// PFULL\ (PB1) 25
// INT2  (PB2)  26
// SCL (PC0)  27
// SDA (PC1)  28
// LED (PC2)  29
// RX0 (PD0)  30
// TX0 (PD1)  31


const uint8_t PROGMEM digital_pin_to_PCMSK_bit[] = {
	6, /* 0, port D */
	5, //D
	4, //D
	7, //D
	3, //C
	4, //C
	3, //D
	2, //D
	5, /* 8, port C */
	6, //C
	7, //C
	3, //B
	7, //B
	6, //B
	5, //B
	4, //B
	7, /* 16, port A */
	6, //A
	5, //A
	4, //A
	3, //A
	2, //A
	1, //A
	0, //A
	0, /* 24, everything not on the shield headers */ //B PGOOD_
	1, //B  PFULL_
	2, //B  INT2_
	0, //C  SCL
	1, //C  SDA
	2, //C  LED
	0, //D  RX0
	1  //D  TX0
};


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
        NOT_A_PORT,
        (uint16_t) &DDRA,
        (uint16_t) &DDRB,
        (uint16_t) &DDRC,
        (uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PD,
	PD,
	PC,
	PC,
	PD,
	PD,
	PC, /* 8 */
	PC,
	PC,
	PB,
	PB,
	PB,
	PB,
	PB,
	PA, /* 16 */
	PA,
	PA,
	PA,
	PA,
	PA,
   	PA,
	PA,
	PB, /* 24 */
	PB,
	PB,
	PC,
	PC,
	PC,
	PD,
	PD  /* 31 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(6), /* 0, port D */
	_BV(5), //D
	_BV(4), //D
	_BV(7), //D
	_BV(3), //C
	_BV(4), //C
	_BV(3), //D
	_BV(2), //D
	_BV(5), /* 8, port C */
	_BV(6), //C
	_BV(7), //C
	_BV(3), //B
	_BV(7), //B
	_BV(6), //B
	_BV(5), //B
	_BV(4), //B
	_BV(7), /* 16, port A */
	_BV(6), //A
	_BV(5), //A
	_BV(4), //A
	_BV(3), //A
	_BV(2), //A
	_BV(1), //A
	_BV(0), //A
	_BV(0), /* 24, everything not on the shield headers */ //B PGOOD_
	_BV(1), //B  PFULL_
	_BV(2), //B  INT2
	_BV(0), //C  SCL
	_BV(1), //C  SDA
	_BV(2), //C  LED
	_BV(0), //D  RX0
	_BV(1)  //D  TX0
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	TIMER2B, 	/* 0 */
	TIMER1A, 	/* 1 */
	TIMER1B, 	/* 2 */
	TIMER2A,     	/* 3 */
	NOT_ON_TIMER, 	/* 4 */
	NOT_ON_TIMER, 	/* 5 */
	NOT_ON_TIMER, 	/* 6 */
	NOT_ON_TIMER,	/* 7 */
	NOT_ON_TIMER, 	/* 8 */
	NOT_ON_TIMER, 	/* 9 */
	NOT_ON_TIMER, 	/* 10 */
	TIMER0A, 	/* 11 */
	NOT_ON_TIMER,   /* 12 */
	NOT_ON_TIMER,   /* 13 */
	NOT_ON_TIMER,   /* 14 */
	TIMER0B,        /* 15 */
	NOT_ON_TIMER, 	/* 16 */
	NOT_ON_TIMER,   /* 17 */
	NOT_ON_TIMER,   /* 18 */
	NOT_ON_TIMER,   /* 19 */
	NOT_ON_TIMER,   /* 20 */
	NOT_ON_TIMER,   /* 21 */
	NOT_ON_TIMER,   /* 22 */
	NOT_ON_TIMER,   /* 23 */
	NOT_ON_TIMER,   /* 24 */
	NOT_ON_TIMER,   /* 25 */
	NOT_ON_TIMER,   /* 26  */
	NOT_ON_TIMER,   /* 27 */
	NOT_ON_TIMER,   /* 28 */
	NOT_ON_TIMER,   /* 29 */
	NOT_ON_TIMER,   /* 30 */
	NOT_ON_TIMER    /* 31 */
};

#endif

// --------------- Mosquino-specific -------------------------------


// The POWER_GOOD and POWER_FULL signals of Mosquino are both active-low, which is counterintuitive, and probably confusing for some users.
// For clarity and futureproofing, these wrappers will return the correct polarity.

// In an early prototype, there was a dedicated BUS_SENSE pin whose only function was to detect the USB attached state.
// This function is now multiplexed with the POWER_FULL signal (attachment to a probably wall-powered PC is 'unlimited' power for our purposes).
// As before (but moreso), user code should actually check for a connection as part of its comm protocol rather than blindly assuming
// bus_sense() / power_full() means a PC is present and a client is listening.


#define bus_sense() power_full()
#define power_good() (!(digitalRead(PIN_POWER_GOOD)))
#define power_full() (!(digitalRead(PIN_POWER_FULL)))


// Magic incantation to temporarily disable the ATmega's BOD (Brown Out Detect) during sleep mode, dropping sleep power consumption from ~20uA to <<1uA.
// After performing the ritual, user code has a limited time (a few cycles) to execute a SLEEP before BOD-during-sleep is re-enabled. So, it is
// probably a good idea to provide a wrapper which immediately follows this with a SLEEP. In fact, lets make sleeping without BOD the default behavior
// since a proper Mosquino powersupply will usually disconnect the load (AVR) on undervoltage..

// this "should" be in avr/sleep.h already, but as of 0022 Arduino still distributes a very old version that seemingly predates it! Sooo...
#ifndef sleep_bod_disable
//#warning Your AVR-GCC version is ancient
#define sleep_bod_disable() \
do { \
   uint8_t tempreg; \
   __asm__ __volatile__("in %[tempreg], %[mcucr]" "\n\t" \
                        "ori %[tempreg], %[bods_bodse]" "\n\t" \
                        "out %[mcucr], %[tempreg]" "\n\t" \
                        "andi %[tempreg], %[not_bodse]" "\n\t" \
                        "out %[mcucr], %[tempreg]" \
                        : [tempreg] "=&d" (tempreg) \
                        : [mcucr] "I" _SFR_IO_ADDR(MCUCR), \
                          [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)), \
                          [not_bodse] "i" (~_BV(BODSE))); \
 } while (0)
#endif

// wrappers and (un)friendly names for AVR sleep functions. We can keep set_sleep_mode() as-is...
#define sleep() do {sleep_enable(); sleep_bod_disable(); sleep_cpu();} while(0)
#define sleep_with_bod() do {sleep_enable(); sleep_cpu();} while(0)


// General note: Wrapping macros in a "do{...} while(0)" structure is a hack that ensures its name can be used as a normal function
// (e.g. trailing semicolon) without problems. The compiler optimizes away the superfluous condition, so this doesn't affect the
// actual code generated.
// http://gcc.gnu.org/onlinedocs/cpp/Swallowing-the-Semicolon.html#Swallowing-the-Semicolon




// Support for manipulating the clock speed on the fly.

#define CLOCK_DIV_1     0x00
#define CLOCK_DIV_2     0x01
#define CLOCK_DIV_4     0x02
#define CLOCK_DIV_8     0x03
#define CLOCK_DIV_16    0x04
#define CLOCK_DIV_32    0x05
#define CLOCK_DIV_64    0x06
#define CLOCK_DIV_128   0x07
#define CLOCK_DIV_256   0x08
// remaining values are reserved.

#define set_clock_div(x)    do { CLKPR = (1<<CLKPCE); CLKPR = (x);} while(0)


// --------------- END Mosquino-specific -------------------------------

#endif