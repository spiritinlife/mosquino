/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Updated for Mosquino rev2 pinout; fix Sangino's original ASCII diagram for '644. 20110618 / tgipson
  Modified for Mosquino pinout. 20100927 / tgipson


  Copyright (c) 2005 David A. Mellis

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

  $Id: pins_arduino.c 254 2007-04-20 23:17:38Z mellis $
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

// On the Sanguino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA644P / MOSQUINO rev2
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


#define PA 1
#define PB 2
#define PC 3
#define PD 4

// Creature comfort defs for Mosquino 'special' pins
// #define PIN_POWER_GOOD 24
// #define PIN_POWER_FULL 25
// #define PIN_INT2 26
// #define PIN_SCL 27
// #define PIN_SDA 28
// #define PIN_LED 29
// #define PIN_RX0 30
// #define PIN_TX0 31
// 
// 
// #define PIN_BUS_SENSE PIN_POWER_FULL
// #define PIN_PGOOD PIN_POWER_GOOD
// #define PIN_PFULL PIN_POWER_FULL


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] =
{
        NOT_A_PORT,
        &DDRA,
        &DDRB,
        &DDRC,
        &DDRD,
};

const uint8_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	&PORTA,
	&PORTB,
	&PORTC,
	&PORTD,
};

const uint8_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	&PINA,
	&PINB,
	&PINC,
	&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
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

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(6), /* 0, port D */
	_BV(5), //D
	_BV(4), //D
	_BV(7), //D
	_BV(3), //C
	_BV(4), //C
	_BV(3), //C
	_BV(2), //C
	_BV(5), /* 8, port C */
	_BV(6), //B
	_BV(7), //B
	_BV(3), //B
	_BV(7), //B
	_BV(6), //B
	_BV(5), //B
	_BV(4), //B
	_BV(0), /* 16, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 24, everything not on the shield headers */ //B INT2
	_BV(1), //C  SCL
	_BV(2), //C  SDA
	_BV(0), //C  LED
	_BV(1), //D  RX0
	_BV(2), //D  TX0
	_BV(0), //D  PGOOD
	_BV(1)  //D  BUS_SENSE
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
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

