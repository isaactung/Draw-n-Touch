#include "MyLCD.h"

MyLCD lcd;
RECT TextArea;
void setup() {
  RECT btn;
  btn.x = PIXEL_X-70; btn.y=-10; btn.sizex=40; btn.sizey=30;
  TextArea.x=0; TextArea.y=0; TextArea.sizex=200; TextArea.sizey=50;

  Serial.begin(115200);
  
  lcd.vLCDInit(BLACK);
  lcd.vClean();
  for (int d =0; d< 10; d++)
  {
    if (d==5) 
    {
      btn.x +=42;
      btn.y=-10;
    }
    
    btn.y += 32;
    lcd.dDrawButton(btn,String(d),WHITE,BLACK,GREEN,0x30+d);

  }
  btn.x -=21; btn.y += 32;  btn.sizex=80;
  lcd.dDrawButton(btn,"BACK",WHITE,BLACK,GREEN,'B');
  btn.y += 32;
  lcd.dDrawButton(btn,"Enter",WHITE,BLACK,GREEN,'E');

}

int dCnt=0;
char dsp[5];
int d=0;

void loop() {
uint8_t rtn=0xFF;
    //lcd.vPress(dCnt,true);
   rtn = lcd.dCheckBTNEvent();
   if ((rtn == 'B') )
   {
    if (d==5) d=4;
    dsp[d]=0; 
    if (d>0) d--;
   }
   else if (rtn == 'E')
   {
    d=0; memset(&dsp,0,5);
   }
   else if (rtn != 0xFF)
   {
    if (d==5)
     {
      for (int i=0; i<4; i++)
        dsp[i] =dsp[i+1];
      dsp[4]=rtn;
     }
     else
      dsp[d++] =rtn;
   }

   if (rtn != 0xFF)
   {
    lcd.vDrawText(TextArea, 2, String(dsp) , WHITE, BLACK, true);
    Serial.println(rtn,HEX);
   }
   
   rtn = 0xFF;
   
}
