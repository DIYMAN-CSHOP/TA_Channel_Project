#define DEBUG

#define BT_Serial 9600 //This value may be change: 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200 
//please try 9600, 19200, 38400, 115200 first if you don't know the value of bluetooth device.

#define DelayInBuildPannel 100

#define PinBT_Tx 11
#define PinBT_Rx 10

#define PinCh1 2
#define PinCh2 3
#define PinCh3 4
#define PinCh4 5
#define PinCh5 6
#define PinCh6 7

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5

#define MinPULSE 1000
#define MaxPULSE 2000
//#define NumMode 3

#define CHAR_FREE_PAD_CH12 'Q'
#define CHAR_FREE_PAD_CH34 'W'

#define CHAR_SLIDER_CH3 'E'
#define CHAR_SLIDER_CH4 'R'
#define CHAR_SLIDER_CH5 'T'
#define CHAR_SLIDER_CH6 'Z'

#define CHAR_SLIDER_PERCENT_CH1 'U'
#define CHAR_SLIDER_PERCENT_CH2 'I'
#define CHAR_SLIDER_PERCENT_CH3 'O'
#define CHAR_SLIDER_PERCENT_CH4 'P'

#define CHAR_BUTTON_INVERT_CH1 'A'
#define CHAR_BUTTON_INVERT_CH2 'S'
#define CHAR_BUTTON_INVERT_CH3 'D'
#define CHAR_BUTTON_INVERT_CH4 'F'
#define CHAR_BUTTON_MIX 'G'
#define CHAR_BUTTON_MODE 'H'
#define CHAR_BUTTON_REFRESH 'J'
#define CHAR_BUTTON_CONFIG 'C'
#define CHAR_SLIDER_SPEED_MODE2 'V'
#define CHAR_8WAY_PAD_MODE2 'L'
#define CHAR_4WAY_PAD_MODE2 'K'

#define NUM_MODE 2

#define ADDR_MODE 0
#define ADDR_IS_MIX 1
#define ADDR_INVERT_CH1 2
#define ADDR_INVERT_CH2 3
#define ADDR_INVERT_CH3 4
#define ADDR_INVERT_CH4 5
#define ADDR_INVERT_CH5 6
#define ADDR_INVERT_CH6 7
#define ADDR_PERCENT_CH1 8
#define ADDR_PERCENT_CH2 9
#define ADDR_PERCENT_CH3 10
#define ADDR_PERCENT_CH4 11
#define ADDR_PERCENT_CH5 12
#define ADDR_PERCENT_CH6 13
#define ADDR_SPEED_MODE2 14

const char* add_text = "add_text(%d,%d,medium,L,\"%s%s\\n\",245,245,245,%c)";
const char* add_button = "add_button(%d,%d,%d,%c;,)";
//const char* add_text_box = "add_text_box(%d,%d,%d,L,%s,245,245,245,%s)";
const char* add_slider = "add_slider(%d,%d,%d,%d,%d,%d,%c,;,0)";
const char* add_free_pad = "add_free_pad(%d,%d,%d,%d,%d,%d,%c,;)";
const char* add_8way_pad = "add_8way_pad(%d,%d,1,5,2,6,3,7,4,8,0,0,%c,;)";
const char* add_4way_pad = "add_4way_pad(%d,%d,1,2,3,4,0,0,%c,;)";
