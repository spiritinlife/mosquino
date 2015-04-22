# Introduction #

Mosquino's "Aedes" (40-pin ATmega) mainboard is based on the Arduino, specifically the Sanguino variant. If you are familiar with the hardware end of Arduino/sanguino, most of the schematic (apart from the power section) should be self-explanatory. This document shows where to get the most current schematics and summarizes the unusual or nonobvious bits.

# Schematics Download (and PCB/Gerbers, Bill Of Materials, etc.) #

Downloading the actual design files (schematic/board/etc.) from the Downloads section is the most reliable way to ensure you are working with current copies. These are designed in the [EAGLE design software](http://www.cadsoftusa.com/eagle-pcb-design-software/) and can be viewed/edited in the [Freeware version](http://www.cadsoftusa.com/download-eagle/freeware/?language=en).

A [GIF](http://tim.cexx.org/projects/mosquino/sch/mosquino4_sch.png) and [PDF](http://tim.cexx.org/projects/mosquino/sch/mosquino.pdf) copy are also available for convenience. These are not guaranteed to be up-to-date always, but are good if you are just curious how Mosquino works under the hood and don't want to download anything.

For _bleeding-edge_ development versions, head on over to the Repository. Click on the **Source** tab above, select **Hardware** from the Repository dropdown list at the top of the page, and hit Browse. Be warned that the repository may contain boards that are not available in the general download area, are untested, and/or may not even work :-)

# Power Supply #

The onboard power management circuitry is the main thing that separates Mosquino from most other Arduino-based designs, and is worth some explaining. Please refer to the figure below for the explanation that follows.

![http://tim.cexx.org/projects/mosquino/sch/mosquino4-psu.png](http://tim.cexx.org/projects/mosquino/sch/mosquino4-psu.png)

The Mosquino board accepts power from three different sources:

  * Already regulated (1.8 ~ 3.3V) power from the Power Shield headers
  * Unregulated (0 ~ 15V) power from the Power Shield headers
  * USB power (5V) from a PC via the USB port)

Note however that a Power Shield will normally provide either regulated _or_ unregulated power, not both.

## Supply Priority and Backflow Prevention ##

In the schematic, these three sources are designated VREGIN, VUNREG and VUSB\_RAW, respectively. Each of these power sources are shown entering the power supply section near the lefthand side, marked Power Entry. Unregulated and USB power (if present) are dumped onto the common unregulated power signal, VCOM (the latter via a reverse-blocking diode). P-channel MOSFETs T1 and T2 will disconnect the regulated and unregulated supplies if USB power is present, so that they are not consumed unnecessarily or backdriven by USB power. D1 is a zener diode that will shunt excess voltage in the event that the unregulated input exceeds the 15V maximum.

Pre-regulated power (VREGIN), if present and not inhibited by USB power, flows directly to the board via its power switch SW1 (righthand side) without further monkeying. The unregulated source (VCOM) is switched on or off by P-channel MOSFET T4 by the undervoltage lockout circuit.

## Undervoltage Lockout Section ##

The undervoltage lockout (UVLO) is a crucial element for a board, such as Mosquino, whose power may come from very weak (low current), intermittent or very slowly-rising sources (e.g. a solar cell moving from complete darkness to sunrise and eventually full sun during the day). Many microcontrollers and other semiconductor devices misbehave when operated below their minimum operating voltages, typically by drawing excessive current. If the downstream circuit draws excessive current at, say, 0.8V (even if only over a narrow range such as 0.8 ~ 1.0V), on a weak power source or slowly-charging battery, the board will **never** start up - even if it draws extremely little power once operating normally - because the supply can never get "over the hump".

_For example: the MCP1703 regulator used in this circuit draws only a few uA (quiescent) at legal operating voltages, but as much as 100uA when undervoltaged slightly. This may not sound like much, but when running from a small Peltier device from bodyheat, it's the difference between reading and broadcasting several sensor readings per second, and doing absolutely nothing because you're unlikely to get >100uA from a small bodyheat harvester alone._

The UVLO prevents this case by keeping the downstream circuit disconnected until the input voltage (even if very slowly rising) is above the downstream stuff's minimum operating voltage, and disconnects it again if the voltage drops too low. In addition, it adds some headroom (hysteresis) so that the turn-on surge doesn't just immediately kick it back off again. The UVLO circuit is designed to be well-behaved, and draw a maximum of only a few uA of current, over the entire 0.0V ~ 15V range (including all undervoltage regions).

U1 is a comparator with a built-in 1.18V voltage reference (REF pin). ![R2](https://code.google.com/p/mosquino/source/detail?r=2) and ![R3](https://code.google.com/p/mosquino/source/detail?r=3) form a resistor voltage divider; the values are chosen so that when the actual input is equal to the turn-on threshold (3.7V), the divided value fed to the comparator's + input is equal to the 1.18V reference fed to the - input, turning on T3 and thus T4. Actually it is slightly more complex than that; ![R4](https://code.google.com/p/mosquino/source/detail?r=4) and ![R5](https://code.google.com/p/mosquino/source/detail?r=5) implement the hysteresis by providing positive feedback: that is, when the UVLO is turned on, they tweak the input to appear just a bit higher than it really is, so that the comparator won't actually turn off again until it is below the turn-on threshold by some amount. This operation, and the math for choosing the ![R2](https://code.google.com/p/mosquino/source/detail?r=2)/![R3](https://code.google.com/p/mosquino/source/detail?r=3)/![R4](https://code.google.com/p/mosquino/source/detail?r=4)/![R5](https://code.google.com/p/mosquino/source/detail?r=5) values, is described in detail [here](http://tim.cexx.org/?p=826).

The combination of T3 and T4 inverts the comparator output signal so that T4 is shut off when the comparator output (LDEN) is low. Finally, because the LTC1540 has a maximum operating voltage of 11V, but we want Mosquino to work with 12V-15V power supplies, the combination of [R6](https://code.google.com/p/mosquino/source/detail?r=6) and D3 (a 10V Zener diode) limit its supply voltage to a safe value.

_NOTE: The LTC1540's output may float up to a little over 1V before the LTC1540 reaches operational voltage. Thus T3 should be chosen with a gate threshold voltage greater than about 2V to guarantee that the circuit is not accidentally turned on by this spurious signal._

## Regulator ##

Next up is the MCP1703, a very low quiescent current, 3.3V low-dropout regulator (LDO). The Microchip Technologies' MCP170x family consists of several devices (currently '1700-'1703) with quiescent currents of about a few uA and maximum voltages ranging from 6V to 16V. Quiescent current generally scales upward with maximum voltage range. Currently there are even lower-current regulators available (such as Texas Instruments' TPS78033 family), but I did not find any with high (>=15V) maximum input voltages.

## Power Good sense ##

U2, another LTC1540 comparator, provides the Power Good (PGOOD\) signal to the Mosquino board. Its function is similar to the UVLO (voltage monitoring), but rather than cutting power to anything, it provides advanced warning to a user sketch if the input voltage is dropping to a level where the UVLO may kick in soon. No hysteresis is required for this circuit. Its power and monitored voltage are provided by VREGOUT and LDEN, respectively, preventing it from drawing any current when the UVLO is in lockout.

The PGOOD\ (and PFULL\) signals are _open-drain_ with a very weak pullup, meaning they can be asserted either by the mainboard or a Power Shield (i.e. either one can make the condition 'true' without interfering).

## Convenience Switch ##

T5 is a convenience feature that allows the Mosquino board to be powered, and programmed, from USB power even when no Power Shield is present. When no USB power is present, it prevents any source on VREG / VCC\_UNSW from backdriving U3.

_NOTE: Power Shields which provide VUNREG will bypass T5 by jumpering VREGOUT and VCC\_UNSW together. Again, it is mainly a convenience feature to allow USB operation without any Power Shield attached, and can be omitted if desired._

## Final Power Supply Note ##

MOSFETs, by virtue of their design, include a parasitic "[body diode](http://en.wikipedia.org/wiki/Power_MOSFET#Body_diode)" between the drain (cathode) and the source (anode) of the MOSFET. When the MOSFET is used for reverse voltage blocking, this is obviously undesirable. Fortunately, a MOSFET in the "on" state [conducts equally well](http://www.edaboard.com/thread149107.html) in the drain-source and source-drain directions, allowing them to be wired "backwards" to point the body diode in the desired direction. In the Mosquino power supply, several of the MOSFETs (T1, T2, T5) are reversed in this manner.