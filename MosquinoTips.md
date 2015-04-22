# Introduction #

Add your content here.


# General #

Problem: Board consumes excess power, undervoltages, shuts off or spontaneously reboots when running from a non-USB power source and the sketch uses Serial. The power source is OK.

Solution: Make sure your sketch raises the serial Tx pin (e.g. by using Serial.begin(), Serial.write(), etc.) ONLY if a USB connection is present, e.g. by checking the POWER\_FULL / BUS\_SENSE signal, or commenting out Serial usage for untethered use. The onboard Serial-USB converter IC is powered only when a USB cable is attached. If the serial Tx pin into this chip is driven HIGH when the chip is not powered, this voltage can leak through the chip onto the board's VUSB line (the power entry line from the USB port), which will cause external power sources to be disconnected as though a USB connection were present. Obviously, the sketch should make no attempt to drive the serial Rx line as an output either.

# Power Reduction Topics #

## General Program Design ##
FIXME: Writeme

## Floating Pins ##
Unconnected microcontroller input pins, sometimes called "floating" pins, are a potential source of excess power consumption. When there is nothing to drive voltage onto a pin (e.g. no output from the microcontroller and no external connection), the voltage on that pin is basically undefined and will float all over the place due to static charges in the air and on nearby objects, EMI from other circuitry, home AC wiring and all sorts of random stuff. The micro's internal ESD protection circuitry should clamp this into the range (GND ~ Vcc), but in many cases, this voltage will be somewhere in between a logic 1 or 0 level, causing the pin's input buffer to turn "half on" and consume excessive power.

If excessive power consumption is noticed, floating pins are one likely cause. This can be corrected by either enabling each unconnected pin's internal pullup resistor, or setting the unconnected pins as outputs (high or low; doesn't matter).

## Standby Power Guzzlers ##
FIXME: Writeme

### Important notes about the Power Reduction Register (PRR) ###

The ATmega's Power Reduction Register allows individual on-chip peripherals, such as the USART, TWI (I2C) and timers, to be halted at any time by **cutting the clock signal to them**, saving power. Therefore...

The PRR has an effect ONLY in the ATmega's active modes (e.g. when the ATmega's oscillator is running). In sleep modes that shut down the ATmega's oscillator (e.g. SLEEP\_MODE\_PWR\_DOWN), the peripherals will receive no clock, and thus consume minimum power, regardless of the PRR setting.

Any configuration changes to a peripheral must occur while that peripheral is clocked (i.e. not disabled by PRR setting). Configuration changes attempted on a disabled peripheral will have no effect (at best). For example, "disabling" the ADC via its PRR bit without turning it off first (via the ADEN bit in the ADCSRA register) will actually freeze it in the ON state (no longer clocked, but its internal voltage references will still be enabled, wasting ~90uA)!

## Misc. Power Reduction Tips & Tricks ##