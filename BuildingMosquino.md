IMPORTANT: The roll-your-own process is not for beginners. This page assumes that you know how to solder small-pitch SMT parts (or are a very fast learner), order circuit boards, source parts from a Bill Of Materials (possibly including find suitable replacements if one is out of stock)... and potentially read schematics and troubleshoot if something goes wrong.


This guide assumes you have already downloaded and unzipped the files for the desired board(s) from the Downloads section.


# Get Organized #
The cost of printed circuit board (PCB) manufacturing has come down quite a bit even in the last decade, but it is still not as cheap as you'd probably like. Expect to pay between $20 and $100+ for blank boards, depedning how many copies & shields, etc. you want, and how long you're prepared to wait for them. If you have a local hackerspace or other group infested with us Arduino nerds, organizing a group order can make things more tolerable.

Even if you are ordering just for yourself, deciding which boards/parts you need and ordering them all at once can save you some cash.

For reference, here are the downloads for...

[Current Mainboard](http://code.google.com/p/mosquino/downloads/list?q=label:PCB+label:mainboard)

[I/O Shields](http://code.google.com/p/mosquino/downloads/list?q=label:PCB+label:shield)

[Power Shields](http://code.google.com/p/mosquino/downloads/list?q=label:PCB+label:pwr)

# Get Parts #
You could order the parts and boards in any order, but I strongly recommend ordering (or at least checking the availability of) parts first, to avoid any surprises with regard to availability. Although I've tried to select widely available parts and standard footprints (compatible part available from multiple manufacturers) wherever possible, there are some unavoidable single-source parts.

The parts list (Bill Of Materials, BOM) for each board is in a spreadsheet file (.xls) included in each design's zip file. These can be opened with spreadsheet software such as MS-Office or OpenOffice.org/LibreOffice, etc. If using newer versions of MS-Office, you can safely ignore warnings to the effect of "file contents do not match extension" (the script used to generate them actually writes out an HTML-like format understood by most spreadsheet programs).

For each component, two part numbers are provided: the manufacturer's, and a vendor part number. The Vendor part numbers are for [Digi-key](http://www.digikey.com) unless otherwise noted. Using these is probably easiest, however, if something is out of stock, or there is no local distribution in your country and the shipping would be ridiculous, the manufacturer's part number can hopefully be used to source it from a local distributor.


NOTES:
  * For parts expensive enough to care about (MOSFETs, etc.; not resistors) I have tried to use the same part/# across the mainboard and shields when possible, minimizing the total # of part numbers to deal with and maybe helping reach price breaks for higher quantities.
  * A fractional quantity (e.g. 0.333) for a component indicates a parts which comes as a larger unit than you need for the board - e.g. breakaway headers. For these, tally them up for the board(s) you are building and round up. It's a good idea to get a couple extra, because you might end up with leftover ends that are too short to use easily, and these parts can occasionally break at inopportune places.

# Get Boards #

I will assume that if you plan to etch/mill your own boards, you don't need to be reading this guide. For everyone else, here are a handful of hobbyist-friendly PCB services I know of. This is not meant to be an exhaustive list.

[OSHPark](http://oshpark.com/) - Formerly known as Laen's PCB service (DorkbotPDX), this is the source of the sweet purple PCBs you see in the pictures. It's cheap, easy, and I recommend it highly, especially for someone who is new to having PCBs manufactured. At the time of this writing, you can simply send the .brd file and not have to muck about with Gerber layer files at all. The boards are fabricated in the USA.

[BatchPCB](http://batchpcb.com) - A similar, inexpensive batch service that grew out of Sparkfun.com. They use Gold Phoenix PCB in China for the actual fabrication, then ship individual orders out from Boulder, CO.

[Gold Phoenix](http://www.goldphoenixpcb.com/) - The aforementioned full-panel service. Good prices on larger hobby quantities, but the order and payment process can be a bit bamboozling for first time users. Quantities start at one panel (100 or 155cm^2), but everything up to full production quantities and even assembly are supported. I strongly recommend springing for 100% Electrical Testing on boards from here as I've had a few duds otherwise.

Some other hobbyist-friendly board fabricators I have heard of (but have not yet used personally) are:

[Seeed Studio](http://www.seeedstudio.com) (click on 'Services' and then 'Fusion PCB Service')

[ITead Studio](http://imall.iteadstudio.com/)

I have heard some rumors that these are actually the same company.

Again - this is not meant to be an exhaustive list, just a few places I know of that work well in hobbyist quantities. Note that PCB houses generally etch large PCB panels at a time; most of the services above provide prototype-quantity service by batching multiple customers' PCB designs onto a panel and running it when it fills up. Therefore, your leadtime may vary a bit depending how many other orders they are receiving.

# Assembly #

I tried to stick with fairly solderable parts overall (generally no smaller than 0805 / SOIC packages), but there are exceptions. On the Mosquino mainboard, these are the FTDI USB-to-serial converter (FT232), and the mini-USB plug. If you are soldering by hand, for this reason I recommend doing these two parts first. That way, if by any chance the board is damaged while soldering these unfriendly parts and has to be scrapped, you don't lose an entire board's worth of parts as well. Once those two are on, proceed generally from smallest (resistors, caps) to largest (ICs, then sockets, headers, etc.) to minimize stuff getting in your way. Be sure to place the crystal oscillator **before** soldering in the CPU socket, as the crystal actually sits underneath the socket and will be difficult to get to if done the other way around.

Breakaway headers (as the name implies) can be literally broken to the desired number of pins, but cutting them at the desired point with wire cutters (or some other suitable cutting instrument) is much more reliable. Male headers can be cut between pins reliably. For female headers, I prefer to cut in the middle of a "pin" (hole), where the plastic is thinnest, sacrificing that hole in the process. This avoids the header breaking apart or cutting uncleanly, as tends to happen if trying to cut between holes. Discard the metal contact that is liberated in the process; any remaining bits of plastic from the "hole" can then be trimmed off, if desired. (If none of that made any sense, Make / Adafruit have a nice [video](http://blog.makezine.com/2009/10/09/how-to-cut-header-pins/) showing the use of diagonal cutters to cut male and female headers to size.)

# Smoke Test #

If you have not already done so, you should install the [Arduino IDE](http://arduino.cc/) and the appropriate [Mosquino core](http://code.google.com/p/mosquino/downloads/list?can=2&q=label%3Acore). For most users, the newest (currently 1.x) is recommended. See GettingStarted for more info.

After careful visual inspection for solder bridges, backward-installed parts, etc., plug the Mosquino mainboard into your PC. You do not need any power shields or anything else attached. If all goes well, the FTDI USB-to-serial interface will be detected, and a new serial port (COMx, for Windows users) will be installed. Needless to say, you should not experience smoke, fire, or any other significant heat emanating from the board. (Actually, if there is a serious error in assembly, the polyfuse near the mini-USB port should trip and protect your board and PC from meltdown. It will self-reset after the board is unplugged for a few minutes, and any faults corrected of course.)

# Programming #

When the mainboard has been completely assembled, the Arduino-compatible Mosquino bootloader must be loaded onto the AVR for the first time. This requires external hardware. If you already have a working Arduino or compatible (including Mosquino), an AVR programmer, or anything else that can be pressed into service as one (e.g. a [Raspberry Pi board](http://hackaday.com/2012/08/20/raspberry-pi-as-an-avr-programmer/)), you're good to go. If not, AVRISP knockoffs can be bought off the internet cheaply, or it may be possible to borrow this service from a friend or local hackerspace with an Arduino or two hanging around.

This is not meant to be an exhaustive manual for every possible thing-that-can-be-used-as-an-AVR-programmer, but here is a quick example using an Arduino as a programmer.

Basically, follow the instructions on the official [Arduino as ISP](http://arduino.cc/en/Tutorial/ArduinoISP) page. In step 3, select your Arduino (NOT Mosquino, yet!) board in Tools -> Board, plug in your Arduino board (again, NOT Mosquino!) to the PC, and upload the sketch. In step 6, select Tools -> Board -> Mosquino, but leave the Arduino plugged in (the Mosquino board never gets plugged into the PC during this process.)

For wiring, connect wires between the Arduino and Mosquino board as follows. IMPORTANT: Do not install any power or I/O shields, as they may not be 5V-tolerant:

| Arduino Pin | Mosquino Pin |
|:------------|:-------------|
| 10 | RESET (female header near the reset button) |
| 11 | MOSI (pin 14) |
| 12 | MISO (pin 13) |
| 13 | SCK (pin 12) |
| +5V | Vcc (available on Bus Block) |
| Gnd | Gnd (available on Bus Block) |


## Common Bootloading Problems / Tips ##

Assuming the hardware is built correctly, this should "just work". However, there are a few problems that can trip up some users.

  * "Yikes! Invalid device signature" message: the ATMega processor has a 3-byte identifying code built in that identifies the vendor and part number - this message means the programmer software couldn't read the code, or it does not match the expected value. If the Arduino IDE does not show the actual signature returned, try selecting the "Show verbose output..." checkboxes in preferences (1.01 and later). A code of all zeros indicates it could not be read at all - doublecheck your wiring. If you get an actual number, most likely the host board (Arduino)'s auto-reset feature is goofing things up; the programming software on the PC is inadvertently resetting the Arduino by opening the port, and reading the host board's signature rather than the target board. For example:

```
avrdude: Device signature = 0x1e9406
avrdude: Expected signature for ATMEGA644P is 1E 96 0A
         Double check chip, or use -F to override this check.
```

If you are affected by this, make sure to temporarily defeat the auto-reset feature (the ArduinoISP page recommends placing a >=10uF capacitor between the host board's RESET and ground pins). There is a [more complete list](http://www.arduino.cc/playground/Main/DisablingAutoResetOnSerialConnection) of options for various boards.

  * "protocol error, expect=0x14, resp=0x64" or similar on Arduino 1.0:
Long story, but [this problem occurs due to](http://arduino.cc/forum/index.php/topic,82451.msg622247.html) an incompatibility between the Arduino 1.x core and versions of the ArduinoISP sketch that shipped with it. This may or may not be fixed in the shipping versions by the time you read this. For now, the easiest solution is to use and older version (Arduino 0023 or earlier) to run the ArduinoISP / Burn Bootloader process.