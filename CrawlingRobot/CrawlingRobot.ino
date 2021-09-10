/*
 * Please do not delete the lines below
 * Vui lòng tôn trọng và không xóa những dòng bênh dưới
 * Đây là mã nguồn mở của KÊNH NTECH
 * ĐT & ZALO: 0909.241.242
 * YOUTUBE:   KÊNH NTECH
 * FACEBOOK:  KÊNH NTECH
  */
  
#include <Servo.h>
#include <EEPROM.h>

//Ultrasonic Sensor
#define trigPin 6
#define echoPin 5

//Define Variables
long duration;
int distance;

#define LED_PIN 13
#define INTERRUPT_PIN 2
#define CH1_PORT 10 //LEFT WHEEL
#define CH2_PORT 11 //RIGHT WHEEL

#define VALSERVO_FORWARD 180
#define VALSERVO_STOP 90
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

//FOLLOW_MODE
#define MIN_DIS 5
#define MID_DIS 10
#define MAX_DIS 20

//AVOIDING_MODE
#define AVOIDING_DIS 10

#define FOLLOW_MODE 0
#define AVOIDING_MODE 1

int MODE = FOLLOW_MODE;

Servo ch1, ch2;
#define ADDR_MEM_MODE 0

#define NUM_ARRAY_OLD_DISTANCES 5
int OldDistances[NUM_ARRAY_OLD_DISTANCES];

void setup(){
  Serial.begin(115200);

  MODE = EEPROM.read(ADDR_MEM_MODE);

  for(int i=0; i<NUM_ARRAY_OLD_DISTANCES; i++)
    OldDistances[i] = 32767/NUM_ARRAY_OLD_DISTANCES;

  pinMode(INTERRUPT_PIN, INPUT_PULLUP); //Nhớ sử dụng điện trở khoảng 10K cho chân INTERUPT.
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), modeChange, FALLING);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_PIN, OUTPUT);


  ch1.attach(CH1_PORT);
  ch2.attach(CH2_PORT);

  ch1.write(VALSERVO_STOP);
  ch2.write(VALSERVO_STOP);  
}

bool bNewModeSetting = false;
unsigned long myTime = 0;
void modeChange() {
  unsigned long t = millis();
  if(t - myTime > 500)
  {
    myTime = t;
    MODE=(++MODE)%2;
    bNewModeSetting = true;
    Serial.println("MODE: " + String(MODE));
    EEPROM.write(ADDR_MEM_MODE, MODE);
  }
}

int timeProcess = 0;

unsigned long _t1 = 0;
bool IsSleep = false;

char valueBT = CHAR_STOP;

void loop(){
  if(bNewModeSetting)
  {
    bNewModeSetting = false;
    for(int i=0; i<MODE+1; i++)
    {
      digitalWrite(LED_PIN , HIGH);
      delay(500);
      digitalWrite(LED_PIN , LOW);
      delay(500);
    }
  }
  
  timeProcess = 0;
  
  
  digitalWrite(trigPin , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin , LOW);

  duration = pulseIn(echoPin , HIGH);
  distance = (duration/2) / 28.5;

  for(int i=NUM_ARRAY_OLD_DISTANCES-1; i>0; i--)
    OldDistances[i] = OldDistances[i-1];
  OldDistances[0] = distance;
  distance = 0;
  for(int i=0; i<NUM_ARRAY_OLD_DISTANCES; i++)
    distance += OldDistances[i];
  distance /=  NUM_ARRAY_OLD_DISTANCES;


if(MODE == FOLLOW_MODE)
{
      timeProcess = 0;
      if(distance <= MIN_DIS)
      {
        IsSleep = false;
        valueBT = CHAR_BACKWARD;
        Serial.println(String(distance) + "<=MIN_DIS: MoveBackward");
      }
      if(distance > MIN_DIS && distance <= MID_DIS)
      {
        IsSleep = false;
        valueBT = CHAR_STOP;
        Serial.println("MIN_DIS<"+String(distance)+"<=MID_DIS: Stand");
      }
      if(distance > MID_DIS && distance <= MAX_DIS){
        IsSleep = false;
        valueBT = CHAR_FORWARD;
        Serial.println("MID_DIS<"+String(distance)+"<MAX_DIS: MoveForward");
      }
      if(distance > MAX_DIS && IsSleep == false)
      {
        unsigned long t = millis();
        if(valueBT == CHAR_TURNLEFT && t - _t1 > 500)
        {
          valueBT = CHAR_TURNRIGHT;
          _t1 = t;
          Serial.println("TURN_RIGHT");
        }
        else if(valueBT == CHAR_TURNRIGHT && t - _t1 > 1000)
        {
          Serial.println(String(distance) + " - 3");
          valueBT = CHAR_STOP;
          IsSleep = true;
          Serial.println("TURN_STOP");
        }
        else if(valueBT != CHAR_TURNLEFT && valueBT != CHAR_TURNRIGHT)
        {
          valueBT = CHAR_TURNLEFT;
          _t1 = millis();
          Serial.println("TURN_LEFT");
        }

          //Serial.println(String(distance) + ">MIN_MAX: Turn");
        
        //timeProcess = 1000;
        
      }
}
else if(MODE == AVOIDING_MODE)
{
  if(distance <= AVOIDING_DIS){
    valueBT = CHAR_TURNLEFT;
    timeProcess = 500;
    Serial.println("TURN LEFT");
  }
  else{
    valueBT = CHAR_FORWARD;
    Serial.println("FORWARD");
  }
}

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
      
delay(timeProcess);
}
