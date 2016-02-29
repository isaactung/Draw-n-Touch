#include "MyLCD.h"


uint16_t identifier;
RECT top;
Point center;

uint16_t dDefBGColor;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button btn[BTN_MAX];

MyLCD::MyLCD(void) : Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET)
{
  dBtnCount = 0;
  vInit();
}

void MyLCD::vInit(void)
{
  // ===TFT init ===
  center.x= PIXEL_X*2; center.y=PIXEL_Y*2; 
  top.x=0; top.y=0; top.sizex=PIXEL_X; top.sizey=PIXEL_Y;  // draw area
}

void MyLCD::vLCDInit(uint16_t defBGcolor,uint8_t dRotation)
{
  Point pt;

  dRot = dRotation%4;
  dDefBGColor = defBGcolor;
  reset();
  identifier = readID();

  if(identifier == 0x9325) {
   // Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x4535) {
   // Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x9328) {
   // Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
   // Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
   // Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
   // Serial.println(F("Found HX8357D LCD driver"));
  } else {
   // Serial.print(F("Found LCD ID: "));
   // Serial.println(identifier,HEX);
    identifier = 0x9341;
  }

  begin(identifier);
  vSetRotation(dRot);
  vClean();
  pt.x = 0; pt.y = 100;
  vDrawText(pt,2,"LCTD ID: 0x"+String(identifier,HEX)+"\r\nInit Completed!", WHITE);
  
  //drawCircle(100,100,50,BLUE);
}

void MyLCD::vDrawText(Point pt, int textsize, String sText, uint16_t txtcolor, uint16_t bgcolor, bool bFillBG)
{
  RECT area;
  int strLen = 0;
  
  strLen = sText.length();
  area.x = pt.x;
  area.y = pt.y;
  area.sizex = FONT_X*strLen*textsize;
  area.sizey = FONT_Y*textsize;
  vDrawText(area, textsize, sText, txtcolor, bgcolor, bFillBG);
}
void MyLCD::vDrawText(RECT area, int textsize, String sText, uint16_t txtcolor, uint16_t bgcolor, bool bFillBG)
{
  
  setTextColor(txtcolor);
  setCursor(area.x,area.y);
  setTextSize(textsize);
  if (bFillBG)
    fillRect(area.x,area.y, area.sizex, area.sizey, bgcolor);
  setCursor(area.x+1,area.y+1); 
  print(sText);
  
}

uint8_t MyLCD::dDrawButton(RECT area, String sText, uint16_t bndcolor, uint16_t txtcolor, uint16_t bgcolor, uint8_t dRtnValue)
{
  uint8_t dReturn = 0xFF;
  char  text[10];
  int textsize = (area.sizey/FONT_Y)-1;  // auto cala font size

  if (dBtnCount < BTN_MAX)
  {    
    sText.toCharArray(text,10);
    
    btn[dBtnCount].initButton((Adafruit_GFX *)this,area.x,area.y,area.sizex,area.sizey,bndcolor,bgcolor,txtcolor,text,textsize);
    btn[dBtnCount].drawButton();
    dReturn = dBtnCount;

    if (dRtnValue != 0xFF)
      daBtnRtnValue[dBtnCount] = dRtnValue;
    dBtnCount++;  
    dReturn =   dBtnCount;
  }

  return dReturn;
}

void MyLCD::vPress(uint8_t dBtnInd, boolean p)
{
  if (dBtnInd < dBtnCount)
  {
    btn[dBtnInd].drawButton(p);
    btn[dBtnInd].press(p);
  }
  
}

void MyLCD::vClean(void)
{
  fillScreen(dDefBGColor);
}

TSPoint MyLCD::sGetTSPoint()
{
   TSPoint p = ts.getPoint();
   TSPoint q;

   q.x=q.y=q.z=-1;

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
   {
    
    if (dRot%2 ==0) q = p;
    else { q.x=p.y; q.y=p.x; }
    
    q.x = map(q.x, dTSXLow, dTSXHigh, dXLow, dXHigh);
    q.y = map(q.y, dTSYLow, dTYXHigh, dYLow, dYHigh);
    
   }
   return q;
}

uint8_t MyLCD::dCheckBTNEvent(uint8_t dBtnID)
{
  uint8_t dRtnVal = 0xFF;
  TSPoint p = ts.getPoint();
  TSPoint q;

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    
    if (dRot%2 ==0) q = p;
    else { q.x=p.y; q.y=p.x; }
    
    q.x = map(q.x, dTSXLow, dTSXHigh, dXLow, dXHigh);
    q.y = map(q.y, dTSYLow, dTYXHigh, dYLow, dYHigh);

    // change to use function sGetTSPoint???!!!!
    
    for (int ind = 0; ind < dBtnCount ; ind++)
    {
        if (((dBtnID==0xFF) || (ind == dBtnID) ) && (btn[ind].contains(q.x,q.y)) )
        {
          //dRtnVal = daBtnRtnValue[ind];
          if (!btn[ind].isPressed())
            vPress(ind,true);
          dKeyPressTime = millis();
        }
        else if (btn[ind].isPressed())
        {
          vPress(ind,false);
           dRtnVal = daBtnRtnValue[ind];
        }

      }
    
  }
  else
  { 
    if (millis() -dKeyPressTime >= KEYAUTORELEASEMS)
    {
      for (int ind = 0; ind < dBtnCount ; ind++)
      {
        if (btn[ind].isPressed())
        {
            vPress(ind,false);
            dRtnVal = daBtnRtnValue[ind];
        }
      }
    }
  }

  return dRtnVal;
}

void MyLCD::vSetRotation(uint8_t dLocation)
{
  setRotation(dLocation);
  switch (dLocation)
  {
    case 0:
      dTSXLow = TS_MINX; dTSXHigh = TS_MAXX ;
      dXLow = width();  dXHigh = 0;
      dTSYLow = TS_MINY; dTYXHigh = TS_MAXY;
      dYLow = height(); dYHigh = 0;
    break;
    case 1:
      dTSXLow = TS_MAXY; dTSXHigh = TS_MINY ;
      dXLow = 0;  dXHigh = width();
      dTSYLow = TS_MINX; dTYXHigh = TS_MAXX;
      dYLow = 0; dYHigh = height();
    break;
    case 2:
      dTSXLow = TS_MINX; dTSXHigh = TS_MAXX ;
      dXLow = 0;  dXHigh = width();
      dTSYLow = TS_MINY; dTYXHigh = TS_MAXY;
      dYLow = 0; dYHigh = height();
    break;
    case 3:
      dTSXLow = TS_MAXY; dTSXHigh = TS_MINY; 
      dXLow =  width();  dXHigh = 0;
      dTSYLow = TS_MINX; dTYXHigh = TS_MAXX;
      dYLow = height(); dYHigh = 0;
    break;
  }
  
}

