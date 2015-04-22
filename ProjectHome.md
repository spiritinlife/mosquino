Mosquino is a software-compatible derivative of the open-source Arduino platform that is designed for extremely low power consumption, operating from ambient (harvested) environmental energy sources such as heat, sunlight and vibration. (Or coincells, if you’re not feeling adventurous.) It operates from 1.8 ~ 3.3V (regulated input) or up to 15V (unregulated input), with a target quiescent power consumption of <1uA including its onboard realtime clock.

![http://tim.cexx.org/projects/mosquino/shields-full_nobg.jpg](http://tim.cexx.org/projects/mosquino/shields-full_nobg.jpg)

## Why 'Mosquino'? ##
Mosquino is originally based on the [Sanguino](http://sanguino.cc/) design using the Atmel ATmega644PA and family microcontrollers. It turns out the name Sanguino literally translates as _bleeding_! In keeping with the theme, this parasitically-powered board is a "little bloodsucker", and pronounced like everyone's favorite flying insect. Don’t worry, it doesn’t drink much.

## Why yet-another Arduino-compatible board? ##

### Optimized for Minimal Power Consumption ###
Mosquino features a micropower onboard LDO voltage regulator with a quiescent current consumption of only a few uA and maximum 15V operating voltage, which can be bypassed for sub-uA idle currents. An onboard undervoltage lockout keeps the system powered down until the power supply has risen to a usable level. Two external hardware interrupt lines, an onboard realtime clock with alarm features, and support for interrupts on GPIO change make it easier to take advantage of the idle and Deep Sleep modes of the ATMega644/1284PA microcontrollers.

### Smart Expansion Interface ###
The Mosquino board allows up to 3 I/O shields to be installed at once without any stacking or pin conflicts. Common bus signals, including SPI and I2C, are available at all of the interchangeable shield sockets, allowing interface sharing and shield-shield communication. The shield interface is aligned on 100-mil headers, so that shields can also be built on standard perfboard and don't require PCB etching. Note that this means the shield interface is not directly compatible with typical Arduino shields.

### Power Shields ###
Mosquino can be powered from many different power sources with their own conditioning needs. Besides the usual I/O shields, the power conditioning is also implemented as a shield. The flexible Power Shield interface allows for direct powering or stacking with a battery charging shield, supplying either regulated or unregulated power to the mainboard (unregulated power is conditioned by an onboard 3.3V regulator). The interface also allows the power module to provide power management signals (Power Good / Power Full) which are usable by the user sketch.

Power shields for solar, vibration and thermal energy harvesting are currently tested and working, with additional modules in the works. Of course, power from a battery, USB or wall-wart can be directly supplied as well.