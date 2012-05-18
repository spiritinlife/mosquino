#include <avr/pgmspace.h>
#include <SystemFont5x7.h>
#include <fixednums15x31.h>
#include <nvlcd.h>
#include <SPI.h>




/* Basic examples for the nvlcd library. nvlcd is a library to drive
nonvolatile displays, specifically Kent Displays' Inc.'s 128x32 cholesteric display.
Other display types (e-paper, etc.) may be added as they become commercially available.

This example assumes the following fonts are present:
SystemFont5x7
fixednums15x31

This library does not use a framebuffer (to save memory) or assume that read-modify-write
access to the display's memory is available. For this reason, text and
graphic display is done only on full 8-pixel vertical pages at a time, and
aligned on these vertical bounardies. The horizontal position of the text or
graphic can be anywhere on the display (does not need to be bounadry aligned).


20110813 / tgipson

*/





// image from sampleimage.c
extern const char sampleImage[] PROGMEM;

//tiny and simple image from RAM
const char smile[] = { 12, 16, 0xF0, 0x0C, 0xC2, 0x1A, 0x19, 0x01, 0x01, 0x19, 0x1A, 0xC2, 0x0C, 0xF0, 
     0x00, 0x03, 0x04, 0x05, 0x0A, 0x0A, 0x0A, 0x0A, 0x05, 0x04, 0x03, 0x00
};

#define K_CS 0
#define K_BUSY 1
#define K_D_C 2
#define K_RESET 3
#define K_SCLK SCLK
#define K_MOSI MOSI
#define K_VCC -1
//#define K_GND 22

// Change to a GPIO pin with an LED, if available. For Mosquino this is pin 29.
int ledPin =  PIN_LED;



void setup()
{
    int i=0;
    Serial.begin(9600);
    
    pinMode(ledPin, OUTPUT);

    Serial.println("Starting");

    // this constructor is for hardware SPI, and no GPIO used as GND pin (pass -1)
    NVLCD lcd=NVLCD(K_CS, K_BUSY, K_D_C, K_RESET, K_VCC, -1);

  while(1)
  {
    // Set screen flipping mode (cycle thru all modes), turn on LCD and do some basic setup...

    digitalWrite(ledPin, HIGH);
    lcd.set_h_flip(i%2);
    lcd.set_invert(i%2);
    lcd.set_v_flip((i>>1) % 2);
    //Serial.println("turn on");
    lcd.on();
    //Serial.println("reset");
    lcd.reset();
    //Serial.println("clearing");
    lcd.clear();
    
    lcd.set_border(0x01); // Set the display's border pixels to bright


    // Display a fullscreen image from Flash

    lcd.pixmap_P(0, 0, sampleImage);

    lcd.commit();
    digitalWrite(ledPin, LOW);

    delay(4000);
    
    Serial.println("set font");
    lcd.set_font(SystemFont5x7);    


    // Various string printing
    digitalWrite(ledPin, HIGH);
    lcd.clear();
    lcd.set_cursor(0,0);
    lcd.print_p("Print from ROM");
    lcd.commit();
    digitalWrite(ledPin, LOW);

    delay(4000);

    digitalWrite(ledPin, HIGH);
    lcd.set_cursor(16,8);
    lcd.print("Print from RAM");
    lcd.pixmap(110, 8, smile); // also from RAM
    lcd.commit();
    digitalWrite(ledPin, LOW);

    delay(4000);    

    digitalWrite(ledPin, HIGH);
    lcd.set_cursor(1,24);
    lcd.print_p("Partial screen update");
    lcd.commit(0, 24, 0, 32);  // commit only the line we just wrote
    digitalWrite(ledPin, LOW);

    delay(4000);

    lcd.set_font(fixednums15x31);    // BIG numeric fonts

    digitalWrite(ledPin, HIGH);
    lcd.clear();
    lcd.set_cursor(0,0);
    lcd.print_p("12345678");
    lcd.commit();
    digitalWrite(ledPin, LOW);

    
    lcd.off();

    delay(5000);
    i++;

  }
    
}


void loop()
{

}


