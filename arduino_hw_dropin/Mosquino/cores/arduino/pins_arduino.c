/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

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

// ATMEL ATMEGA644P / MOSQUINO
//
//                   +---\/---+
//  INT0 (D 15) PB0  1|        |40  PA0 (AI 0 / D 23)
//  INT1 (D 14) PB1  2|        |39  PA1 (AI 1 / D 22)
//  INT2 (D 24) PB2  3|        |38  PA2 (AI 2 / D 21)
//   PWM (D 13) PB3  4|        |37  PA3 (AI 3 / D 20)
//   PWM (D 12) PB4  5|        |36  PA4 (AI 4 / D 19)
//  MOSI (D 11) PB5  6|        |35  PA5 (AI 5 / D 18)
//  MISO (D 10) PB6  7|        |34  PA6 (AI 6 / D 17)
//   SCK (D  9) PB7  8|        |33  PA7 (AI 7 / D 16)
//              RST  9|        |32  AREF
//              VCC 10|        |31  GND
//              GND 11|        |30  AVCC
//            XTAL2 12|        |29  PC7 (D  8)
//            XTAL1 13|        |28  PC6 (D  7)
//  RX0 (D 28)  PD0 14|        |27  PC5 (D  6) TDI
//  TX0 (D 29)  PD1 15|        |26  PC4 (D  5) TDO
//  RX1 (D  1)  PD2 16|        |25  PC3 (D  4) TMS
//  TX1 (D  0)  PD3 17|        |24  PC2 (D 27) TCK
//  PWM (D  3)  PD4 18|        |23  PC1 (D 26) SDA
//  PWM (D  2)  PD5 19|        |22  PC0 (D 25) SCL
//  PWM (D 30)  PD6 20|        |21  PD7 (D 31) PWM
//                    +--------+
//

#define PA 1
#define PB 2
#define PC 3
#define PD 4

// Creature comfort defs for Mosquino 'special' pins
#define PIN_INT2 24
#define PIN_SCL 25
#define PIN_SDA 26
#define PIN_LED 27
#define PIN_RX0 28
#define PIN_TX0 29
#define PIN_POWER_GOOD 30
#define PIN_BUS_SENSE 31

#define PIN_PGOOD PIN_POWER_GOOD



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
	PC,
	PC,
	PC, /* 8 */
	PB,
	PB,
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
	PC,
	PC,
	PC,
	PD,
	PD,
	PD,
	PD  /* 31 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(3), /* 0, port D */
	_BV(2), //D
	_BV(5), //D
	_BV(4), //D
	_BV(3), //C
	_BV(4), //C
	_BV(5), //C
	_BV(6), //C
	_BV(7), /* 8, port C */
	_BV(7), //B
	_BV(6), //B
	_BV(5), //B
	_BV(4), //B
	_BV(3), //B
	_BV(1), //B
	_BV(0), //B
	_BV(7), /* 16, port A */
	_BV(6),
	_BV(5),
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(0),
	_BV(2), /* 24, everything not on the shield headers */ //B INT2
	_BV(0), //C  SCL
	_BV(1), //C  SDA
	_BV(2), //C  LED
	_BV(0), //D  RX0
	_BV(1), //D  TX0
	_BV(6), //D  PGOOD
	_BV(7)  //D  BUS_SENSE
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, 	/* 0  - PB0 */
	NOT_ON_TIMER, 	/* 1  - PB1 */
	TIMER1A, 	/* 2  - PB2 */
	TIMER1B,     	/* 3  - PB3 */
	NOT_ON_TIMER, 	/* 4  - PB4 */
	NOT_ON_TIMER, 	/* 5  - PB5 */
	NOT_ON_TIMER, 	/* 6  - PB6 */
	NOT_ON_TIMER,	/* 7  - PB7 */
	NOT_ON_TIMER, 	/* 8  - PD0 */
	NOT_ON_TIMER, 	/* 9  - PD1 */
	NOT_ON_TIMER, 	/* 10 - PD2 */
	NOT_ON_TIMER, 	/* 11 - PD3 */
	TIMER0B,     	/* 12 - PD4 */
	TIMER0A,     	/* 13 - PD5 */
	NOT_ON_TIMER,   /* 14 - PD6 */
	NOT_ON_TIMER,   /* 15 - PD7 */
	NOT_ON_TIMER, 	/* 16 - PC0 */
	NOT_ON_TIMER,   /* 17 - PC1 */
	NOT_ON_TIMER,   /* 18 - PC2 */
	NOT_ON_TIMER,   /* 19 - PC3 */
	NOT_ON_TIMER,   /* 20 - PC4 */
	NOT_ON_TIMER,   /* 21 - PC5 */
	NOT_ON_TIMER,   /* 22 - PC6 */
	NOT_ON_TIMER,   /* 23 - PC7 */
	NOT_ON_TIMER,   /* 24 - PA0 */
	NOT_ON_TIMER,   /* 25 - PA1 */
	NOT_ON_TIMER,   /* 26 - PA2 */
	NOT_ON_TIMER,   /* 27 - PA3 */
	NOT_ON_TIMER,   /* 28 - PA4 */
	NOT_ON_TIMER,   /* 29 - PA5 */
	TIMER2B,        /* 30 - PA6 */
	TIMER2A         /* 31 - PA7 */
};
