# Introduction #

The Mosquino core is a slightly modified version of the Arduino core. At the time of this writing (4/9/2102), the main changes are pin numbering, interrupts support and specific power management extensions.

This wiki page might also be a good guide to others wishing to create a custom Arduino-based board, as to my knowledge the list of required changes / hardcoded core stuff is not really documented anywhere.

# Overview (Arduino 1.0 series) #

As of 1.0, the Arduino project now stores board-specific support (notably, pin mappings) in a directory separate from the Arduino core, normally /hardware/projectname/variants/boardname/. Currently, the only file there is pins\_arduino.h.

The following specific changes are made:
  * Mosquino-specific extensions (macros) are added to `pins_arduino.h`.
  * Pin and related mappings (port, timer, etc.) in `pins_arduino.h` have been modified to match the Mosquino board pinout. Note that in 1.0, this file contains most of the **analog** pin mapping data as well.
  * Analog pin mapping added in wiring\_analog.c.
  * 'extern' declaration for a Mosquino-specific mapping table added to `Arduino.h`.
  * Added support for 4MHz CPU speed to delayMicroseconds() in `wiring.c`.

In addition, the following Arduino bug fixes (not Mosquino-specific, although for our purposes it is important that these functions actually work):
  * `WInterrupts.c` and 'wiring\_private.h' have modifications to fix the INT2 hardware interrupt ([Issue 879](https://code.google.com/p/mosquino/issues/detail?id=879)) and enable support for it on the ATmega644. These changes should eventually make their way into the official Arduino core.
  * (optional?) Updated versions of several files pulled from the GitHub repository.

## pins\_arduino.h ##

### Mosquino Extensions ###

The Mosquino extensions have been added in pins\_arduino.h.

Most of the contents here will be self-evident. One 'interesting' feature they contain - which I sincerely hope is adopted by other custom board designers and the Arduino team iteself - is definitions declaring the variant name (MOSQUINO), the board series/pinout (MOSQUINO\_AEDES) and the board revision (the latter is unlikely to be useful, but is there for completeness). This allows libraries authors who insist on hard-coding their own board-specific pin maps / etc. to at least be able to target actual boards, rather than guessing wildly based on the defined CPU type (e.g. `#ifdef ATMEGA1284 /* insert hardcoded Arduino Mega pin map here */`).

If you make a custom board / core based on Mosquino, please update these to reflect your own core and board names.

### Arduino Pin Numbers ###

All of the Arduino's pin mappings are found in the `pins_arduino.h` file and the needed changes for a custom pinout will be fairly obvious.

### Pin Change Interrupt support ###

Due to the large number of pins and the complexity of mapping pin-change interrupt bits on the Mosquino pinout via nested-if macro, a new table, `digital_pin_to_PCMSK_bit[]`, has been defined. A corresponding 'extern' declaration is added to `Arduino.h`. This table serves a similar purpose to the `digital_pin_to_port_PGM[]` table, but maps the pin-change interrupt mask bit number for each pin rather than a port bit mask.

The pin change interrupt macros (`digitalPinToPCICR(p), digitalPinToPCICRbit(p), digitalPinToPCMSKbit(p)`) have been modified to reflect the Mosquino pinout. The latter looks up a value from the `digital_pin_to_PCMSK_bit[]` table mentioned above.

### Pin Names ###

Definitions for the symbolic names for 'standard' named pins (e.g. `MISO, MOSI`) as well as Mosquino-specific named pins (e.g. `PIN_PGOOD`) have been modified or added, as appropriate.

## Arduino.h ##

The external declaration:
```
extern const uint8_t PROGMEM digital_pin_to_PCMSK_bit[];
```
has been added for the `digital_pin_to_PCMSK_bit[]` table added to `pins_arduino.h`.

## wiring\_analog.c ##

Near the beginning is a bit of 'smart' logic that guesses the Arduino board you are using based on the defined CPU and tries to guess whether the user passed an analog pin alias (A7..A0) or an Arduino digital pin number, and attempts to fix the latter case. After updating for Mosquino compatibility, this section looks like this:

```
#if defined(MOSQUINO_AEDES)
	if (pin >= 16 && pin <= 23) pin = 7 - (pin - 16); // allow for channel or pin numbers
#else #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif
```
Change as needed for your custom board. NOTE that functions in wiring\_analog.c use the passed-in pin numbers to directly index bits in ADC control registers (ADCSRB, ADMUX, etc.), if designing a board with a custom pin numbering, it is **strongly** recommended to keep the analog pins consecutive.

## wiring.c ##

This file contains some #ifdefs to let delayMicroseconds() support several (hardcoded) CPU speeds. A small tweak was added to add support for the Mosquino's default speed of 4MHz:

```
	// the following loop takes half of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.

	// ...but for Mosquino operation (4MHz default), only execute once

	#if F_CPU >= 8000000L
	us <<= 1;
	#endif
```

# Overview (pre-1.0 Arduino versions) #

The following specific changes are made:
  * New files, `mosquino.h/mosquino.c` have been added to contain Mosquino-specific definitions and extensions.
  * Pin mappings in `pins_arduino.c/pins_arduino.h` have been modified to match the Mosquino board pinout.
  * Pin mapping data for _analog_ input modified in `WProgram.h` and `wiring_analog.c`.
  * The `WInterrupts.c` file in 0022 is replaced by the previous version to fix an issue with interrupts when using sleep modes. Better solutions / fixes are welcome!

# Mosquino Extensions #

Two new files have been added to the core:
  * mosquino.c
  * mosquino.h

All Mosquino-specific features and extensions (not including pin numbers) will be here. These files are included from WProgram.h.

Most of the contents here will be self-evident. One 'interesting' feature they contain - which I sincerely hope is adopted by other custom board designers and the Arduino team iteself - is definitions declaring the variant name (MOSQUINO), the board series/pinout (MOSQUINO\_AEDES) and the board revision (the latter is unlikely to be useful, but is there for completeness). This allows libraries authors who insist on hard-coding their own board-specific pin maps / etc. to at least be able to target actual boards, rather than guessing wildly based on the defined CPU type (e.g. `#ifdef ATMEGA1284 /* insert hardcoded Arduino Mega pin map here */`).

If you make a custom board / core based on Mosquino, please update these to reflect your own core and board names.

# Pin Numberings #

## pins\_arduino.h/pins\_arduino.c ##

Most of the Arduino's pin mappings are found in the files pins\_arduino.h/pins\_arduino.c and the needed changes for a custom pinout will be fairly obvious; however, there are some less obvious hardcoded values scattered around the core. In particular, I have found the following locations to concern analog pin numbering:

## WProgram.h ##

The mappings of analog pin aliases A7..A0 are defined here, via a list of `const static uint8_t A0 = ...` lines. Change these to reflect the correct Arduino Pin Numbers.

## wiring\_analog.c ##

Near the beginning is a bit of 'smart' logic that guesses the Arduino board you are using based on the defined CPU and tries to guess whether the user passed an analog pin alias (A7..A0) or an Arduino digital pin number, and attempts to fix the latter case. After updating for Mosquino compatibility, this section looks like this:

```
#if defined(MOSQUINO_AEDES)
	if (pin >= 16 && pin <= 23) pin = 7 - (pin - 16); // allow for channel or pin numbers
#else #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif
```
Change as needed for your custom board. NOTE that functions in wiring\_analog.c use the passed-in pin numbers to directly index bits in ADC control registers (ADCSRB, ADMUX, etc.), so I strongly recommend following Arduino's pin numbering conventions here. That is, A0 is defined as 0 and corresponds to the ADC0/Px0 pin on the analog port, and so on.