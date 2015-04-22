# Introduction #

Mosquino is a software-compatible derivative of the open-source Arduino platform that is designed for extremely low power consumption, operating from ambient (harvested) environmental energy sources such as heat, sunlight and vibration. (Or coincells, if you’re not feeling adventurous.) It operates from 1.8 ~ 3.3V, with a target quiescent power consumption of a few uA including realtime clock.

# Design goals and tradeoffs #
  * Balance power optimizations with usability and ease-of-use
  * Do not remove/cripple core Arduino features like built-in bootloader and USB programming.
  * Default operating voltage: 5V or 3.3V? (Chose 3.3V.)
  * "Good" tradeoff between input voltage range (0 ~ 15V) and low power
  * Make tradeoff between Arduino pin/shield compatibility and best fit for new low-power devices: it was decided to create a new pinout.
  * Support multiple devices (shields) without stacking; take advantage of bus interfaces (SPI, I2C) for shields: Mosquino shield interface
  * Support a wide variety of power sources on the same board: "power shield" interface
  * Larger pin count and device (RAM/Flash) for more complex applications
  * Ability to "cold start" cleanly from complete discharge (0.0V), very low-current and slowly-rising power sources.

# Unique Features / Differences from a standard Arduino #

  * 3.3V operation (5V-tolerant; operating voltage 1.8 ~ 5.0V)

An official Arduino runs at 5V. However, the electronics field has been rapidly migrating away from 5V to 3.3V and even lower, especially for low-power devices. Many of the most interesting devices, such as sensors and radios, are now exclusively 3.3V operation. It was decided to make Mosquino a 3.3-V design to keep up with this trend, reducing power consumption and making it easier to operate from power sources such as Li-Ion (3.7V) and lithium (3V) batteries.

  * Custom shield interface and pinout

Because existing Arduino shields are designed around 5V operation and very rarely optimized for power consumption, it was decided to innovate the Mosquino shield interface instead of matching the Arduino pinout.

Rationale:
Most existing shields would not work with Mosquino's 1.8V ~ 3.3V supply at all, and even if they did, would not be a good fit for its design goals (standby power consumption in the uA range).

Mosquino approach:
To be useful, a Mosquino sketch probably needs to do more than one "thing" (typical example: take measurements, record them, transmit them via RF), which often calls for more than one peripheral (shield).

Rather than a single shield covering the entire board, the Mosquino shield interface provides 3 identical shield "slots" (just under 1/3 board length), allowing up to 3 shields to be installed at once without stacking. A single shield may span multiple slots if needed (e.g. for more I/O), however it is expected the vast majority of typical sensors and other devices will fit in a single slot. The shield slot's pins are entirely aligned on .100" spacing, so that DIY shields can easily be made on perfboard or breadboards.

Each shield slot provides access to up to 8 I/O (4 guaranteed, up to 4 may be shared with other functions) and a shared signal block. The shared signal block consists of the shared SPI and I2C buses, two INTerrupt lines (or secondary RS232 port), Power and Ground, AREF, charging voltage (up to 15V) and the POWER\_GOOD / POWER\_FULL power management signals.

  * Power Shield Interface

While powering from a USB port or batteries is pretty straightforward, powering from small environmental power sources often requires a power conditioning circuit specifically tailored to that power source. For example, a power conversion circuit good for piezoelectric power sources would be a very poor fit for thermoelectric (Peltier) sources. In addition to I/O shield slots, Mosquino provides a 10-pin slot for an interchangeable "power shield" optimized for a given power source. The power shield may condition power supplied by the Mosquino board's spring terminals or its own power entry connector, and may output either pre-regulated (1.8 ~ 3.3V) power directly, or unregulated (0 ~ 15V) power through the Mosquino's onboard voltage regulator.

Power Shields have access to USB power when available, and may be designed to stack with another Power Shield (for example, a power conditioning shield stacked to a battery + charge management shield).

  * Onboard Real-Time Clock and calendar with battery back-up

An onboard low-power (<1uA) realtime clock with battery maintains accurate time even when all external power sources are removed. Besides general timekeeping tasks, this can be used for waking the board periodically at low duty cycles, allowing use of the ATmega's deepest sleep mode.


# Power Management Features #

  * No Standby Power Guzzlers

The PC interface (FTDI232 USB-to-serial converter) operates from isolated USB supply; does not consume current when USB cable unplugged. Likewise, the bootloader immediately exits if the USB cable is not attached. This allows the sketch to start up immediately without wasting juice waiting for bootloader activity.

  * Onboard power management signals

A POWER\_FULL / BUS\_SENSE signal tells if USB link / “infinite” power is present. This wired-OR signal can be asserted either by the Mosquino board or the Power Shield.

A POWER\_GOOD signal alerts the sketch when power source is nearly depleted. This wired-OR signal is asserted by the Mosquino board (when using the onboard regulator) or provided by the Power Shield.

  * Realtime Clock Interrupt and Hardware Interrupts

Hardware Real-Time Clock and three hardware INTerrupt lines (one for RTC, two uncommitted) to facilitate / encourage event-driven code, CPU sleep mode usage (as opposed to software delay loops) with periodic/scheduled wakeups.

  * Prioritized Power Supply

Primary (regulated or unregulated, e.g. battery) and USB power can be simultaneously connected to the Mosquino board safely, without backdriving one another. The primary supply is automatically disconnected (no power draw) when USB power is available. The priority feature uses reversed MOSFETs rather than "diode-OR"ing to avoid introducing a diode voltage drop to the primary supply.

  * Microamp Undervoltage lockout with hysteresis

Integrated load-switching prevents the circuit from drawing power until the voltage exceeds minimum requirements and there’s enough power to do something useful with. Since many devices behave strangely and often draw excessive current when driven with too-low voltages, the load switch avoids the problem of a slowly-charging power source never getting "over the hump". While in the locked out state, leakage into the Mosquino board is less than 2uA (typ.) over the entire (0.0V ~ 3.299V) range.