#ifndef _DEF_MYLCD_
#define _DEF_MYLCD_

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include <TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define GRAY    0x8410
#define WHITE   0xFFFF
#define RED     0xF800
#define ORANGE  0xFA60
#define YELLOW  0xFFE0 
#define LIME    0x07FF
#define GREEN   0x07E0
#define CYAN    0x07FF
#define AQUA    0x04FF
#define BLUE    0x001F
#define MAGENTA 0xF81F

// LCD & Font Size define(for 2.4")
#define PIXEL_X  320
#define PIXEL_Y  240
#define FONT_X  7
#define FONT_Y  9

//====== Button define =====
#define BTN_MAX 20

//====== Touch Screen =======
// These are the pins for the shield!
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 195//150
#define TS_MINY 170//250//120
#define TS_MAXX 910//920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define KEYAUTORELEASEMS 350

// define position struct
typedef struct 
{ 
   unsigned int x;   
   unsigned int y;   
   unsigned int sizex;
   unsigned int sizey;
} RECT;
 
typedef struct 
{ 
   unsigned int x;   
   unsigned int y;   
} Point;

class MyLCD : public Adafruit_TFTLCD {

  public:
    MyLCD(void);
    void vInit(void); 
    void vLCDInit(uint16_t defBGcolor=BLACK, uint8_t dRotation=1);
    void vDrawText(RECT area, int textsize, String sText, uint16_t txtcolor, uint16_t bgcolor=0, bool bFillBG=false);
    void vDrawText(Point pt, int textsize, String sText, uint16_t txtcolor, uint16_t bgcolor=0, bool bFillBG=false);
    uint8_t dDrawButton(RECT area, String sText, uint16_t bndcolor, uint16_t txtcolor, uint16_t bgcolor, uint8_t dRtnValue=0xFF);
    void vPress(uint8_t dBtnInd, boolean p);
    void vClean(void);
    uint8_t dCheckBTNEvent(uint8_t dBtnID=0xFF);
    void vSetRotation(uint8_t dLocation=0);
    TSPoint sGetTSPoint();

   private:
    uint8_t dBtnCount;
    uint8_t daBtnRtnValue[BTN_MAX];
    uint8_t dRot; //dRotation
    int16_t dTSXLow, dTSXHigh, dXLow, dXHigh;
    int16_t dTSYLow, dTYXHigh, dYLow, dYHigh;
    unsigned long dKeyPressTime;
    
};

#endif //_DEF_MYLCD_
