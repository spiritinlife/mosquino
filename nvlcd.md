# Introduction #

This is the documentation of the **nvlcd** library for the Kent Displays  128x32 bistable graphics display. The library supports images and text/string display with fixed-width fonts, partial display updates and more.

This library currently supports only the 128x32 Kent cholesteric display, which uses a Solomon Systech display controller (part number unknown). Support for additional displays and nonvolatile display types (e.g. eInk) may be added as they become commercially available to hobbyists.

# Downloads #
[nvlcd Library](http://code.google.com/p/mosquino/downloads/list?q=label:nvlcd)

# Basics #

## Constructor for software (bitbang) serial access ##

`NVLCD(int8_t cs_pin, int8_t busy_pin, int8_t c_d_pin, int8_t reset_pin, int8_t pwr_pin, int8_t gnd_pin, int8_t sck_pin, int8_t mosi_pin)`

This is the full-size constructor which specifies every pin possibly used by the display, including a switchable power pin and SPI clock/data pins. Pin numbers should be the Arduino pin # for your given board. Enter '-1' for pins which are not applicable to your setup. 'pwr\_pin', 'gnd\_pin', 'sck\_pin' and 'mosi\_pin' are optional: if your display is powered directly from Vcc or another source (not a GPIO pin), enter "-1" for this pin. If no valid SPI pins are given, the library will attempt to use the hardware SPI port.

## Constructor for hardware SPI ##

`NVLCD(int8_t cs_pin, int8_t busy_pin, int8_t c_d_pin, int8_t reset_pin, int8_t pwr_pin, int8_t gnd_pin)`

This is a reduced constructor for hardware SPI users. 'pwr\_pin' and 'gnd\_pin' are optional: if your display is powered directly from Vcc or another source (not a GPIO pin), enter "-1" for this pin.

## Initialization functions ##

`set_font(Font_t font)`

Sets the font used by nvlcd for subsequent text display. `font` is the name of a GLCD-compatible font data array (technically a pointer to the start of the array). Any _fixed-width_ GLCD font can be used (variable-width fonts are not supported).

`reset()`

Resets the display by toggling its hardware reset line. It is recommended to call this function after applying power to the display.

`on()`

Applies power to the display controller (if the display is powered by a GPIO pin).

`off()`

Disconnects power to the display controller in order to save power between image writes (if the display is powered by a GPIO pin). Since these displays are bistable, the last image displayed will be retained indefinitely, even without power.

`set_temp_comp(int temperature)`

The cholesteric displays produce the best image when temperature compensated. Use this function to tell the library the current temperature in degrees Celsius, if known. The valid operating range of the 128x32 display is 0C to +50C. The displayed image may be dim or uneven if the temperature compensation value is substantially different from the actual temperature.

# Basic Settings #

`set_invert(byte invert)`

Set or clear the inverse video mode. A value of 0 (false) clears inverse mode, and a non-zero value (true) sets it. In the normal mode, the display is nominally cleared to its dark state, and text and graphics are shown in the light state. In the inverse video mode, the background is bright and the text/images are dark.

`set_border(byte border)`

Set or clear the border around the active image area. A 0 (false) value will set the border to the dark state, and a nonzero value (true) will set it to the bright state. This setting is independent of the inverse video setting.

`set_h_flip(byte h_flip)`

Enable or disable horizontal flipping. A 0 (false) value will display the image normally; a nonzero (true) value will mirror the video horizontally.

`set_v_flip(byte v_flip)`

Enable or disable vertical flipping. A 0 (false) value will display the image normally; a nonzero (true) value will mirror the video vertically.

Setting both H and V flip will have the effect of rotating the display by 180 degrees.

# Display Functions #

**Note: These functions will alter the image stored in the display RAM, but the changes won't visibly take effect until you call commit() to commit the new image to the screen.**

`clear()`

Clear the display.

`set_cursor(byte x, byte y)`

Set the text cursor position to x,y. Subsequent text writes will begin at this location. X controls the horizontal position and Y controls the vertical, with the origin (0,0) located at the top-left corner of the display.

_Note: In the current version (no framebuffer), the vertical cursor position is limited to multiples of 8 pixels. Other values will be rounded down to the nearest multiple of 8 pixels._

`home()`

Set the text cursor to (0,0).

`putch(byte c)`

Write a single ASCII character to the display from RAM. This character can be a character literal (e.g. 'a') or stored in a variable.

`putch_P(PGM_P pc)`

Write a single ASCII character to the display from Flash. This character must be a literal (e.g. 'a').

`puts(const char * s) `<br>
<code>print(const char * s) </code>

Write a string to the display from RAM. The 'print' variant implements the Arduino print(...) function and can be used interchangeably. E.g.:<br>
<blockquote><code>lcd.print("Hello!");</code><br>
<code>lcd.print(var, HEX);</code></blockquote>

<code>puts_P(PGM_P s)</code><br>
<code>print_p(PGM_P s)</code>

Write a string to the display from Flash.<br>
<br>
<code>pixmap(int x, int y, const char * pImg)</code>

Display an image (pixel map) from RAM. x and y are the starting position (top-left corner) of the image, and pImg is the name of the array holding the image data (technically, a pointer to the start of the array).<br>
<br>
<i>Note: In the current version (no framebuffer), the vertical cursor position is limited to multiples of 8 pixels. Other values will be rounded down to the nearest multiple of 8 pixels.</i>

<code>pixmap_P(int row, int col, PGM_P pImg)</code>

Display an image from Flash. Operation is the same as above.<br>
<br>
<code>commit()</code>

Commit the current display memory contents to the screen. This image will be retained (with or without power) until a new image is committed. This takes from 1/2 second up to several seconds depending on the temperature, and will be the slowest at low temperatures.<br>
<br>
<code>commit(int x1, int y1, int x2, int y2)</code>

Commit a partial display area to the screen. In cases where only a smaller portion of the image has changed, this can be used to save power over a full screen update. x1 and y1 are the top-left corner of the update area; x2, y2 are the bottom-right corner.<br>
<br>
<i>Note: In the current displays, partial updates commit full lines at a time; the x-values are ignored.</i>