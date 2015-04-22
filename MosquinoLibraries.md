# Standard Libraries #

The [DS1337 Real Time Clock library](http://code.google.com/p/mosquino/wiki/DS1337) is needed in order to use the Mosquino board's onboard realtime clock. This download is highly recommended for all users.

# Other Recommended Libraries #

The [PinChangeInt](http://code.google.com/p/mosquino/downloads/list?q=label:pinchangeint) (pin change interrupt) library allows any I/O pin to be set as an interrupt source, generating an interrupt (and waking the CPU from sleep, if applicable) when the logic level on the pin changes. This is a modified version of the [arduino-pinchangeint](http://code.google.com/p/arduino-pinchangeint/) library updated for compatibility with ATMEGA644PA. See the [arduino-pinchangeint wiki](http://code.google.com/p/arduino-pinchangeint/w/list) for documentation.

Rocket Scream's [Lightweight Low Power Arduino Library](http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/) provides simplified access to the ATMega PRR (power reduction register) to individually disable peripherals to save power. It also provides a large selection of very low power nap modes (ranging to 15ms to 8s) using the watchdog timer. These are ideal for use in place of software delay loops where short delays are needed.

# Shield Support Libraries #

The [nvlcd](http://code.google.com/p/mosquino/wiki/nvlcd) library provides support for the nonvolatile graphical display shield.

The [sdfatlib](http://code.google.com/p/mosquino/downloads/list?q=label:sd) library provides support for the MicroSD card shield. This is a modified version of Bill Greiman's [sdfatlib](http://code.google.com/p/sdfatlib/) modified to support the Mosquino pin map. The documentation is included in the download.