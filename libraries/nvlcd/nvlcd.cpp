extern "C" {
	#include <avr/pgmspace.h>
}
#include <SPI.h>
#include "nvlcd.h"

// Underlying controller is assumed to be the Solomon Systech SSD1603 bistable display controller.
// Unfortunately, its full command set is secret until consumer hardware comes out which flexes all its functionality
// and its ribbon cable is reachable by probes...
// (or they actually publish a real datasheet ... don't hold yer breath :-)




// Temperature compensation data element definition.
typedef struct
{
  int8_t Temp;
  uint8_t VAClearPW;
  uint8_t VAIdle;
  uint8_t AAClearPW;
  uint8_t AAIdle;
  uint8_t DrivePW;
  uint8_t Voltage;
}
tcTableEntry;
#define TEMP_PTS (sizeof(tcTable)/sizeof(tcTable[0])) // # of table entries.
#define TEMP_MAX (tcTable[0].Temp+5) // Max update temp (C).




// Temp Comp Data
//
// White/Blue KLC-19.
//
static const tcTableEntry tcTable[] =
{
  //
  // Temperatures are in degrees Celsius. All other values (in hex) are
  // encoded for the display driver. Within the comments, the values
  // are durations in milliseconds, except for voltage.
  //
  // Temp VAClear VAIdle AAclear AAIdle Drive Voltage
  // ---- ------- ------ ------- ------ ----- -------
  // 45 50 2 20 6 10 24
  {
    45, 0x12, 0x05, 0x0D, 0x07, 0x09, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 40 60 2 25 6 12 24
  {
    40, 0x13, 0x05, 0x0E, 0x07, 0x0A, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 35 80 2 30 6 14 24
  {
    35, 0x14, 0x05, 0x0F, 0x07, 0x0B, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 30 100 2 40 6 20 24
  {
    30, 0x15, 0x05, 0x11, 0x07, 0x0D, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 25 100 2 40 6 20 24
  {
    25, 0x15, 0x05, 0x11, 0x07, 0x0D, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 20 150 2 50 6 25 24
  {
    20, 0x16, 0x05, 0x12, 0x07, 0x0E, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 15 150 2 60 6 35 24
  {
    15, 0x16, 0x05, 0x13, 0x07, 0x10, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 10 250 2 100 6 50 24
  {
    10, 0x18, 0x05, 0x15, 0x07, 0x12, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 5 350 4 150 10 80 24
  {
    5, 0x19, 0x06, 0x16, 0x09, 0x14, 0x40  }
  ,
  // ---- ------- ------ ------- ------ ----- -------
  // 0 500 6 200 18 100 24
  {
    0, 0x1A, 0x07, 0x17, 0x0C, 0x15, 0x40  }
  // ---- ------- ------ ------- ------ ----- -------
};




NVLCD::NVLCD(byte _cs_pin, byte _busy_pin, byte _c_d_pin, byte _reset_pin, byte _pwr_pin, byte _gnd_pin, byte _sck_pin, byte _mosi_pin)
{


    cs_pin = _cs_pin;
    busy_pin = _busy_pin;
    c_d_pin = _c_d_pin;
    reset_pin = _reset_pin;
    pwr_pin = _pwr_pin;
    gnd_pin = _gnd_pin;
    sck_pin = _sck_pin;
    mosi_pin = _mosi_pin;

    if(sck_pin > 127 || mosi_pin > 127) // if user enters nonvalid or '-1' value
    {
      // attempt to use SPI standard library
      spi_putc_type = SPI_PUTC_STD_SPI;
    }
    else
    {
      spi_putc_type = SPI_PUTC_BITBANG;
    }
    init();

    //pWriteFunc = NULL; //&lcd_bitbang;

}
NVLCD::NVLCD(byte _cs_pin, byte _busy_pin, byte _c_d_pin, byte _reset_pin, byte _pwr_pin, byte _gnd_pin)
{
    cs_pin = _cs_pin;
    busy_pin = _busy_pin;
    c_d_pin = _c_d_pin;
    reset_pin = _reset_pin;
    pwr_pin = _pwr_pin;
    gnd_pin = _gnd_pin;
    sck_pin = 0;
    mosi_pin = 0;

    // No SPI pins given for constructor, so attempt to use SPI standard library
    spi_putc_type = SPI_PUTC_STD_SPI;

    //pWriteFunc = _pWriteFunc;
    init();

}


/*------ Basic Functions -----*/

void NVLCD::set_font(Font_t _font)
{
   font = _font;

   if(font != NULL)
   {
        // retrieve important font parameters so they don't have to be fetched for each character written

        font_width = pgm_read_byte(&font[FONT_FIXED_WIDTH]);
        font_height = pgm_read_byte(&font[FONT_HEIGHT]);
        font_first_char = pgm_read_byte(&font[FONT_FIRST_CHAR]);

        //byte char_width_bytes = width; // 1 byte -> 1 col (8 px vertical)
        char_height_bytes = (font_height+7) / 8; // round up
   }
}


void NVLCD::reset(void)
{
  digitalWrite(reset_pin, LOW);  // assert \RST
  delay(2);  // delay 2ms...128x32 only requires ~20uS
  digitalWrite(reset_pin, HIGH); // deassert \RST
}

void NVLCD::init(void)
{

    init_pins();
    off();
    temp_comp_value = 22; // default temp. compensation value in degC. HACK: FIXME: Hack around with the Solomon chip
                          // to find & use its builtin temp. sensor(!)
    h_flip_value = 0;
    v_flip_value = 0;
    invert_value = 0x00;
    font = NULL;

    if(spi_putc_type == SPI_PUTC_STD_SPI)
    {
       //Check if SPI has already been initialized and begin() it if not.
       if(!(SPCR & _BV(SPE)))
       {
          // SPI not initialized yet; set it up for our display

          SPI.begin();
          SPI.setBitOrder(MSBFIRST);
          SPI.setDataMode(0);
          SPI.setClockDivider(SPI_CLOCK_DIV4);
       }
    }
}

void NVLCD::on(void)
{
  if(pwr_pin < 128)
     digitalWrite(pwr_pin, HIGH);
}
void NVLCD::off(void)
{
  if(pwr_pin < 128)
     digitalWrite(pwr_pin, LOW);
}

void NVLCD::set_temp_comp(int v)
{
  temp_comp_value = v;
}

void NVLCD::clear(void)
{
  uint8_t i;
  uint8_t j;
  home();
  set_seg_mapping();
  for (i=0; i<N_PAGES; i++)
  {
    set_page(i);
    set_col(0);
    lcd_mode_data();
    for(j=0; j<N_COLS; j++)
    {
      lcd_write(0x00 ^ invert_value);
    }
  }
  home();

}

void NVLCD::set_invert(byte v)
{
  if(v)
  {
    invert_value = 0xFF;
  }
  else
  {
    invert_value = 0x00;
  }
}

void NVLCD::set_border(byte)
{
  // Hunh...so it appears the 'border' lines are just ordinary pixels. Funk this :-)
}
void NVLCD::set_h_flip(byte v)
{
  // Horizontal display flipping (reverse SEG scanning direction)
  h_flip_value = v;
}
void NVLCD::set_v_flip(byte v)
{
  // Vertical display flipping (reverse COM scanning direction)
  v_flip_value = v;
}

void NVLCD::commit()
{
  // Creature comfort wrapper: commit entire valid DDRAM to screen
  commit(0, 0, N_ROWS, 0);
}

void NVLCD::commit(int x1, int y1, int x2, int y2)
{

// Commit DDRAM contents from row x1 to row x2 to physical glass
// NOTE: y1, y2 parameters are accepted in case future controllers can update rectangular areas (unlikely, but these folks never cease to amaze)
//       Currently these parameters are ignored (only full lines updated at a time)

// The following code blantantly filched from Kent Displays' datasheet.
// CHANGES: 1) Allow independent V and H flipping modes
//          2) fix DUMMY_ROWS (TODO)
//          3) wrap TC table lookup into image commit

// Function: DriveImage
// Purpose: Updates display with image data in driver RAM using
// supplied drive parameters.
// Inputs: firstRow - first physical display row to update
// numRows - number of rows to update
// VAClearPW - VA Clear Pulse width (driver encoding)
// VAIdle - VA Idle Duration (driver encoding)
// AAClearPW - AA Clear Pulse width (driver encoding)
// AAIdle - AA Idle Duration (driver encoding)
// DrivePW - Scan Pulse width (driver encoding)
// ClearV - VA/AA Clear Voltage (driver encoding)
// DriveV - Scan Voltage (driver encoding)
// VAClearRpt - Number of VA Clear Cycles
// VAIdleRpt - Number of VA Idles
// AAClearRpt - Number of AA Clear Cycles
// AAIdleRpt - Number of AA Idles
// DriveRpt - Number of Driver Cycles
// Bias - Drive Voltage Bias (driver encoding)
// Outputs: None.
// Notes: Full-screen updates include additional dummy rows (DUMMY_ROWS) to
// let last image row see some non-select rows.
// Driver must already be powered with image loaded.
///////////////////////////////////////////////////////////////////////////////
//void DriveImage(uint8_t firstRow, uint8_t numRows,
//uint8_t VAClearPW, uint8_t VAIdle,
//uint8_t AAClearPW, uint8_t AAIdle,
//uint8_t DrivePW, uint8_t ClearV, uint8_t DriveV,
//uint8_t VAClearRpt, uint8_t VAIdleRpt,
//uint8_t AAClearRpt, uint8_t AAIdleRpt,
//uint8_t DriveRpt,
//uint8_t Bias)

  byte DrivePW;
  byte DriveV;
  byte VAClearPW;
  byte VAIdle;
  byte AAClearPW;
  byte AAIdle;
  byte ClearV;

  uint8_t offset;
  uint8_t startLineCmd;

  uint8_t firstRow = x1;
  uint8_t numRows = x2 - x1;

  // Get /*and upload*/ current TC data

  if ((temp_comp_value >= tcTable[TEMP_PTS-1].Temp) && (temp_comp_value <= TEMP_MAX))
  {
    // Search TC table to find the drive parameters.
    for (int i = 0; i < TEMP_PTS; i++)
    {
      if ( temp_comp_value >= tcTable[i].Temp ) // Within range of table entry?
      {
        // Read parameters from table.
        DrivePW = tcTable[i].DrivePW;
        DriveV = tcTable[i].Voltage;
        VAClearPW = tcTable[i].VAClearPW;
        VAIdle = tcTable[i].VAIdle; // Stir time between VA & AA clear.
        AAClearPW = tcTable[i].AAClearPW;

        AAIdle = tcTable[i].AAIdle;
        ClearV = DriveV;

        break;
      }
    }
  }


  // Map first row to first driver COMMON.
  //if ((firstRow == 0) && (numRows == 32)) // Full screen?
  //{
    // Include top/bottom active frame in update area. Thus the image spans
    // 34 COM lines, COM15 to COM48.
   // firstRow = 15;
   // numRows = 34;
  //}
  //else // Partial screen.
  //{
    // First image row is on the COM line that is 16 inward from the edge
    // COM16 or COM47.
    //firstRow += 16;
    firstRow += 15;
 // }


  // Compute image location parameters. V flip (COM) is handled during drive; H flip (SEG) is handled during writing image data.
  if(v_flip_value)
  {
    offset = 0x40 - firstRow; // Location on display.
  }
  else
  {
    offset = firstRow + numRows; // Location on display.
    if (firstRow == 15) // Offset needs special treatment when we
      offset += DUMMY_ROWS; // do a full image in non-remapped mode.
  }
  startLineCmd = 0x40 + ((firstRow==15) ? 15 : 16); // Location in display RAM.

  // Initialization code. Mostly voodoo since there is no public controller datasheet.

  lcd_mode_cmd(); // switch to command mode

  lcd_write( 0xA3 ); // Enable band gap and other analog control.
  lcd_write( 0x18 );

  lcd_write( 0xF6 ); // Enable oscillator.
  lcd_write( 0x40 );
  lcd_write( 0xAE ); // Set auto charge pump threshold value.
  lcd_write( 0x00 );
  lcd_write( 0xA2 ); // Set Bias level
  lcd_write( Bias );


  // Set drive parameters.
  lcd_write( 0x80 );
  lcd_write( 0x00 );
  lcd_write( VAClearPW ); // VA Clear Duration
  lcd_write( VAIdle ); // VA Idle Duration. Stir time.
  lcd_write( AAClearPW ); // AA Clear Duration
  lcd_write( AAIdle ); // AA Idle Duration
  lcd_write( DrivePW ); // Drive Duration
  lcd_write( ClearV ); // Clear Voltage
  lcd_write( DriveV ); // Drive Voltage
  // Set dummy waveform for supply initialization.
  lcd_write( 0x93 );
  lcd_write( 0x00 ); // Skip VA Clear.
  lcd_write( 0x94 );
  lcd_write( 0x00 ); // Skip VA Idle.
  lcd_write( 0x95 );
  lcd_write( 0x00 ); // Skip AA Clear.
  lcd_write( 0x96 );
  lcd_write( 0x00 ); // Skip AA Idle.
  lcd_write( 0x97 );
  lcd_write( 0x00 ); // Skip Drive.
  // Dummy update results in future supply initialization to Clear Voltage.
  lcd_write( 0x31 ); // Dummy update.
  wait_for_ready();

  // 128x32 and 132x64 init are basically the same so far...

  // More driver initialization code.
  lcd_write( 0xA3 ); // Enable other analog control.
  lcd_write( 0x1A );
  lcd_write( 0xE9 ); // Enable bias driven.


  lcd_write( 0x84);
  lcd_write( 0x2F); // Enable booster and high voltage buffer.
  // Delay to allow dc/dc reach voltage.
  // Rise time to 25V with Vin = 2.4V is 280 msec.
  // Rise time to 25V with Vin = 3.3V is 160 msec.

  // FIXME: should we have a voltage compensation table too?

  delay(300);

  // Set up update.
  lcd_write( 0x93 );
  lcd_write( VAClearRpt ); // VA Clear Repeats
  lcd_write( 0x94 );
  lcd_write( VAIdleRpt ); // VA Idle Repeats
  lcd_write( 0x95 );
  lcd_write( AAClearRpt ); // AA Clear Repeats
  lcd_write( 0x96 );
  lcd_write( AAIdleRpt ); // AA Idle Repeats
  lcd_write( 0x97 );
  lcd_write( DriveRpt ); // Drive Repeats
  lcd_write( 0x32 ); // Drive scheme:
  lcd_write( 0x32 ); // Clear to bright.
  // Configure update area.
  lcd_write( 0xa8 );
  if (numRows == 34)
    lcd_write( numRows+DUMMY_ROWS ); // Mux ratio, full screen.
  else
    lcd_write( numRows ); // Mux ratio, partial screen.
  lcd_write( 0xd3 );
  lcd_write( offset ); // Physical location on screen.
  lcd_write( startLineCmd ); // Starting location in RAM.
  // Set AA-idle-to-drive delay to 8 msec (coincidentally, 0x08 in driver
  // encoding). Shorter values may cause image problems due to chip logic,
  // especially at high operating frequencies. The problem manifests
  // as one or more spurious bright lines during a partial update.
  lcd_write( 0xAA );
  lcd_write( 0x08 );
  // Set COMs to remap or not remap the image. Segs are remapped during
  // display RAM loading.
  if(v_flip_value)
  {
    lcd_write( 0xC0 ); // Scan COMs normally
  }
  else
  {
    lcd_write( 0xC8 ); // Scan COMs in reverse.
  }
  // Perform Update.
  lcd_write( 0x31);
  wait_for_ready();
  // FIXME: This wait takes awhile (especially at low temperature) - use something better than a spinloop...

  // Put display to sleep.
  lcd_write( 0x2A);
  lcd_write( 0xE9);
  lcd_write( 0x04);
  lcd_write( 0xF6);
  lcd_write( 0x00);
  lcd_write( 0xA3);
  lcd_write( 0x00);
  lcd_write( 0xAB);
  lcd_write( 0x00);
  // Power down display/driver.
  //DisableVDD();
}

void NVLCD::set_seg_mapping(void)
{
  // Kent datasheet sez this can only be done while loading data (wtf?)
  // So (unless we prove otherwise) functions that write DDRAM must call this first.
  lcd_mode_cmd();
  if(h_flip_value)
  {
    lcd_write( 0xA1 ); // Remap segs.
  }
  else
  {
    lcd_write( 0xA0 ); // Don't remap segs.
  }

}

/*------ Cursor Functions -----*/

void NVLCD::home(void)
{
  set_cursor(0,0);
}
void NVLCD::set_page(byte page)
{
  // This and many other LCD operate in 'pages' (each input byte supplies data for 8 vertical lines)
  // There is no concept of discrete 'lines' unless you get ugly splitting up data between pages.
  // FIXME: Try to harmonize lines and pages...
  lcd_mode_cmd();
  lcd_write(PAGE_ADD | (page+1));
  // 'page+2' to skip unused and border frame.
}






void NVLCD::set_col(byte col)
{
  lcd_mode_cmd();
  lcd_write (COL_MSN | (col >> 4));
  lcd_write (COL_LSN | (col & 0x0F));
}

void NVLCD::set_cursor(byte row, byte col)
{
  set_page(row/8);
  set_col(col);
}

/*------ Text and Graphic Functions -----*/

void NVLCD::putch(byte c)
{
     Serial.print("Got byte: ");
     Serial.print(c);
     // only supporting fixed size font for now
     //if( font[FONT_LENGTH] == 0 && font[FONT_LENGTH+1] == 0)
     {


        //byte width = pgm_read_byte(&font[FONT_FIXED_WIDTH]);
        //byte height = pgm_read_byte(&font[FONT_HEIGHT]);
        //byte first_char = pgm_read_byte(&font[FONT_FIRST_CHAR]);

        //Serial.print("\nWidth: ");
        //Serial.print(width, HEX);
        //Serial.print("  Height: ");
        //Serial.print(height, HEX);

	byte line=0;
	byte temp=0;


        if(c >= font_first_char)
        {
           // resolve ASCII value to character index in font
           c = c - font_first_char;

           // index the *byte* in font corresponding to this index
           // NB: fonts can be > 8 pixels tall and/or wide.

           //byte char_width_bytes = width; // 1 byte -> 1 col (8 px vertical)
           //byte char_height_bytes = (height+7) / 8; // round up

           //byte char_total_bytes

           // >8 wide -> (char, char+1, ... in font data)
           // >8 tall -> multiple lines (char, char+1, ... in font data)

           // First byte: c * (char_width_bytes * char_height_bytes)
           for(int row=0; row < char_height_bytes; row++)
           {
              // FIXME: advance cursor column here if multiline fonts...
              for(int col=0; col < font_width; col++)
              {
                //Serial.print("row=");
                //Serial.print(row, DEC);
                //Serial.print("  col=");
                //Serial.print(col, DEC);
                int k = (c * (font_width * char_height_bytes) + (row * char_height_bytes) + col)    + 6; // 6 fluff bytes at the start of each font
                //Serial.print("  idx=");
                //Serial.print(k, DEC);

                byte cha = pgm_read_byte(&font[k]);
                //Serial.print("  val=");
                //Serial.print(cha, HEX);
                //Serial.println(" ");
                lcd_write(cha ^ invert_value);
              }
           }

        }

     }

}

void NVLCD::putch_P(PGM_P pc)
{
  putch(pgm_read_byte(pc));
}

void NVLCD::puts(const char * s)
{
     // print a string from RAM
  Serial.print("String addr:");
  Serial.print((long int)s, HEX);
  Serial.println();
    set_seg_mapping();
    lcd_mode_data();
    while(*s != 0)
    {
       putch(*s);
       //*s++;
       s++;
    }
}




void NVLCD::puts_P(PGM_P s)
{
     // print a string from code memory
    byte c;
  Serial.print("String addr:");
  Serial.print((long int)s, HEX);
  Serial.println();

    set_seg_mapping();
    lcd_mode_data();
    while((c = pgm_read_byte(s)) != 0)
    {
       putch(c);
       s++;
    }
}



//void NVLCD::pixmap(char * pImg, int col, int page) // NOTE: 'Page' is an 8px vertical boundary.
void NVLCD::pixmap(int row, int col, const char * pImg) // NOTE: 'Page' is an 8px vertical boundary.
{
	byte xsize=pImg[0];
	byte ysize=pImg[1];

	byte xx=0;
	byte yy=0;

	pImg += 2;

        set_seg_mapping();
	for (yy=0; yy<ysize/8; yy++)
	{
		// handle pages
		set_page(yy+(row/8));
		set_col(col);
                lcd_mode_data();
		for (xx=0; xx<xsize; xx++)
		{
			//handle columns
			lcd_write(*pImg++ ^ invert_value);
		}
	}
}

//void NVLCD::pixmap_P(PGM_P pImg, int col, int page) // NOTE: 'Page' is an 8px vertical boundary.
void NVLCD::pixmap_P(int row, int col, PGM_P pImg) // NOTE: 'Page' is an 8px vertical boundary.
{
        byte xsize = pgm_read_byte(&pImg[0]);
        byte ysize = pgm_read_byte(&pImg[1]);
	byte xx=0;
	byte yy=0;

	pImg += 2;

        set_seg_mapping();
	for (yy=0; yy<ysize/8; yy++)
	{
		// handle pages
		set_page(yy+(row/8));
		set_col(col);
                lcd_mode_data();
		for (xx=0; xx<xsize; xx++)
		{
			//handle columns
                        lcd_write(pgm_read_byte(pImg++) ^ invert_value);

		}
	}
}

/*------ Private Functions -----*/


void NVLCD::cr(void) // display-specific carriage return implementation
{
}

// Serial write or bitbang LCD serial data using I/O lines.
void NVLCD::lcd_write(byte v)
{
  if(spi_putc_type == SPI_PUTC_STD_SPI)
  {
     // Assert chip select to display.
     // The driver seems to require a new chip select to start each byte.
     select();
     SPI.transfer(v);
     deselect();
  }
  else
  {
     lcd_bitbang(v);
  }
}

void NVLCD::lcd_bitbang(byte data)
{
  byte mask;
  // Assert chip select to display.
  // The driver seems to require a new chip select to start each byte.
  select();

  // Bit-bang data out, MSB first. Data changes on falling edge and
  // is latched on rising. Clock is inactive low.
  for (mask = 0x80; mask != 0; mask = mask >> 1)
  {
    if (data & mask)
      digitalWrite(mosi_pin, HIGH);
    else
      digitalWrite(mosi_pin, LOW);

    digitalWrite(sck_pin, HIGH);
    digitalWrite(sck_pin, LOW);

  }
  // De-assert chip select to display.
  deselect();

}



void NVLCD::lcd_mode_cmd()
{
  digitalWrite(c_d_pin, LOW);
}

void NVLCD::lcd_mode_data()
{
  digitalWrite(c_d_pin, HIGH);
}


void NVLCD::select()
{
  digitalWrite(cs_pin, LOW);
}

void NVLCD::deselect()
{
  digitalWrite(cs_pin, HIGH);
}

void NVLCD::wait_for_ready()
{
  // stall until BUSY is low
  while(digitalRead(busy_pin)){;}
}

void NVLCD::init_pins()
{
  if(gnd_pin < 128)
  {
      pinMode(gnd_pin, OUTPUT);
      digitalWrite(gnd_pin, LOW);
  }
  if(pwr_pin < 128)
  {
      pinMode(pwr_pin, OUTPUT);
      digitalWrite(pwr_pin, LOW);
  }
  pinMode(busy_pin, INPUT);

  if(spi_putc_type == SPI_PUTC_BITBANG)
  {
      pinMode(mosi_pin, OUTPUT);
      pinMode(sck_pin, OUTPUT);
  }

  pinMode(reset_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);
  pinMode(c_d_pin, OUTPUT);

}

// stolen from GLCD
/*
 * needed to resolve virtual print functions
 */
void NVLCD::write(uint8_t c) // for Print base class
{
     Serial.print(" write:");
     Serial.print(c);
     putch(c);
}

