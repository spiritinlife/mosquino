# Power Shield Documentation #

## Piezoelectric / Electrostatic (pwr-piezo) ##

This power shield is suited for harvesting usable power from vibration using the straining of piezoelectric materials (e.g. PZT, PVDF), or other moderate voltage (5+ typ.), extremely low current sources such as alternating electric fields. It can also be used as a general purpose high-efficiency step-down voltage converter. It incorporates the [Linear Technology LTC3588](http://www.linear.com/product/LTC3588-1) IC, which integrates a bridge rectifier and high efficiency buck converter. A built-in undervoltage lockout (UVLO) prevents operation and current consumption until the input has charged up to a usable voltage.

**Startup:**
  * Min. Startup Voltage: 4.0V (1.8V output setting), 5.1V (3.3V output setting)
  * Shutdown voltage (UVLO): 2.9V (1.8V output setting), 3.7V (3.3V output setting)
  * Min. Startup Current: n/a
  * Min. recommended start-up capacitance: 150uF

**Input:**
  * Input Voltage Range: 0-20V; >20V input allowed if current limited to 5mA or less.
  * Input type: DC or AC (non polarized)

**Output:**
  * Type: Regulated (provide VREG)
  * Output voltage: 1.8V or 3.3V (selectable)
  * Output current: 100mA max.

**Power:**
  * Quiescent current (Vin < Vstart): 0.45uA
  * Quiescent current (Vin > Vstart): 0.95uA (buck enabled, sleeping @ 4.5V); 1.7uA (buck enabled; sleeping @ 18V)
  * Quiescent current (buck active): 150uA
  * Maximum Power Point (for piezo sources): 7.4V (1.8V output setting), 9.3V (3.3V output setting)

**Signals To Mainboard**
  * Provides PGOOD? YES (true when output is in regulation)
  * Provides PFULL? NO

Please refer to the LTC3588 datasheet for detailed specifications.

### User Settings ###

Add external capacitance as needed for your application runtime between the "+" and "-" pad blocks near the top-right side of the board.

Voltage Setting: Place a jumper across J1 pins as marked for either 1.8V or 3.3V output. NOTE: Not all Mosquino configurations or I/O devices support 1.8V operation.

### Advanced Settings ###

NOTE: A soldering iron is required to modify this setting.

The 'RM' (rectifier mode) solder jumper selects between fullbridge (default) or ground-referenced input. In the fullbridge mode, the rectifier rectifies both positive and negative half-waves of an AC input, with 2 diode drops. In the ground-referenced mode, the rectifier acts as a halfbridge rectifier (1 diode drop) with one input terminal over the rectifier and the other serving as a ground reference. This mode may allow operation from slightly lower input voltages.


## 600mV Stepup Converter (pwr-steup) ##

The 600mV stepup power shield (using the [STMicro L6920](http://www.st.com/internet/analog/product/75172.jsp) boost IC) boosts input voltages as low as 600mV (or less) to 3.3V. This power shield is ideally suited for powering a Mosquino circuit from a low voltage, moderately high current source such as a single AA cell. A gutted solar walk light (rechargeable battery/holder and solar cell intact) makes an ideal power source and maybe even project enclosure using this shield.

**Startup:**
  * Min. Startup Voltage: 350mV
  * Shutdown voltage: Approx. 275mV
  * Min. Startup Current: >10mA?
  * Min. recommended start-up capacitance: n/a (not recommended for cold start-up applications - see notes below)

**Input:**
  * Input Voltage Range: 0 ~ 3.6V (if input > 3.3V, output may float up to input voltage at extremely light loads. An optional Zener diode provides overvoltage protection, at the expense of additional leakage current)
  * Input type: DC (obey Mosquino mainboard polarity)

**Output:**
  * Type: Regulated (provide VREG)
  * Output voltage: 3.3V (adjustable for advanced users)
  * Max. Output current: Depends on input voltage. 30mA typ. (0.8V input), 180mA (1.2V input), 500mA max. (>2.5V input)

**Power:**
  * Quiescent current (Vin < Vstart): <=1uA
  * Quiescent current (Vin > Vstart): <=15uA typ.

**Signals To Mainboard**
  * Provides PGOOD? YES (true when input exceeds 0.8)
  * Provides PFULL? YES (true when input exceeds 1.23V)


Please refer to the L6920 datasheet for detailed specifications.

Note: Although this IC provides some sort of undervoltage lockout, since the startup voltage is so low, no reasonable capacitor value will fully start up the Mosquino board before dropping to the undervoltage threshold. Therefore, it is recommended to start this shield from a charged battery, or a power source which can provide >10mA start-up current (e.g. a typical solar cell receiving direct sunlight).

### User Settings ###

A power storage element (battery or cap/supercap) can be added between the "+" and "-" pads near the top of the board.

### Advanced Settings ###

NOTE: A soldering iron is required to modify these settings.

The L6920 provides an output voltage adjustment pin, FB. Jumpering this pin to the IC's output sets a fixed 3.3V output; jumpering to ground (remove 0-ohm jumper resistor at [R2](https://code.google.com/p/mosquino/source/detail?r=2), add 0-ohm jumper resistor at [R1](https://code.google.com/p/mosquino/source/detail?r=1)) sets 5V output. 5V output is unsuitable for Mosquino's regulated input and will likely damage your board; however, this adjustment may be useful for breadboard projects that require 5V. The output can also be adjusted linearly within the 2V ~ 5V range by an appropriate resistor divider between [R1](https://code.google.com/p/mosquino/source/detail?r=1) and [R2](https://code.google.com/p/mosquino/source/detail?r=2) (refer to the datasheet linked above).

The resistor divider ([R4](https://code.google.com/p/mosquino/source/detail?r=4), [R5](https://code.google.com/p/mosquino/source/detail?r=5)) sets the low battery detect threshold, which is used to generate the PFULL signal. The default of 1.23V (approximately equal to the full-charge voltage of a NiCd or NiMH battery) can be changed by replacing these resistor values.

# FIXME: Example with a hacked-up solar walk light


## Thermal (Peltier / Seebeck effect) Boost Converter (pwr-pelt) ##

This shield, using the [Linear Technology LTC3109](http://www.linear.com/product/LTC3109) ultralow voltage step-up IC, can harvest useful energy from thermal differences via the Seebeck effect. Typically, a Peltier device (normally used for cooling) is placed between two surfaces at different temperatures (e.g. a hot/cold surface and a large heat sink to ambient air) and will generate a current (at very low voltages) as heat flows across it. With a typical 40mm^2 Peltier element, this shield will produce useful power from a 2degC thermal gradient.

This shield should also be useful for other high current, extremely low voltage (down to ~20mV) power sources, such as fuel cells (untested).


**Startup:**
  * Min. Startup Voltage: ~30mV
  * Shutdown voltage: Approx. <30mV
  * Min. Startup Current: TBD
  * Min. recommended start-up capacitance: TBD (recommended for charging of a battery or other storage element)

**Input:**
  * Input Voltage Range: +/-2.0V absolute max; +/- 500mV recommended.
  * Input type: DC, but polarity unimportant (can operate on either polarity). Testing indicates operation from low-frequency AC sources too.

**Output:**
  * Type: Unregulated (provides VUNREG)
  * Output voltage: 4.1V
  * Max. Output current: Depends on input; 15mA max.

**Power:**
  * Quiescent current (Vin > Vstart): 6mA @ 30mV (6mA sounds like a lot, but @ 30mV it's only 180uWatts)

**Signals To Mainboard**
  * Provides PGOOD? YES (true when output voltage in regulation)
  * Provides PFULL? NO


### User Settings ###

An auxiliary storage element (e.g. supercapacitor) can be placed between the J5 pads (polarity as indicated), and will begin charging (up to 4.1V) if input power is available and the output has already been charged to 4.1V. When input power is lost, the output will be fed from the auxiliary source.

### Notes ###

When using a typical Peltier device as the power source, if ambient air is used as the heat/cold sink, ample heatsinking must be provided for the heat/cold on the other side to be dissipated. Otherwise, the 'large' thermal gradient between the hot and cold sides will not stay large, as the 'work' is done by the flow of heat between the two sides, and they are rather thermally conductive. A typical CPU heatsink, cooled by passive convection, probably won't be big enough unless the thermal gradient is large.

If one end is subject to frequent and rapid changes in temperature, a suitably large thermal mass could be used in place of a heatsink.