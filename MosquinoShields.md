# Introduction #

Mosquino can be extended by the use of modular stackable daughterboards, known as shields. In a slight divergence from the Arduino you may know and love, rather than a single shield covering the entire board, the Mosquino shield interface is split into 3 identical "slots", allowing multiple shields to be installed at once without stacking. A single shield may span multiple slots if needed (e.g. for more I/O), however it is expected the vast majority of typical sensors and other devices will fit in a single slot. The shield slot's pins are entirely aligned on .100" spacing, so that DIY shields can easily be made on perfboard or breadboards.

Each shield slot provides access to up to 8 I/O (4 guaranteed, up to 4 may be shared with other functions) and a shared signal block. The shared signal block consists of the shared SPI and I2C buses, two INTerrupt lines (or secondary RS232 port), Power and Ground, AREF, charging voltage (up to 15V) and the POWER\_GOOD / POWER\_FULL power management signals.

![http://tim.cexx.org/projects/mosquino/shields-full.jpg](http://tim.cexx.org/projects/mosquino/shields-full.jpg) <br>
Mosquino board loaded with shields<br>
<br>
<h1>Rationale</h1>

Shields "usually" do one specific thing. In modern times, that thing is usually accomplished by a single small IC and fairly paltry number (if any) of discrete components. So it is usually unnecessary (and wasteful) to have a full-size board.<br>
<br>
Sensor ICs are increasingly moving to standard low-pin-count digital buses, namely SPI and I2C. We can make the most of them by making it easy to access and share these buses.<br>
<br>
By giving every shield its own set of pins rather than stacking, we avoid the frequent headache (for the user) of trying to determine whether a stack of 2 or more shields will have conflicting pins, and (for the developer) trying to minimize such conflicts by guessing and dancing around the pinouts of existing shields according to their popularity.<br>
<br>
Many popular sensors and other toys have no readymade shields available or even designed for them (yet). Adherence to a 100-mil grid allows shields to be easily made by the user with low-cost perfboard, without having to make any custom boards.<br>
<br>
<h1>Specification</h1>

Below is the formal specification for Mosquino I/O shields. If you are designing one, the easiest way is to download the <a href='http://code.google.com/p/mosquino/downloads/list?q=template'>I/O Shield Template from the Downloads section</a>.<br>
<br>
<h2>Dimensions</h2>

<img src='http://tim.cexx.org/projects/mosquino/docs/shield-dims.png' />

You can also download this in <a href='http://tim.cexx.org/projects/mosquino/docs/shield-dims.pdf'>PDF form</a>.<br>
<br>
<h2>Pin Usage</h2>

I/O shield pins come in two sections, on opposite ends of the board. On the side with the triangular indicator is the "Bus Block", and on the other side is GPIO. Due to the nature of the ATmega, a fair number of these pins have multiple functions.<br>
<br>
<h3>Bus Block</h3>

The us Block consists mainly of common signals (Vdd, GND, etc.) and bus interfaces (SPI, I2C) that can be safely shared between multiple shields. The Bus Block pins are:<br>
<br>
<table><thead><th> Vcc </th><th> SCK </th><th> MOSI </th><th> MISO </th><th> SS </th><th> Rx1 (INT0) </th><th> Tx1 (INT1) </th><th> Gnd </th></thead><tbody>
<tr><td> Vcc </td><td> AREF </td><td> VCOM </td><td> PGOOD </td><td> PFULL </td><td> SDA </td><td> SCL </td><td> Gnd </td></tr></tbody></table>

Note that the Rx1 and Tx1 pins (secondary RS232 Serial port) are also the INT0 and INT1 hardware interrupt pins, respectively. Each pin can be used for one function or the other at a time, but not both.<br>
<br>
Also note that the I2C pins (SDA, SCL) are also used by the onboard Real-Time Clock IC. To ensure correct operation of the RTC and your shield, please ensure only I2C-compliant devices are attached to these pins (i.e. don't try to use them as GPIO).<br>
<br>
<h3>GPIO</h3>

The top row of each I/O shield consists of GPIO pins. For the purpose of this discussion, consider the pins on the GPIO row as being numbered (counterclockwise, like the mainboard). In the image above, GPIO Pin 1 would be at the top right.<br>
<br>
Each I/O Shield slot's GPIO header consists of 8 pins. A minimum of 4 of these pins (pins 1-4) are guaranteed to be usable without conflict with any other shield. Depending on the slot, 6 or all 8 pins may be usable without conflict. That is to say, the pinout has been organized so that pins which could conflict with another shield will <b>always</b> be on the 5-8 side of the header. Such a pin could conflict because it serves multiple functions on the ATmega (for example, a line which can be used as GPIO or the SPI clock line), and so is connected to a pin on both the GPIO header and the Bus Block.<br>
<br>
Pin conflicts are a somewhat unavoidable nature of the beast. But at least in this case, the user has the option to physically swap shields around in slots in a way that avoids a conflict.<br>
<br>
The table below lists the alternate function (if any) of each GPIO pin on every slot. Note that each of the pin names below matches the name of a pin on the Bus Block, because they are the same signal. Shields will (or should) use the pin on the GPIO header if using the pin as GPIO, and use the corresponding pin on the Bus Block if using the pin's alternate (bus) function.<br>
<br>
Pins with Alternate Functions<br>
<table><thead><th> </th><th> Slot 1 </th><th> Slot 2 </th><th> Slot 3</th></thead><tbody>
<tr><td> Pin 8 </td><td> Rx1, INT0 </td><td> SS </td><td>  </td></tr>
<tr><td> Pin 7 </td><td> Tx1, INT1 </td><td> SCK </td><td>  </td></tr>
<tr><td> Pin 6 </td><td>  </td><td> MISO </td><td>  </td></tr>
<tr><td> Pin 5 </td><td>  </td><td> MOSI </td><td>  </td></tr>
<tr><td> Pin 4 </td><td>  </td><td>  </td><td>  </td></tr>
<tr><td> Pin 3 </td><td>  </td><td>  </td><td>  </td></tr>
<tr><td> Pin 2 </td><td>  </td><td>  </td><td>  </td></tr>
<tr><td> Pin 1 </td><td>  </td><td>  </td><td>  </td></tr></tbody></table>

<code>*</code>No pin on slot 3 can conflict with another shield. However, all of the pins on slot 3 can be used as either digital I/O or an analog input.<br>
<br>
<h2>Markings</h2>

The marking requirements are minimal and consist of only 2 markings:<br>
<br>
<ul><li>Polarity indication: A triangular mark (matching the triangular mark on the mainboard for each shield slot) indicates the correct orientation of the shield when installed.</li></ul>

<ul><li>Slot Compatibility indication: A circled (1), (2) and/or (3) on the shield (again, matching the same marking in the respective numbered slot) indicates that it can be used in that slot number without any chance of pin conflict with itself or another shield. (The shield may still be usable in slots it is not marked for, but the user will have to check whether or not the possibly-conflicting pins are used by another shield they have installed)</li></ul>

In addition, labeling the used header pins with the appropriate signal name is strongly recommended, if space permits.<br>
<br>
<h2>Designing a Custom Shield</h2>

If you use the EAGLE PCB editor (or compatible), the easiest way to start is to download the I/O Shield template above. Otherwise, please refer to the above dimensions.<br>
<br>
<b>If your shield uses analog input pins:</b>

Wire up your analog and GPIO pins (if any) to the GPIO header. They do not <i>have</i> to be in a particular order, but it makes sense to group the analog pins together and the digital pins together, and start with the analog pins so that they start with pin # A0 (a.k.a. pin 16). The resulting shield should only be used in slot 3, which has the analog pins. Place the slot (3) symbol on the shield and remove any others (unless the analog feature is optional).<br>
<br>
<b>If your shield uses only digital pins (including PWM):</b>

Wire up any needed GPIO pins to the GPIO (top) header, starting at Pin 1. If there are 4 or less, congratulations, your shield can probably be used in any slot without a care in the world. If there are more, try to put any mandatory or output signals on pins 1-4, and save 5+ for auxiliary/optional inputs, if possible. This will maximize flexibility of which slots your shield can be used in.<br>
<br>
If GPIO pins 5-6 are used: The shield may conflict with a shield using the SPI pins when placed in Slot 2 (see table above).<br>
<br>
If GPIO pins 7-8 are used: The shield may conflict with a shield using the SPI pins when placed in Slot 2, and may conflict with a shield using the Tx1/Rx1 pins (or INT0/INT1) when placed in Slot 1.<br>
<br>
<b>For all shields</b>

If your shield drives the PGOOD/PFULL/INT0/INT1 pins (or the I2C pins), remember that they are meant to be used <i>open-drain</i> and pulled up by an onboard pull-up resistor. To ensure coexistence (or at least the possibility of it) with other shields using these pins, your shield should never drive a voltage onto these pins. To drive a LOW onto these pins, pull them low. To drive a HIGH, float the pin. If you are using the Rx1/Tx1 pins for serial transmission, you (and the ATmega) are free to drive the pins, but they then cannot be used as interrupt pins by other shields.<br>
<br>
If your shield does not use the outer Bus Block row, it can be omitted and (if desired) the board dimension shrunk to just outside the inner one. Likewise, if your shield uses only the outer row, the inner row can be omitted.<br>
<br>
Adjust the slot compatibility markings (circled (1) (2) (3) symbol) to indicate which slots the shield can be used in without chance of conflicts.<br>
<br>
Ensure the triangular polarity marking is present and in the correct location.<br>
<br>
<h2>Misc. notes</h2>

Shields designed for Mosquino should be optimized for low power consumption, and should be able to be powered down or switched off when not actively in use. Many modern ICs have a sleep or standby mode that can be entered via pin or software control, if the shield uses these, nothing special has to be done. Otherwise, switching the shield's VCC connection through a P-channel MOSFET (tie the gate to an I/O pin) is a good way to provide user power control.<br>
<br>
Shields should draw an <b>absolute maximum</b> peak current of 200mA. In general, it should be much less. Shields should clearly indicate their peak and average current consumption (in the documentation or even right on the shield). <i>The Mosquino's voltage regulator is rated for 250mA; some of that will be used by the board itself. If your shield draws high current, this will limit the total number of shields that can be used on a given project, or the ability to use your shield with some power sources/shields.</i>

To avoid clearance issues with future boards, it is not recommended to place components on the bottom side of the board. If you must, try to keep them short and placed either between the Bus Block header rows or immediately above the Bus Block header rows. This area is least likely to contain tall components in current and future Mosquino boards.