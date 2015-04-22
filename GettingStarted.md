This page assumes you already have working Mosquino hardware. If not, see BuildingMosquino.

# Software #

## Arduino IDE ##

Mosquino is derived from the open-source [Arduino](http://arduino.cc/) project and is compatible with the official Arduino IDE. If you don't already have it, download and install the current version of the [Arduino IDE](http://arduino.cc/en/Main/Software).

## Mosquino Support ##

### Short version (advanced users) ###
Unzip the latest [Mosquino core](http://code.google.com/p/mosquino/downloads/list?can=2&q=label:core) into your Arduino sketch folder's 'hardware' folder (if one doesn't exist, create it). Unzip any desired [Mosquino libraries](http://code.google.com/p/mosquino/downloads/list?can=2&q=label:library) to the 'libraries' folder in the same location.

### Longer version ###

**Install Board Support (core):**
To add support for the Mosquino board, download the newest [Mosquino core](http://code.google.com/p/mosquino/downloads/list?can=2&q=label:core). The current core is based on the Arduino 1.0 core. (There is also a version based on the older 0022 core available for backward-compatibility with old Arduino libraries and sketches. This version is no longer updated and should not be needed by most users.)

To install the core, find the location where Arduino stores user sketches on your platform. On Windows this is usually a folder named 'Arduino' inside 'My Documents'. This is usually something like _C:\Documents and Settings\yourname\My Documents\Arduino\_ (Win2k/XP) or _C:\Users\yourname\My Documents\Arduino\_ (Windows Vista/7+) or _/home/username/sketchbook_ (Linux)

Check if there is a folder named 'hardware' in this location. If not, create one. Finally, unzip the contents of the Mosquino core file you downloaded (a folder named 'Mosquino' or similar) into the 'hardware' folder. You should now have a My Documents\Arduino\hardware\Mosquino folder with additional folders and files ('bootloaders', 'cores', etc.) inside of it.

If the Arduino IDE is currently running, close and re-open it so that Mosquino appears in the list of supported boards.

**Install Libraries:**
Likewise, download any needed [Mosquino libraries](http://code.google.com/p/mosquino/downloads/list?can=2&q=label:library). The MosquinoLibraries Wiki page has more information about the libraries and what they do. At minimum, most users will want to install the DS1337 library to use the onboard realtime clock.



# Your First Mosquino Program #

Open the Arduino IDE and select the Mosquino board in Tools -> Board. Select the COM port it is attached to in Tools -> Serial Port.

## Hello World ##
In the Arduino IDE, open the example called 'Blink' in Examples -> 01.Basic -> Blink . This is a very simple sketch that is mainly there to verify your hardware and software is working.

The Mosquino board uses a different pin number for its onboard LED than a standard Arduino board (pin 29). In the example code, change the LED pin as follows:

~~// Pin 13 has an LED connected on most Arduino boards.~~<br>
<del>// give it a name:</del><br>
<del>int led = 13;</del><br>

int led = PIN_LED;<br>
<br>
<br>
(See MosquinoReference for a list of special-function pins and symbolic names you can use in your sketches.)<br>
<br>
Now upload the sketch using the 'Upload' button (rightward-facing arrow). If all goes well, the Mosquino board will reset (red LED will blink up to a few times), the Rx/Tx LEDs will blink briefly as the compiled sketch is uploaded, then the red LED should begin blinking on and off once per second. If so, congrats, everything is working!<br>
<br>
<h2>Sleep Test</h2>
The 'sleeptest' example shows the basics of putting the ATMega into sleep mode. This sketch disables all known standby power leaks (Brown Out Detect, ADC voltage references) and puts the CPU into its lowest power sleep mode, allowing for baseline current consumption measurements. If you have a multimeter that can measure low currents (uA), if everything is working correctly you should see current consumption on the order of 1uA (3.3V) when running from a pre-regulated power source (VREG), and a handful uf uA (6-12uA @ 4-10V) when operating from an unregulated source (VUNREG).<br>
<br>
NOTES:<br>
<br>
If measuring the board's current consumption, be aware that many multimeters cannot deliver the board's turn-on surge or active current when set on the uA measurement scale. To ensure the board starts up reliably, start on a larger scale (e.g. mA) and switch to the lower scale after the sketch puts the CPU in sleep mode. Otherwise the board may draw excessive current without fully starting up (VREG), or cycle rapidly on and off as the UVLO circuit trips (VUNREG).<br>
<br>
This sketch does not configure any I/O pins (the safest option in case external hardware is attached). Thus you could see excessive current consumption due to 'floating' pins. See <a href='http://code.google.com/p/mosquino/wiki/MosquinoTips#Floating_Pins'>Floating Pins</a> for more about this. If you are seeing excessive current draw, you can probably correct it by enabling the software pull-up resistors for unconnected pins or setting these pins as outputs.<br>
<br>
<pre><code>/*<br>
  Sleeptest<br>
  Put the AVR all the way to sleep so we can measure board current consumption.<br>
 */<br>
<br>
// Include AVR power and sleep headers. Names of AVR sleep modes such as SLEEP_MODE_PWR_DOWN are defined here.<br>
#include &lt;avr/power.h&gt;<br>
#include &lt;avr/sleep.h&gt;<br>
<br>
<br>
void setup()   {                <br>
<br>
  <br>
  // Disable the ADC by clearing the ADEN (A/D Enable) bit in the ADCSRA (ADC Status Register A) register.<br>
  // This will also disable any bandgap reference in use by the ADC, provided nothing else is using it (save ~90uA).<br>
<br>
  ADCSRA &amp;= ~(_BV(ADEN));<br>
 <br>
  // kill WDT if running<br>
  //MCUSR = 0;<br>
  //WDTCSR |= _BV(WDCE) | _BV(WDE);<br>
  //WDTCSR = 0;<br>
<br>
}<br>
<br>
<br>
void loop()                     <br>
{<br>
  // The following 2 lines will put the CPU into sleep mode. Since we have not configured any interrupt sources,<br>
  // it will remain sleeping until physically reset or powercycled.<br>
  <br>
  // To sleep, set one of the available modes (see http://code.google.com/p/mosquino/wiki/MosquinoReference#From_AVR-GCC),<br>
  // then use sleep() to actually enter sleep mode.<br>
<br>
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // SLEEP_MODE_PWR_DOWN is the deepest sleep mode.<br>
  sleep();<br>
<br>
  // The following code should not execute. Its purpose is to verify that the CPU entered sleep mode<br>
  // above and was not somehow awakened. In other words, the LED should stay OFF!<br>
  <br>
  // If you now measure the board's current consumption with a multimeter, you should read between 1 ~ a few uA at room temperature, <br>
  // depending on the power source (VREG or VUNREG) and input voltage in use.<br>
  <br>
  while(1)<br>
  {<br>
      digitalWrite(PIN_LED, HIGH);<br>
      digitalWrite(PIN_LED, LOW);<br>
  }<br>
}<br>
</code></pre>

<h2>Sleep & Wakeup Using The RTC</h2>

A more comprehensive example of using the onboard Realtime Clock is available in the example that comes with the <a href='http://code.google.com/p/mosquino/wiki/MosquinoLibraries#Standard_Libraries'>DS1337 RTC library</a>. Below is a simple sketch showing doing something (blinking the LED) at a 5-second interval, sleeping between activity and using the RTC alarm to wakeup.<br>
<pre><code>#include &lt;DS1337.h&gt;<br>
#include &lt;Wire.h&gt; // Needed by RTC library<br>
<br>
/*<br>
  DS1337 RTC Sleep-Wake Example<br>
<br>
  This sketch shows a simple example of using the RTC instead of delay loops to do something once per time interval,<br>
  sleeping in between.<br>
<br>
 */<br>
<br>
int ledPin =  PIN_LED;<br>
<br>
// Create an instance of the RTC class. The library handles setting up the I2C pins used by the RTC.<br>
DS1337 RTC = DS1337();<br>
<br>
void setup()   {                <br>
<br>
  pinMode(ledPin, OUTPUT);    <br>
  // ensure LED is off<br>
  digitalWrite(ledPin, LOW);<br>
 <br>
  //Serial.begin(9600);<br>
<br>
  RTC.start(); // ensure RTC oscillator is running, if not already<br>
  <br>
  if(!RTC.time_is_set()) // set a time, if none set already...<br>
  {<br>
    //Serial.println("Clock not set, setting to epoch (1/1/2000).");<br>
    // set initially to epoch<br>
    RTC.setSeconds(0);<br>
    RTC.setMinutes(0);<br>
    RTC.setHours(0);<br>
    RTC.setDays(1);<br>
    RTC.setMonths(1);<br>
    RTC.setYears(2000);<br>
    RTC.writeTime();<br>
  }<br>
}<br>
<br>
<br>
void loop()                     <br>
{<br>
<br>
  // Blink the LED to show that we 'did something'.<br>
  // In reality you'd probably take a sensor reading, send a radio packet, or etc. here.<br>
  digitalWrite(ledPin, HIGH);<br>
  delay(100);<br>
  digitalWrite(ledPin, LOW);  <br>
  <br>
  // Use the RTC's alarm to enter deep sleep for 5 seconds before looping.<br>
  // You could use a software delay, e.g. delay(5000); to get a 5-second delay, <br>
  // but this saves a lot of power.<br>
  RTC.snooze(5);<br>
  <br>
}<br>
</code></pre>