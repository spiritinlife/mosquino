# Mosquino Pinout #

For ordinary pins, the pin number is as marked on the board. For the topside row (GPIO) these start at 0 at the top-right and end at 23 at the top-left. (FIXME: Add diagram here). Note that pins 16-23 are _analog_ inputs by default. Pin 16-23 correspond to analog pin # A7..A0 (yes, the numeric order is reversed from the digital pin #s.) This is for backward (bugward?) compatibility with Arduino versions and libraries that ignore the pin mapping and use the supplied pin number to write registers directly.


## Special I/O Pins ##

These pins either provide Mosquino special functions or are not part of the standard I/O pin block. They are accessible to user sketches via the pin names or numbers below.

| Name | Pin # | Pinned out? | User Function | Notes |
|:-----|:------|:------------|:--------------|:------|
| PIN\_POWER\_GOOD | 24 | Yes | Input | Open-drain, pulled LOW if sufficient power is available<sup>1</sup> |
| PIN\_POWER\_FULL | 25 | Yes | Input | Open-drain, pulled LOW if power source full or 'infinite' power available<sup>2</sup> |
| PIN\_INT2 | 26 | No | N/A | Dedicated interrupt pin for realtime clock |
| PIN\_SCL | 27 | Yes | I/O | Can be used as shared GPIO<sup>3</sup> when not using I2C |
| PIN\_SDA | 28 | Yes | I/O | Can be used as shared GPIO<sup>3</sup> when not using I2C |
| PIN\_LED | 29 | No | N/A | Connected to the onboard LED |
| PIN\_RX0 | 30 | No | N/A | USB-Serial Rx |
| PIN\_TX0 | 31 | No | N/A | USB-Serial Tx |


(1) When using the onboard regulator or a power shield that provides POWER\_GOOD

(2) When using a power shield that provides POWER\_FULL, or the USB cable is attached

(3) Can be used as GPIO by a single shield at a time, or for shield-to-shield communication. Be sure only one shield is driving this pin as an OUTPUT at any given time.


## Multi-Function Pins ##

These pins are available on the standard I/O pin block, but have extra functions and/or are present on the Bus Block (i.e. may have special requirements when multiple shields are loaded).

| Name | Pin # | On Bus Block? | User Function(s) | Notes |
|:-----|:------|:--------------|:-----------------|:------|
| PIN\_TX1 | 6 | Yes | I/O, 2nd Hardware Serial Tx, INT1 | Open-drain recommended for INTx functions |
| PIN\_RX1 | 7 | Yes | I/O, 2nd Hardware Serial Rx, INT0 | Open-drain recommended for INTx functions |
| SCK | 12 | Yes | I/O, SPI Clock |  |
| MISO | 13 | Yes | I/O, SPI Master Input / Slave Output |  |
| MOSI | 14 | Yes | I/O, SPI Master Output / Slave Input |  |
| SS | 15 | Yes | I/O, SPI Slave Select | Must be left as an input to use SPI Slave mode |

## Non-I/O Pins ##

In addition to I/O pins, the following special pins are available on the Bus Block for use by shields.

  * VCC and GND: Mosquino's main power supply and ground.
  * VCOM: Mosquino's unregulated power input (VUNREG or VUSB, as available). Be aware that the Mosquino's specs permit up to 16V to be on this pin!
  * AREF: Optional analog reference.


# Mosquino Power Management Extensions #

## Power Sensing ##

Mosquino's VUNREG input features an integrated undervoltage lockout (UVLO) circuit, which keeps the board in full shutdown until the input voltage is adequate for normal operation. This prevents erratic operation or, more importantly, excessive current consumption when the input voltage is too low. For a weak power source such as a small solar cell or Peltier device, this would otherwise prevent the supply from ever rising to a usable level. The UVLO switches on when the voltage rises to approximately 3.7V and switches off if it falls below approximately 3.3V.

Mosquino provides two signals, POWER\_GOOD and POWER\_FULL, that allow the user sketch to determine how much power is available before starting a potentially hungry task. Note that the exact behavior of each signal, when using a power shield, is up to the shield's designer and any user-configurable settings it has. Consult the individual shield's documentation in this case.

**POWER\_GOOD** tells if there is a reasonable amount of power available for the next task. When using the onboard VUNREG supply, POWER\_GOOD is asserted when the input voltage is at least 3.6V.

Note that how much is considered a "reasonable" amount of power depends on the longest atomic task to be performed. it is up to the user to ensure that the runtime between POWER\_GOOD becoming false and the UVLO kicking in is adequate for the desired task. For example, size the battery or input capacitor(s) as needed.

**POWER\_FULL** tells if "infinite" power, e.g. USB or wall plug power, is available. It will assert when the USB cable is plugged in OR when asserted by the power shield, if any. This signal from a shield may indicate that a battery cannot be charged further, and/or when input power is being shunted (wasted) because it cannot be used. This signal may be used to trigger power-hungry tasks or enable charging of a backup battery, etc.

```
  uint8_t power_good(void)
```

Returns TRUE if the POWER\_GOOD signal is asserted, FALSE otherwise.

```
  uint8_t power_full(void)
```

Returns TRUE if the POWER\_FULL signal is asserted, FALSE otherwise.

Example usage (pseudocode):
```
void setup()
{
  set_up_periodic_interrupt(); // wake via realtime clock every 60 seconds
}

void loop()
{
  while(power_full())
    {
      do_really_hungry_task();
    }
  if(power_good())
    {
      take_measurement();
      broadcast_measurement();
    }
  sleep(); // back to sleep until next measurement period. If power was not good, we did nothing and are just waiting for the power source to recharge.
}
```

## Sleep and Sleep-Related Functions ##

```
  sleep()
```
Macro function that disables the ATMega's Brown Out Detect (BOD) and puts it to sleep using the last mode set by `set_sleep_mode(<mode>)` (see below). In many cases, such as when using the Mosquino VUNREG supply, the onboard or shield-provided undervoltage lockout makes the BOD redundant. Turning it off saves about 17uA in sleep mode.


```
  sleep_with_bod()
```
Macro function that puts the ATMega to sleep using its lowest power SLEEP mode, while leaving the BOD enabled.

For more precise control of sleep mode entry, and the available sleep modes, see the next section.

### From AVR-GCC ###

#### Sleep Functions ####

The compiler underlying Arduino, avr-gcc, provides several handy power management functions of its own. This section provides a very brief overview of the the most important ones; you should check the following pages for the full list and how to use them:

[AvrPowerReduction](http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html)

[AvrSleep](http://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html)


To use them, you must add
```
#include <avr\power.h>
#include <avr\sleep.h>
```

to your sketch.

```
  sleep_bod_disable()
```
Macro that disables the ATMega's Brown Out Detect (BOD) during the next sleep\_cpu() instruction. You must call sleep\_cpu() immediately after sleep\_bod\_disable() for it to take effect.

At the time of this writing, the Arduino software ships with an older avr-gcc that does not include this function. The Mosquino core will provide its own copy if not present from avr-gcc.

```
  void set_sleep_mode (uint8_t mode)
  void sleep_mode (void)
  void sleep_enable (void)
  void sleep_disable (void)
  void sleep_cpu (void) 
```

These are the core functions for controlling sleep mode. `set_sleep_mode(<MODE>)` sets which sleep mode to use (more on that later), but does not put the CPU to sleep. After setting the desired sleep mode, use `sleep_mode()` to actually enter sleep mode.

The sleep\_enable/disable and sleep\_cpu() functions provide more granular control of the sleep process, for special cases where using the sleep\_mode macro could create a race condition. For example, this may be required in test-and-sleep scenarios to take care of not missing the interrupt that will awake the device from sleep.

The sleep modes defined by avr-gcc are as follows, roughly in order of increasing power savings / reduced functionality during sleep:
| SLEEP\_MODE\_IDLE | CPU clock disabled, but oscillator and all peripherals still clocked. |
|:------------------|:----------------------------------------------------------------------|
| SLEEP\_MODE\_ADC | Like Idle, but also disables the I/O clock. This mode is recommended to reduce clock noise during sensitive ADC readings. |
| SLEEP\_MODE\_PWR\_SAVE | Identical to ...PWR\_DOWN, except that any timers currently enabled will remain operating. |
| SLEEP\_MODE\_EXT\_STANDBY | Equivalent to ...POWER\_SAVE, but keeps the external crystal (if any) running, allowing the CPU to return to active mode in only 6 clock cycles. |
| SLEEP\_MODE\_STANDBY | Identical to ...PWR\_DOWN, but keeps the external crystal (if any) running, allowing the CPU to return to active mode in only 6 clock cycles. |
| SLEEP\_MODE\_PWR\_DOWN | The ultimate sleep mode; ALL clocks except WDT (if used) disabled. An asynchronous interrupt (e.g. INTx or I2C address match) must be used to wake the CPU from this mode. |

Yes, it's a slightly bewildering array of power saving options. For all the grisly details, please refer to the ATMEGA644PA's data sheet. In addition, the folks at Rocketscream have put together a very exhaustive test where the [current consumption in each mode with various options](http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/) is compared.

Note that the ATMEGA644PA (the default Mosquino CPU) supports all six modes; however, not all ATMegas support all modes. Check this if using a different CPU.


#### Clock Speed ####

The default Mosquino configuration is with an 8MHz crystal oscillator. However, it starts up at 4MHz (via clock prescaler setting) to guarantee reliable operation over the entire (1.8V..3.3V) operating voltage. If you know your project will only be operated at 3.3V and need the extra speed, or have a need to lower the clock speed further, you can use the settings below to modify the clock prescaler.

```
  clock_prescale_set(x)
```
Sets the clock prescaler to the value specified (see below for available options). Remember, to get back to 'normal' operation, set this to divide by 2 (`clock_div_2`).

Keep in mind that you will need to switch back to the 'normal' (4MHz) clock speed before using timing-dependent resources, such as UART functions (Serial.print(), etc.). Also remember that changing the clock prescaler will alter the timing of timer resources (incl. PWM) and functions such as delay() and millis().

```
  clock_prescale_get() 
```
Get the current clock prescaler setting. The return type is 'clock\_div\_t' (see below for values).

```
typedef enum
{
    clock_div_1 = 0,
    clock_div_2 = 1,
    clock_div_4 = 2,
    clock_div_8 = 3,
    clock_div_16 = 4,
    clock_div_32 = 5,
    clock_div_64 = 6,
    clock_div_128 = 7,
    clock_div_256 = 8,
    clock_div_1_rc = 15, // ATmega128RFA1 only
} clock_div_t;
```

#### Power Reduction Register ####

Please see the official [AvrPowerReduction](http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html) page for the list of relevant functions.

The ATMEGA644PA (and many others) feature a Power Reduction Register (PRR) that allows you to individually enable or disable the clock to specific on-chip peripherals, such as the ADC or SPI. If you know you will not use a particular resource in your project, this may help you save power during idle/active modes. Note that during many sleep modes, peripherals clocks are disabled anyway, so PRR settings will not affect power consumption in these sleep modes.

Keep in mind that some on-chip peripherals require an active clock in order to reconfigure, so make any necessary changes _before_ disabling them via the PRR functions. For example, the ADC module must be enabled/clocked in order to disable any internal voltage references.