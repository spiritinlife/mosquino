# Basics #
  * Processor: ATmega644PA or compatible (40-pin DIP), e.g. ATmega1284.
  * Input voltage range: 0 ~ 15VDC (unregulated input), 1.8 ~ 3.6V (regulated input)
  * Speed: 8MHz (3.3V operation); 4MHz and below (1.8 ~ 3.3V operation), software-selectable
  * Onboard Peripherals: Realtime Clock, USB-Serial interface, POWER\_GOOD / POWER\_FULL detect, LED.
  * I/O Shield pinout: Custom (does not match Uno/etc. pinout). The shield I/O has been logically split into 3 interchangeable sections with common power/ground and bus signals (SPI, I2C), allowing up to 3 I/O shields to be populated at once without stacking. Shield headers have been spaced to be perfboard-friendly.
  * Flexible power options - conditioning circuit for custom (incl. esoteric) power supplies can be provided via standardized "power shield" interface.
  * Power entry via direct wire (quick-connect spring terminals) or power shield-supplied connector.
  * Form factor compatible with the hobbyist's favorite brand of mints tin, providing instant FCC compliance ;-)

# Power Options #

Mosquino is designed to be very flexible with regard to power sources, operating from a wide range of ambient (harvested) energy sources in addition to conventional batteries or wall/USB power. Currently available power shields include:
  * Electrostatic/piezoelectric step-down shield (power from mechanical vibration, flex or electrostatic disturbances; high-impedance sources)
  * Peltier/Seebeck shield (power from temperature gradients of as low as 2degC; 20-30mV)
  * Boost shield (general-purpose 0.6V stepup regulator for solar cells and other low voltage sources)
  * LiPol shield (charging and power from small Li-Ion, LiPol and thin film batteries; stackable with other shields)

# Power Consumption #

## Active Power Consumption ##
**Using 8MHz crystal oscillator and divide-by-2 postscale (default):** 2.50mA @ 3.7V

**Using internal RC oscillator and divide-by-2 postscale:** 2.05mA @ 3.7V

Conditions: Approx. 20degC, all peripherals enabled (e.g. nothing disabled via Power Reduction Register), using onboard regulator, infinite while() loop.

## Quiescent (Sleep) Power Consumption ##
### Using externally regulated power source (VREG) ###
  * 0.4uA @ 1.8V
  * 0.6uA @ 3.3V

Conditions: Approx. 20degC; CPU in Power Save mode, onboard Realtime Clock not running.

The plot below shows the sleep current consumption over the **entire** 0.0 ~ 3.3V range. The board will not function through much of this range (<1.8V), even though it is drawing power - this plot is mainly here to show why UVLO is important :-)

![http://tim.cexx.org/projects/mosquino/vreg-current.png](http://tim.cexx.org/projects/mosquino/vreg-current.png)

### Using unregulated input (VUNREG); onboard regulator and undervoltage lockout: ###

Please refer to the following plots.

![http://tim.cexx.org/projects/mosquino/vunreg-current.png](http://tim.cexx.org/projects/mosquino/vunreg-current.png)
VUNREG Quiescent Current over full operating voltage range

![http://tim.cexx.org/projects/mosquino/vunreg-current-detail.png](http://tim.cexx.org/projects/mosquino/vunreg-current-detail.png)
VUNREG Quiescent Current detail over 0-10V range

Conditions: Approx. 20degC; CPU in Power Save mode, onboard Realtime Clock not running. NOTE: Above approx. 10VDC, overvoltage protection circuitry begins to kick in, resulting in higher quiescent current (but still much less than a stock Arduino board ;-)