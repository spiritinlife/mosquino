#ifndef nvlcd_h
#define nvlcd_h


// include types & constants of Wiring core API
#include <WProgram.h>
#include <WConstants.h>
extern "C" {
	#include <avr/pgmspace.h>
}


// Set display size and glass offset
// So, it seems the 'border' pixels are just pixels, and so the display resolution is actually a little higher than claimed.
// Probably to provide a buffer zone for commercial applications' bezels to hide any discolorations outside the active area.
//#define N_ROWS 32
#define N_ROWS 48
//#define N_COLS 128
#define N_COLS 132


#define DUMMY_ROWS 0
           // FIXME

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





typedef void (*FuncPtr)(byte);




class NVLCD
{
    public:
        NVLCD(byte cs_pin, byte busy_pin, byte c_d_pin, byte reset_pin, byte pwr_pin, byte gnd_pin, byte sck_pin, byte mosi_pin);
        NVLCD(byte cs_pin, byte busy_pin, byte c_d_pin, byte reset_pin, byte pwr_pin, byte gnd_pin, FuncPtr pWriteFunc);

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
        void set_line(byte line);
        void set_col(byte col);
        void set_cursor(byte row, byte col);
        void putch(byte c);
        void putch_P(PGM_P pc);
        void puts(const char *s);
        void puts_P(PGM_P ps);
        void pixmap(char * pImg, int col, int page);
        void pixmap_P(PGM_P pImg, int col, int page);
        

    private:
        void cr(void); // HW carriage return
        void lcd_write(byte);
        void lcd_bitbang(byte); // default write function
        //void lcd_send_cmd(byte);
        //void lcd_send_data(byte);
        void lcd_mode_cmd();
        void lcd_mode_data();
        void select();
        void deselect();
        void wait_for_ready();
        void init_pins();

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
        FuncPtr pWriteFunc;

};

#endif
