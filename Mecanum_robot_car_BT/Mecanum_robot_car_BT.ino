/*
 * Please do not delete the lines below
 * YOUTUBE:   3DPrintAZ
  */
  
#include <SoftwareSerial.h>
#include <Servo.h>

#define PinBT_Tx 4
#define PinBT_Rx 5

#define WHEEL1_PORT 9
#define WHEEL2_PORT 8
#define WHEEL3_PORT 7
#define WHEEL4_PORT 6

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
Servo wheel1, wheel2, wheel3, wheel4;

/*
110
300
600
1200
2400
4800
9600
14400
19200
28800
38400
56000
57600
115200 
*/
void setup()
{
  wheel1.attach(WHEEL1_PORT);
  wheel2.attach(WHEEL2_PORT);
  wheel3.attach(WHEEL3_PORT);
  wheel4.attach(WHEEL4_PORT);

  wheel1.write(VALSERVO_STOP);
  wheel2.write(VALSERVO_STOP);
  wheel3.write(VALSERVO_STOP);
  wheel4.write(VALSERVO_STOP);
  
  //Setup usb serial connection to computer
  Serial.begin(115200);
  //Setup Bluetooth serial connection to android
  bluetooth.begin(9600);  //Có thể thay đổi, thông thường là 115200 hay 9600
  delay(1000);
  Serial.println("HELLO I'M ROBOT :)");
}

//int flagMotor = -1;
//int flagServo = -1;

char CMD = 'S';
#define CMD_STOP 'S'
#define CMD_FORWARD 'F'
#define CMD_BACKWARD 'B'
#define CMD_TURNLEFT 'G'
#define CMD_TURNRIGHT 'I'
#define CMD_CROSSLEFT 'L'
#define CMD_CROSSRIGHT 'R'

int CMD_Index = 0;
char CMD_ARRAY[] = {CMD_STOP, CMD_FORWARD, CMD_STOP, CMD_BACKWARD, CMD_STOP, CMD_TURNLEFT, CMD_STOP, CMD_TURNRIGHT, CMD_STOP, CMD_CROSSLEFT, CMD_STOP, CMD_CROSSRIGHT};

unsigned long oldtime = 0;
void loop()
{
  if(bluetooth.available())
  {
    char valueBT = (char)bluetooth.read();
    Serial.println(valueBT);
    CMD = valueBT;
    CMD = valueBT==CHAR_BACKLEFT?CMD_TURNLEFT:valueBT;
    CMD = valueBT==CHAR_BACKRIGHT?CMD_TURNRIGHT:valueBT;
  }

/*  
    unsigned long t = millis();
    if(t - oldtime > 1000)
    {
      oldtime = t;
      CMD_Index = (++CMD_Index)%12;
    }
  CMD = CMD_ARRAY[CMD_Index];
  Serial.println(String(CMD));
*/  
  
  if(CMD == CMD_STOP)
  {
    wheel1.write(VALSERVO_STOP);
    wheel4.write(VALSERVO_STOP);
    wheel2.write(VALSERVO_STOP);
    wheel3.write(VALSERVO_STOP);
  }
    else if(CMD == CMD_FORWARD)
    {
      wheel1.write(VALSERVO_FORWARD);
      wheel4.write(VALSERVO_FORWARD);
      wheel2.write(VALSERVO_BACKWARD);
      wheel3.write(VALSERVO_BACKWARD);
    }
    else if(CMD == CMD_BACKWARD)
    {
      wheel1.write(VALSERVO_BACKWARD);
      wheel4.write(VALSERVO_BACKWARD);
      wheel2.write(VALSERVO_FORWARD);
      wheel3.write(VALSERVO_FORWARD);
    }
    else if(CMD == CMD_TURNLEFT)
    {
      wheel1.write(VALSERVO_BACKWARD);
      wheel4.write(VALSERVO_BACKWARD);
      wheel2.write(VALSERVO_BACKWARD);
      wheel3.write(VALSERVO_BACKWARD);
    }
    else if(CMD == CMD_TURNRIGHT)
    {
      wheel1.write(VALSERVO_FORWARD);
      wheel4.write(VALSERVO_FORWARD);
      wheel2.write(VALSERVO_FORWARD);
      wheel3.write(VALSERVO_FORWARD);
    }
    else if(CMD == CMD_CROSSLEFT)
    {
      wheel1.write(VALSERVO_BACKWARD);
      wheel4.write(VALSERVO_FORWARD);
      wheel2.write(VALSERVO_BACKWARD);
      wheel3.write(VALSERVO_FORWARD);
    }
    else if(CMD == CMD_CROSSRIGHT)
    {
      wheel1.write(VALSERVO_FORWARD);
      wheel4.write(VALSERVO_BACKWARD);
      wheel2.write(VALSERVO_FORWARD);
      wheel3.write(VALSERVO_BACKWARD);
    }
    delay(10);
}
