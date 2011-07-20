/*
  mosquino.h - board-specific definitions and macros for Mosquino (Aedes and probably others)
  Part of Mosquino

  Copyright (c) 2011 Tim Gipson (Drmn4ea)

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

  $Id: $
*/


#ifndef mosquino_h
#define mosquino_h

// For Mosquino, we will almost never not want this.
#include <avr/power.h>
#include <avr/sleep.h>

// Define so that libs can check for a specific board / family, rather than try to guess the board based on the CPU variant.
// family variant
#define MOSQUINO
// board variant. Aedes uses '644/1284/etc. and features PGOOD, PFULL and DS1337 realtime clock. Yes, they are all named after mosquito species.
#define MOSQUINO_AEDES
// PCB revision. It is very unlikely anyone will ever need to reference this, but it's here. (if they do, it probably means I did something stupid.)
#define MOSQUINO_AEDES_2


// There is a major limitation that user code or libraries cannot bounds-check that the pin # supplied to a e.g. digitalWrite() function is a valid pin number,
// nor do the Arduino functions themselves perform such a check. Try setting a trend now!

#define MAX_VALID_PIN 31

// Creature comfort defs for Mosquino 'special' pins
#define PIN_POWER_GOOD 24
#define PIN_POWER_FULL 25
#define PIN_INT2 26
#define PIN_SCL 27
#define PIN_SDA 28
#define PIN_LED 29
#define PIN_RX0 30
#define PIN_TX0 31


#define PIN_BUS_SENSE PIN_POWER_FULL
#define PIN_PGOOD PIN_POWER_GOOD
#define PIN_PFULL PIN_POWER_FULL




// Magic incantation to temporarily disable the ATmega's BOD (Brown Out Detect) during sleep mode, dropping sleep power consumption from ~20uA to <<1uA.
// After performing the ritual, user code has a limited time (a few cycles) to execute a SLEEP before BOD-during-sleep is re-enabled. So, it is
// probably a good idea to provide a wrapper which immediately follows this with a SLEEP. In fact, lets make sleeping without BOD the default behavior
// since a proper Mosquino powersupply will usually disconnect the load (AVR) on undervoltage..

// this "should" be in avr/sleep.h already, but as of 0022 Arduino still distributes a very old version that seemingly predates it! Sooo...
#ifndef sleep_bod_disable()
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

#endif