#include <SoftwareSerial.h>
#include <Servo.h>

#define PinBT_Tx A2
#define PinBT_Rx A3

#define CH1_PORT 2 //LEFT WHEEL
#define CH2_PORT 4 //RIGHT WHEEL

#define VALSERVO_FORWARD 180
#define VALSERVO_STOP 92
#define VALSERVO_BACKWARD 0

#define CHAR_STOP 'S'
#define CHAR_FORWARD 'F'
#define CHAR_BACKWARD 'B'
#define CHAR_TURNLEFT 'L'
#define CHAR_TURNRIGHT 'R'
#define CHAR_FORWARLEFT 'G'
#define CHAR_FORWARDRIGHT 'I'
#define CHAR_BACKLEFT 'H'
#define CHAR_BACKRIGHT 'J'

SoftwareSerial bluetooth(PinBT_Tx, PinBT_Rx);
Servo ch1, ch2;

void setup()
{
  ch1.attach(CH1_PORT);
  ch2.attach(CH2_PORT);

  ch1.write(VALSERVO_STOP);
  ch2.write(VALSERVO_STOP);
  
  //Setup usb serial connection to computer
  Serial.begin(115200);
  //Setup Bluetooth serial connection to android
  bluetooth.begin(115200);
  delay(1000);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    char valueBT = (char)bluetooth.read();
    Serial.println(valueBT);

    if(valueBT == CHAR_STOP)
    {
      ch1.write(VALSERVO_STOP);
    }
    else if(valueBT == CHAR_FORWARD || valueBT == CHAR_TURNRIGHT || valueBT == CHAR_FORWARDRIGHT || valueBT == CHAR_BACKRIGHT)
      {
        ch1.write(VALSERVO_FORWARD);
      }
      else if(valueBT == CHAR_BACKWARD || valueBT == CHAR_TURNLEFT || valueBT == CHAR_FORWARLEFT || valueBT == CHAR_BACKLEFT)
      {
        ch1.write(VALSERVO_BACKWARD);
      }

    if(valueBT == CHAR_STOP)
    {
      ch2.write(VALSERVO_STOP);
    }
    else if(valueBT == CHAR_FORWARD || valueBT == CHAR_TURNLEFT || valueBT == CHAR_FORWARLEFT || valueBT == CHAR_BACKLEFT)
      {
        ch2.write(VALSERVO_BACKWARD);
      }
      else if(valueBT == CHAR_BACKWARD || valueBT == CHAR_TURNRIGHT || valueBT == CHAR_FORWARDRIGHT || valueBT == CHAR_BACKRIGHT)
      {
        ch2.write(VALSERVO_FORWARD);        
      }
  }
}
