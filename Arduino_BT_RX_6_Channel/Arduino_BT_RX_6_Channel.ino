#include <SoftwareSerial.h>
#include <Servo.h>
#include <EEPROM.h>


#include "define.h"

boolean bIsConfigMode = false;
byte Mode = 0;
boolean bIsMix = false;
boolean bInvertCh[6];
int chValue[6];
Servo Sch[6];
byte chPercent[4];
byte SpeedMode2 = 100;

SoftwareSerial bluetooth(PinBT_Tx, PinBT_Rx);

void LoadAll()
{
  Mode = EEPROM.read(ADDR_MODE);
  bIsMix = EEPROM.read(ADDR_IS_MIX);
  
  bInvertCh[CH1] = EEPROM.read(ADDR_INVERT_CH1);
  bInvertCh[CH2] = EEPROM.read(ADDR_INVERT_CH2);
  bInvertCh[CH3] = EEPROM.read(ADDR_INVERT_CH3);
  bInvertCh[CH4] = EEPROM.read(ADDR_INVERT_CH4);
  
  chPercent[CH1] = EEPROM.read(ADDR_PERCENT_CH1);
  chPercent[CH2] = EEPROM.read(ADDR_PERCENT_CH2);
  chPercent[CH3] = EEPROM.read(ADDR_PERCENT_CH3);
  chPercent[CH4] = EEPROM.read(ADDR_PERCENT_CH4);
  SpeedMode2     = EEPROM.read(ADDR_SPEED_MODE2);

  if(Mode>=NUM_MODE || Mode <0) Mode = 0;
  if(chPercent[CH1] <0 || chPercent[CH1] > 100) chPercent[CH1] = 100;
  if(chPercent[CH2] <0 || chPercent[CH2] > 100) chPercent[CH2] = 100;
  if(chPercent[CH3] <0 || chPercent[CH3] > 100) chPercent[CH3] = 100;
  if(chPercent[CH4] <0 || chPercent[CH4] > 100) chPercent[CH4] = 100;
  if(SpeedMode2<0 || SpeedMode2>100) SpeedMode2 = 100;
}

void WriteOutChannel(byte index)
{
  int v = map(chValue[index], 1000, 2000, 0, 180);
  Sch[index].write(v);
}

void InitPannel()
{
  char buf[50];

  Servo_Detach();
  
  bluetooth.println("*.kwl");
  delay(DelayInBuildPannel);
  bluetooth.println("clear_panel()");
  delay(DelayInBuildPannel);
  bluetooth.println("set_grid_size(14,7)");
  delay(DelayInBuildPannel);


  sprintf(buf, add_button, 8, 0, 7, CHAR_BUTTON_REFRESH);
  bluetooth.println(buf);
  delay(DelayInBuildPannel);
  sprintf(buf, add_text, 9, 0, "REFRESH", "", ' ');
  bluetooth.println(buf);
  delay(DelayInBuildPannel);

  sprintf(buf, add_button, 11, 0, 20, CHAR_BUTTON_CONFIG);
  bluetooth.println(buf);
  delay(DelayInBuildPannel);
  sprintf(buf, add_text, 12, 0, bIsConfigMode?"CONTROL":"CONFIG", "", ' ');
  bluetooth.println(buf);
  delay(DelayInBuildPannel);

  if(bIsConfigMode)
  {
      sprintf(buf, add_button, 0, 0, 21, CHAR_BUTTON_INVERT_CH1);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);

      sprintf(buf, add_button, 1, 0, 21, CHAR_BUTTON_INVERT_CH2);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);

      sprintf(buf, add_button, 2, 0, 21, CHAR_BUTTON_INVERT_CH3);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
    
      sprintf(buf, add_button, 3, 0, 21, CHAR_BUTTON_INVERT_CH4);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);

      //=============================================================================
      
      sprintf(buf, add_text, 0, 1, "CH1", bInvertCh[CH1]?"!":"", CHAR_BUTTON_INVERT_CH1);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
    
      sprintf(buf, add_text, 1, 1, "CH2", bInvertCh[CH2]?"!":"", CHAR_BUTTON_INVERT_CH2);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
    
      sprintf(buf, add_text, 2, 1, "CH3", bInvertCh[CH3]?"!":"", CHAR_BUTTON_INVERT_CH3);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
    
      sprintf(buf, add_text, 3, 1, "CH4", bInvertCh[CH4]?"!":"", CHAR_BUTTON_INVERT_CH4);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);

      sprintf(buf, add_slider, 0,2,3,0,100,chPercent[CH1], CHAR_SLIDER_PERCENT_CH1);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
      sprintf(buf, add_slider, 1,2,3,0,100,chPercent[CH2], CHAR_SLIDER_PERCENT_CH2);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
      sprintf(buf, add_slider, 2,2,3,0,100,chPercent[CH3], CHAR_SLIDER_PERCENT_CH3);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
      sprintf(buf, add_slider, 3,2,3,0,100,chPercent[CH4], CHAR_SLIDER_PERCENT_CH4);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
  }
  else
  {
      sprintf(buf, add_button, 0, 0, 3, CHAR_BUTTON_MODE);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
      
      char bufM[8];
      sprintf(bufM, "MODE %d", Mode+1);
      //String bufM = "Mode" + Mode+1;
      //sprintf(buf, add_text_box, 1,0,2,bufM, CHAR_BUTTON_MODE);

      sprintf(buf, add_text, 1, 0, bufM, "", CHAR_BUTTON_MODE);
      bluetooth.println(buf);
      delay(DelayInBuildPannel);
    
    if(Mode==0)
      {
        sprintf(buf, add_button, 3, 0, 21, CHAR_BUTTON_MIX);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        
        sprintf(buf, add_text, 4, 0, bIsMix?"Mix On":"Mix Off", "", CHAR_BUTTON_MIX);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        
        sprintf(buf, add_text, 4, 5, "CH", "5", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_text, 4, 6, "CH", "6", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);

        sprintf(buf, add_slider, 5,5,2,0,100,0,CHAR_SLIDER_CH5);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_slider, 5,6,2,0,2,0,CHAR_SLIDER_CH6);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        
        sprintf(buf, add_free_pad,0,4,0,100,0,50,CHAR_FREE_PAD_CH34);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        
        sprintf(buf, add_free_pad,10,4,0,100,0,50,CHAR_FREE_PAD_CH12);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
      }
    else if(Mode == 1)
      {
        sprintf(buf, add_text, 4, 2, "SPEED", "", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_text, 4, 3, "CH", "3", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_text, 4, 4, "CH", "4", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_text, 4, 5, "CH", "5", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_text, 4, 6, "CH", "6", ' ');
        bluetooth.println(buf);
        delay(DelayInBuildPannel);

        sprintf(buf, add_slider, 5,2,1,0,100,SpeedMode2,CHAR_SLIDER_SPEED_MODE2);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_slider, 5,3,2,0,100,0,CHAR_SLIDER_CH3);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_slider, 5,4,2,0,100,0,CHAR_SLIDER_CH4);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_slider, 5,5,2,0,100,0,CHAR_SLIDER_CH5);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        sprintf(buf, add_slider, 5,6,2,0,2,0,CHAR_SLIDER_CH6);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);

        
        //bluetooth.println("add_8way_pad(10,4,1,5,2,6,3,7,4,8,0,0,L,;)");
        sprintf(buf, add_8way_pad, 10,4, CHAR_8WAY_PAD_MODE2);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
        
        //bluetooth.println("add_4way_pad(0,4,1,2,3,4,0,0,K,;)");
        sprintf(buf, add_4way_pad, 0,4, CHAR_4WAY_PAD_MODE2);
        bluetooth.println(buf);
        delay(DelayInBuildPannel);
      }
  }

  bluetooth.println("set_panel_notes(-,,,)");
  delay(DelayInBuildPannel);
  bluetooth.println("run()");
  delay(DelayInBuildPannel);
  bluetooth.println("*");
  delay(DelayInBuildPannel);

  Servo_Attach();
}


void Servo_Attach()
{
  Sch[CH1].attach(PinCh1, MinPULSE, MaxPULSE);// (pin, min pulse width, max pulse width in microseconds) 
  Sch[CH2].attach(PinCh2, MinPULSE, MaxPULSE);
  Sch[CH3].attach(PinCh3, MinPULSE, MaxPULSE);
  Sch[CH4].attach(PinCh4, MinPULSE, MaxPULSE);
  Sch[CH5].attach(PinCh5, MinPULSE, MaxPULSE);
  Sch[CH6].attach(PinCh6, MinPULSE, MaxPULSE);

  for(byte i=0; i<6; i++)
    WriteOutChannel(i);
}

void Servo_Detach()
{
  Sch[CH1].detach();
  Sch[CH2].detach();
  Sch[CH3].detach();
  Sch[CH4].detach();
  Sch[CH5].detach();
  Sch[CH6].detach();
}

void setup()
{
  for(byte i=0; i<4; i++)
  {
    bInvertCh[i] = false;
    chValue[i] = 1500;
    chPercent[i] = 100;
  }
  chValue[CH5] = 1000;
  chValue[CH6] = 1000;

  LoadAll();

  Serial.begin(115200);
  bluetooth.begin(BT_Serial);
  delay(1000);

#ifdef DEBUG
  Serial.println("BLUETOOTH CONTROLLER RX V1.1");
#endif

  InitPannel();

  delay(1000);
}

int StrBTIndex = -1;
char StrBT[30];

int oldX, oldY;
int ch34_Px, ch34_Py;
//unsigned long lastimeUpdateXY = 0;

bool bEndCMD = false;

int count = 0;

void PrintChannelInfo()
{
  char buf[50];
  sprintf(buf, "%d_%d_%d_%d_%d_%d", chValue[CH1], chValue[CH2], chValue[CH3], chValue[CH4], chValue[CH5], chValue[CH6]);
  Serial.println(buf);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    char valueBT = (char)bluetooth.read();

    if(StrBTIndex < 30)
      StrBTIndex++;
      else StrBTIndex = 0;
      
    StrBT[StrBTIndex] = valueBT;
    if(valueBT == ';')
    {
      bEndCMD = true;
    }
    if(bEndCMD)
    {
          StrBT[StrBTIndex + 1] = 0;
#ifdef DEBUG
          Serial.println(StrBT);
#endif  

          if(StrBT[0] == CHAR_BUTTON_REFRESH)
          {
            InitPannel();
#ifdef DEBUG
            Serial.println("REFRESH");
#endif  
          }
          else if(StrBT[0] == CHAR_BUTTON_CONFIG)
          {
            bIsConfigMode = !bIsConfigMode;
            //Sch[CH2].write(90);
            InitPannel();
#ifdef DEBUG
            //Serial.println(Mode);
#endif  
          }
          else if(StrBT[0] == CHAR_BUTTON_MODE)
          {
            Mode++;
            Mode=Mode%NUM_MODE;
#ifdef DEBUG
            Serial.println(Mode);
#endif  
            EEPROM.write(ADDR_MODE, Mode);
            InitPannel();
          }
          else if(StrBT[0] == CHAR_BUTTON_MIX)
          {
            bIsMix = !bIsMix;

            char buf5[10];
            sprintf(buf5,"*%c%s", CHAR_BUTTON_MIX, bIsMix?"Mix On":"Mix Off");
            bluetooth.println(buf5);
            bluetooth.println(buf5);
#ifdef DEBUG
            Serial.println(bIsMix?"Mix On":"Mix Off");
#endif  
            EEPROM.write(ADDR_IS_MIX, bIsMix);
          }
          else if(StrBT[0] == CHAR_BUTTON_INVERT_CH1)  //Invert CH1
          {
            bInvertCh[CH1] = !bInvertCh[CH1];
            char buf5[10];
            sprintf(buf5,"*%cCH%d%s", CHAR_BUTTON_INVERT_CH1, 1, bInvertCh[CH1]?"!":"");
            bluetooth.println(buf5);
            bluetooth.println(buf5);
            //Serial.println(buf5);
            EEPROM.write(ADDR_INVERT_CH1, bInvertCh[CH1]);
#ifdef DEBUG
            Serial.print("CH1 "); Serial.println(bInvertCh[CH1]?"Invert On":"Invert Off");
#endif  
          }
          else if(StrBT[0] == CHAR_BUTTON_INVERT_CH2)  //Invert CH2
          {
            bInvertCh[CH2] = !bInvertCh[CH2];
            char buf5[10];
            sprintf(buf5,"*%cCH%d%s", CHAR_BUTTON_INVERT_CH2, 2, bInvertCh[CH2]?"!":"");
            bluetooth.println(buf5);
            bluetooth.println(buf5);
            //Serial.println(buf5);
            EEPROM.write(ADDR_INVERT_CH2, bInvertCh[CH2]);
#ifdef DEBUG
            Serial.print("CH2 "); Serial.println(bInvertCh[CH2]?"Invert On":"Invert Off");
#endif  
          }
          else if(StrBT[0] == CHAR_BUTTON_INVERT_CH3)  //Invert CH3
          {
            bInvertCh[CH3] = !bInvertCh[CH3];
            char buf5[10];
            sprintf(buf5,"*%cCH%d%s", CHAR_BUTTON_INVERT_CH3, 3, bInvertCh[CH3]?"!":"");
            bluetooth.println(buf5);
            bluetooth.println(buf5);
            //Serial.println(buf5);
            EEPROM.write(ADDR_INVERT_CH3, bInvertCh[CH3]);
#ifdef DEBUG
            Serial.print("CH3 "); Serial.println(bInvertCh[CH3]?"Invert On":"Invert Off");
#endif  
          }
          else if(StrBT[0] == CHAR_BUTTON_INVERT_CH4)  //Invert CH4
          {
            bInvertCh[CH4] = !bInvertCh[CH4];
            char buf5[10];
            sprintf(buf5,"*%cCH%d%s", CHAR_BUTTON_INVERT_CH4, 4, bInvertCh[CH4]?"!":"");
            bluetooth.println(buf5);
            bluetooth.println(buf5);
            ///Serial.println(buf5);
            EEPROM.write(ADDR_INVERT_CH4, bInvertCh[CH4]);
#ifdef DEBUG
            Serial.print("CH4 "); Serial.println(bInvertCh[CH4]?"Invert On":"Invert Off");
#endif  
          }
          else if(StrBT[0] == CHAR_FREE_PAD_CH34)
          {
            
            int X=-1, Y=-1;
            sscanf(StrBT+1, "X%dY%d", &X, &Y);
            if(X >=0 && X <= 100 && Y >= 0 && Y<=100)
            {
              int dtCH3 = 500*(1-chPercent[CH3]/100.0);
              int dtCH4 = 500*(1-chPercent[CH4]/100.0);
              
              Y = 100 - Y;

              chValue[CH3] = map(Y, 0, 100, 1000+dtCH3, 2000-dtCH3);
              chValue[CH4] = map(X, 0, 100, 1000+dtCH4, 2000-dtCH4);
              
              if(bInvertCh[CH3]) chValue[CH3] = 3000 - chValue[CH3];
              if(bInvertCh[CH4]) chValue[CH4] = 3000 - chValue[CH4];
              
              WriteOutChannel(CH3);
              WriteOutChannel(CH4);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }
          else if(StrBT[0] == CHAR_FREE_PAD_CH12)
          {
            int X=-1, Y=-1;
            sscanf(StrBT+1, "X%dY%d", &X, &Y);
            if(X >=0 && X <= 100 && Y >=0 && Y<=100)
            {
                int dtCH1 = 500*(1-chPercent[CH1]/100.0);
                int dtCH2 = 500*(1-chPercent[CH2]/100.0);
              
                Y = 100 - Y;
                if(bIsMix == false)
                {
                  chValue[CH1] = map(X, 0, 100, 1000+dtCH1, 2000-dtCH1);
                  chValue[CH2] = map(Y, 0, 100, 1000+dtCH2, 2000-dtCH2);
                }
                else
                {
                  int v1 = Y+X-50; if(v1<0) v1 = 0; if(v1>100) v1 = 100;
                  int v2 = Y+50-X; if(v2<0) v2 = 0; if(v2>100) v2 = 100;
                  
                  chValue[CH1] = map(v1, 0, 100, 1000+dtCH1, 2000-dtCH1);
                  chValue[CH2] = map(v2, 0, 100, 1000+dtCH2, 2000-dtCH2);
                  //chValue[CH1] = chY + (chX - 1500);
                  //chValue[CH2] = chY + (1500 - chX);
    
                  //if(chValue[CH1] > 2000) chValue[CH1] = 2000;
                  //if(chValue[CH1] < 1000) chValue[CH1] = 1000;
                  //if(chValue[CH2] > 2000) chValue[CH2] = 2000;
                  //if(chValue[CH2] < 1000) chValue[CH2] = 1000;
                }

                if(bInvertCh[CH1]) chValue[CH1] = 3000 - chValue[CH1];
                if(bInvertCh[CH2]) chValue[CH2] = 3000 - chValue[CH2];
                
                WriteOutChannel(CH1);
                WriteOutChannel(CH2);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_CH3)
          {
            int X=0;
            sscanf(StrBT+1, "%d", &X);
            if(X>=0 && X<=100)
            {
              chValue[CH3] = map(X, 0, 100, 1000, 2000);
              if(bInvertCh[CH3]) chValue[CH3] = 3000 - chValue[CH3];
              WriteOutChannel(CH3);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_CH4)
          {
            int X=0;
            sscanf(StrBT+1, "%d", &X);
            if(X>=0 && X<=100)
            {
              chValue[CH4] = map(X, 0, 100, 1000, 2000);
              if(bInvertCh[CH4]) chValue[CH4] = 3000 - chValue[CH4];
              WriteOutChannel(CH4);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_CH5)
          {
            int X=0;
            sscanf(StrBT+1, "%d", &X);
            if(X>=0 && X<=100)
            {
              chValue[CH5] = map(X, 0, 100, 1000, 2000);
              WriteOutChannel(CH5);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_CH6)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=2)
            {
              chValue[CH6] = map(X, 0, 2, 1000, 2000);
              WriteOutChannel(CH6);
#ifdef DEBUG
              PrintChannelInfo();
#endif              
            }
          }

          else if(StrBT[0] == CHAR_SLIDER_PERCENT_CH1)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=100)
            {
              chPercent[CH1] = X;
              EEPROM.write(ADDR_PERCENT_CH1, chPercent[CH1]);
#ifdef DEBUG
              Serial.print("Percent CH1:");
              Serial.println(chPercent[CH1]);
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_PERCENT_CH2)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=100)
            {
              chPercent[CH2] = X;
              EEPROM.write(ADDR_PERCENT_CH2, chPercent[CH2]);
#ifdef DEBUG
              Serial.print("Percent CH2:");
              Serial.println(chPercent[CH2]);
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_PERCENT_CH3)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=100)
            {
              chPercent[CH3] = X;
              EEPROM.write(ADDR_PERCENT_CH3, chPercent[CH3]);
#ifdef DEBUG
              Serial.print("Percent CH3:");
              Serial.println(chPercent[CH3]);
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_PERCENT_CH4)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=100)
            {
              chPercent[CH4] = X;
              EEPROM.write(ADDR_PERCENT_CH4, chPercent[CH4]);
#ifdef DEBUG
              Serial.print("Percent CH4:");
              Serial.println(chPercent[CH4]);
#endif              
            }
          }
          else if(StrBT[0] == CHAR_SLIDER_SPEED_MODE2)
          {
            int X=-1;
            sscanf(StrBT+1, "%d", &X);
            if(X >= 0 && X<=100)
            {
              SpeedMode2 = X;
              EEPROM.write(ADDR_SPEED_MODE2, SpeedMode2);
#ifdef DEBUG
              Serial.print("Speed Mode2: ");
              Serial.println(SpeedMode2);
#endif              
            }
          }
          else if(StrBT[0] == CHAR_4WAY_PAD_MODE2)
          {
            int dt = 500*(SpeedMode2/100.0);
            //int dtCh1, dtCh2;
            if(StrBT[1] == '0') //STOP
            {
              chValue[CH1] = 1500;
              chValue[CH2] = 1500;
            }
            else if(StrBT[1] == '1') //FORWARD
            {
              chValue[CH1] = 1500 + dt;
              chValue[CH2] = 1500 + dt;
            }
            else if(StrBT[1] == '2') //RIGH
            {
              chValue[CH1] = 1500 + dt;
              chValue[CH2] = 1500 - dt;
            }
            else if(StrBT[1] == '3') //BACKWARD
            {
              chValue[CH1] = 1500 - dt;
              chValue[CH2] = 1500 - dt;
            }
            else if(StrBT[1] == '4') //LEFT
            {
              chValue[CH1] = 1500 - dt;
              chValue[CH2] = 1500 + dt;
            }
              if(bInvertCh[CH1]) chValue[CH1] = 3000 - chValue[CH1];
              if(bInvertCh[CH2]) chValue[CH2] = 3000 - chValue[CH2];
              WriteOutChannel(CH1);
              WriteOutChannel(CH2);
#ifdef DEBUG
              PrintChannelInfo();
#endif        
          }
          else if(StrBT[0] == CHAR_8WAY_PAD_MODE2)
          {
            int dt = 500*(SpeedMode2/100.0);
            if(StrBT[1] == '0')//STOP
            {
              chValue[CH1] = 1500;
              chValue[CH2] = 1500;
            }
            else if(StrBT[1] == '1') //UP
            {
              chValue[CH1] = 1500 + dt;
              chValue[CH2] = 1500 + dt;
            }
            else if(StrBT[1] == '2') //RIGHT
            {
              chValue[CH1] = 1500 + dt;
              chValue[CH2] = 1500;
            }
            else if(StrBT[1] == '3') //DOWN
            {
              chValue[CH1] = 1500 - dt;
              chValue[CH2] = 1500 - dt;
            }
            else if(StrBT[1] == '4') //LEFT
            {
              chValue[CH1] = 1500;
              chValue[CH2] = 1500 + dt;
            }
            else if(StrBT[1] == '5')//UP RIGHT
            {
              chValue[CH1] = 1500 + dt;
              chValue[CH2] = 1500 + dt/2;
            }
            else if(StrBT[1] == '6')//DOWN RIGHT
            {
              chValue[CH1] = 1500 - dt;
              chValue[CH2] = 1500 - dt/2;
            }
            else if(StrBT[1] == '7')//DOWN LEFT
            {
              chValue[CH1] = 1500 - dt/2;
              chValue[CH2] = 1500 - dt;
            }
            else if(StrBT[1] == '8')//UP LEFT
            {
              chValue[CH1] = 1500 + dt/2;
              chValue[CH2] = 1500 + dt;
            }
              if(bInvertCh[CH1]) chValue[CH1] = 3000 - chValue[CH1];
              if(bInvertCh[CH2]) chValue[CH2] = 3000 - chValue[CH2];
              WriteOutChannel(CH1);
              WriteOutChannel(CH2);
#ifdef DEBUG
              PrintChannelInfo();
#endif        
          }
          
      StrBTIndex = 50;
      bEndCMD = false;
    }
  }
//  delay(10);
}
