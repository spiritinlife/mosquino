# Introduction #

The Mosquino mainboard includes a socket for interchangeable power conditioning boards, known as Power Shields (following the Arduino terminology of "shields" aka expansion boards). This allows the mainboard to be efficiently used with a wide variety of power sources.

The Power Shield typically accepts power from the mainboard's power terminals (though it may have its own connector for this purpose), performs any conditioning needed for the type of power source it is designed for, and passes its output to the mainboard in either pre-regulated or unregulated form.

Power Shields may optionally allow stacking (typically battery charging boards that can be stacked with a power conditioning board) and generate their own custom power management signals (POWER\_GOOD, POWER\_FULL) for use by the user sketch. Power Shields may optionally use USB power when available.

# Specification #

Below is the formal specification for Mosquino Power shields. If you are designing one, the easiest way is to download the [Power Shield Template from the Downloads section](http://code.google.com/p/mosquino/downloads/list?q=template).

## Dimensions and Pinout ##

![http://tim.cexx.org/projects/mosquino/docs/pwr-shield-dims.png](http://tim.cexx.org/projects/mosquino/docs/pwr-shield-dims.png)

You can also download this in [PDF form](http://tim.cexx.org/projects/mosquino/docs/pwr-shield-dims.pdf).

The dimensions above are shown in inches. Like the I/O Shield headers, all pins are aligned on a 0.100" grid.

**Pin Functions:**
Top row
| Name  | Type/Direction | Description |
|:------|:---------------|:------------|
| GND | Pwr | System ground |
| VIN- | Pwr | Connected to "-" mainboard power entry terminal |
| VUNREG | Pwr | Unregulated (0.0 ~ 15V) output voltage to mainboard |
| VIN+ | Pwr | Connected to "+" mainboard power entry terminal |
| VREG | Pwr | Regulated (1.8 ~ 3.3) output voltage to mainboard |

Bottom row
| Name  | Type/Direction | Description |
|:------|:---------------|:------------|
| PFULL# (Power\_Full) |  Output (open-drain, active low) | Power Shield may pull LOW to indicate full/excess or "unlimited" power available |
| VCC\_UNSW | Pwr | If shield provides unregulated power, tie to VREGOUT (note 1) |
| VREGOUT | Pwr | If shield provides unregulated power, tie to VCC\_UNSW |
| VUSB | Pwr | USB power input to shield, when available. This might be used for e.g. battery charging. |
| PGOOD | Output (open-drain, active low) | Power Shield (regulated output) or mainboard (unregulated output) may pull LOW to indicate input power is above minimum operating voltage |

Note 1: The mainboard has an onboard PGOOD threshold circuit for unregulated input, and will assert PGOOD when the input is sufficiently above the regulator's dropout voltage (and the undervoltage lockout's trip voltage). The mainboard will assert PFULL whenever USB power is available. As these signals are open-drain on the mainboard, each will be asserted if pulled low by either the mainboard or the shield (or both).

Note 2: Power Shields which provide unregulated power (to be passed through the mainboard's onboard regulator) should connect the regulator output directly to the mainboard's power switch by tying VREGOUT and VCC\_UNSW together internally. Alternate possible uses: Monitor pin for Vcc, upstream of the mainboard's power switch. This might be used to tap into the mainboard's regulator, or trickle-charge a storage element, regardless of the state of the mainboard's power switch.

## Designing a Power Shield ##

Use VIN+/VIN- (mainboard header) or a connector on your shield for input power. Route output to VREG (if regulated - 1.8 ~ 3.3V) or VUNREG (if unregulated - 0 ~ 13V), as appropriate.

VUSB can be used for recharging when a computer is present.

PGOOD# and/or PFULL#, if supplied, must be active-low in a Wired-OR configuration (e.g. use an open-drain connection to pull down PGOOD#/PFULL# when asserted). These signals are weakly pulled up on the mainboard; leave floating when not asserted.

**If the Power Shields provides unregulated voltage (VUNREG)**

  * Jumper VREGOUT to VCC\_UNSW
  * PGOOD# and undervoltage lockout supplied by mainboard.

**If the Power Shield provides regulated voltage (VREG)**

  * Undervoltage lockout (load disconnect) highly recommended for low-current or slowly-rising power supplies.

### "Null" shields (direct passthru of regulated or unregulated voltage) ###

If no special conditioning of the input is required, a very simple passthrough shield can be made with perfboard, or simply by plugging a couple jumper wires into the shield header.

**Passthrough for regulated voltage (1.8 ~ 3.3V)**

  * Connect VIN- to GND
  * Connect VIN+ to VREG

**Passthrough for unregulated voltage**

  * Connect VIN- to GND
  * Connect VIN+ to VUNREG
  * Connect VREGOUT to VCC\_UNSW

**Passthrough for USB power (connected to PC)**

Power shield interface is automatically bypassed; nothing special needs to be done.

## Advanced topics: Stackable Power Shields ##

Ordinarily, a single Power Shield is plugged directly into the mainboard. However, it's also possible to make a Power Shield stackable such that another can stack on top of it. The most common reason to do this is to provide a rechargeable battery (or other storage device) circuit that receives its power from an ordinary Power Shield. One or more [examples of stackable Power Shields](http://code.google.com/p/mosquino/downloads/list?q=label:stackable) are available in the Downloads section.

In this case the board connections would consist of the 2 rows of bottom contacts (for the mainboard Power Shield socket), plus 2 rows of top contacts (for the upper shield to stack onto), effectively duplicating the mainboard socket. In general, most signals not actively replaced by the interposed power shield should be passed through to the top one.

Due to the wide variety of possible goals of creating a stacking shield, it's not possible to give a definitive connection diagram. However, for a typical application in which conditioned power from a standard Power Shield (top) charges a battery (middle board) which in turn powers the Mosquino mainboard (bottom), you would probably have:

  * VIN+, VIN-, VUSB passed directly through to the top shield. (VUSB might be tapped off for charging.) In nearly all cases, VCC\_UNSW and VREGOUT should likewise be passed through from the mainboard to the top shield.
  * For battery chargers expecting unregulated power, VUNREG from the top shield is wired to the charge circuit's input, and the charge circuit's output wired to the mainboard's VREG or VUNREG input, as appropriate. For chargers expecting regulated power, VREG from the top shield feeds the charger, and again, the charger's output wired to VREG / VUNREG as appropriate. For more complex configurations (e.g. accepting or providing both types), I expect you know what you're doing and don't need to read this.
  * The PGOOD# and PFULL# signals from the top shield may be passed through to the mainboard untouched, augmented (wired-OR) by the charger, or replaced by the charger, as appropriate. In general the charger will probably have a better idea than the top shield of when the battery is full or nearly depleted, so it should generate these signals (replacing those of the top shield) if practical.