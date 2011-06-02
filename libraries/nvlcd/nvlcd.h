#ifndef nvlcd_h
#define nvlcd_h


// include types & constants of Wiring core API
#include <WProgram.h>
#include <WConstants.h>
extern "C" {
	#include <avr/pgmspace.h>
}

// Borrow some things from GLCD. Trying to shoehorn this display into GLCD would be a bear; it's just too different.
// Also, since CPU time is at a premium in a power-optimized design, trying to duplicate/adapt all its advanced features here is not a priority to me ;-)
// But would still like to keep a few things compatible, like the nicely-implemented font structure.

#if defined WIRING
#include <WPrint.h> // used when deriving this class in Wiring
#else
#include "Print.h" // used when deriving this class in Arduino 
#endif

#ifndef USE_ARDUINO_FLASHSTR
// these should be replaced when Arduino supports FLASH strings in the base print class
#include <avr/pgmspace.h>
//typedef class _FlashString {} *FLASHSTRING;
//#define flashStr(x) ((FLASHSTRING)(PSTR((x))))
#define print_p(x) puts_P(PSTR(x))
#endif

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

// the following returns true if the given font is fixed width
// zero length is flag indicating fixed width font (array does not contain width data entries)
#define isFixedWidthFont(font)  (FontRead(font+FONT_LENGTH) == 0 && FontRead(font+FONT_LENGTH+1) == 0))

typedef const uint8_t* Font_t;


// end GLCD stuff


// Set display size and glass offset
// So, it seems the 'border' pixels are just pixels, and so the display resolution is actually a little higher than claimed
// (although border pixels are 'stretched' in one direction or the other, e.g. abnormally wide)
// Probably to provide a buffer zone for commercial applications' bezels to hide any discolorations outside the active area.
//#define N_ROWS 32
#define N_ROWS 48
//#define N_COLS 128
#define N_COLS 132

#define DUMMY_ROWS 0
           // FIXME: CHECKME: This constant is used in Kent's datasheet example, but never defined. What should its value be? "Seems to work" without it so far.

// These may no longer be used
#define BYTES_PER_IMAGE (N_ROWS*N_COLS/8)
#define N_PAGES (N_ROWS/8)
#define BYTES_PER_PAGE (BYTES_PER_IMAGE/N_PAGES)

// set DDRAM starting addresses
#define COL_LSN 0x00 // Set Lower Column Address
#define COL_MSN 0x10 // Set Higher Column Address
#define PAGE_ADD 0xB0 // Set Page Address



// Set fixed parameters
#define VAClearRpt 1
#define VAIdleRpt 1
#define AAClearRpt 1
#define AAIdleRpt 1
#define DriveRpt 1
#define Bias 0x02
                 // = 1/7 bias




// Would point to user-defined SPI byte write function if I could be bothered to get that working :-)
//typedef void (*FuncPtr)(byte);

// definitions for the possible SPI write functions
#define SPI_PUTC_BITBANG 0
#define SPI_PUTC_STD_SPI 1


class NVLCD : public Print
{
    public:
        NVLCD(byte cs_pin, byte busy_pin, byte c_d_pin, byte reset_pin, byte pwr_pin, byte gnd_pin, byte sck_pin, byte mosi_pin);
        NVLCD(byte cs_pin, byte busy_pin, byte c_d_pin, byte reset_pin, byte pwr_pin, byte gnd_pin);

        void set_font(Font_t _font);

        void reset(void);
        void init(void);
        void on(void);
        void off(void);
        void set_temp_comp(int);
        void clear(void);
        void set_invert(byte);
        void set_border(byte);
        void set_h_flip(byte);
        void set_v_flip(byte);
        void commit();
        void commit(int, int, int, int);
        void set_seg_mapping();


        void home(void);
        void set_page(byte page);
        void set_col(byte col);
        void set_cursor(byte row, byte col);
        void putch(byte c);
        void putch_P(PGM_P pc);
        void puts(const char *s);
        void puts_P(PGM_P s);
        void pixmap(int row, int col, const char * pImg);
        void pixmap_P(int row, int col, PGM_P pImg);

        void lcd_mode_data();

        void lcd_write(byte);
        void lcd_bitbang(byte); // default write function

    private:
    	void write(uint8_t); // for Print base class
    	
        void cr(void); // HW carriage return

        //void lcd_send_cmd(byte);
        //void lcd_send_data(byte);
        void lcd_mode_cmd();

        void select();
        void deselect();
        void wait_for_ready();
        void init_pins();

        Font_t font;

        byte cs_pin;
        byte busy_pin;
        byte c_d_pin;
        byte reset_pin;
        byte pwr_pin;
        byte gnd_pin;
        byte sck_pin;
        byte mosi_pin;
        int temp_comp_value; // ChLCD operating range can be fully expressed (and then some!) by 1 byte, but leaving this as int makes the TCtable entries align nicely (8)
        byte h_flip_value;
        byte v_flip_value;
        byte invert_value;
        byte spi_putc_type;


        byte font_width;
        byte font_height;
        byte font_first_char;
        byte char_height_bytes;

};

#endif
