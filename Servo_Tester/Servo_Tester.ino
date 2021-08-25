/*
 * Please do not delete the lines below
 * YOUTUBE:   TA CHANNEL
 * 
 */
  
#include <Servo.h>
#include <EEPROM.h>

#define LED_PIN 13
#define INTERRUPT_PIN 2
#define CH1_PORT 8 //Servo1
#define CH2_PORT 9 //Servo2

#define VALSERVO_FORWARD 180
#define VALSERVO_STOP 90
#define VALSERVO_BACKWARD 0

int MODE = 0;
Servo ch1, ch2;
#define ADDR_MEM_MODE 0

void setup(){
  Serial.begin(115200);

  MODE = EEPROM.read(ADDR_MEM_MODE);

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), modeChange, FALLING);
  
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
    MODE=(++MODE)%3;
    bNewModeSetting = true;
    Serial.println("MODE: " + String(MODE));
    EEPROM.write(ADDR_MEM_MODE, MODE);
  }
}

int timeProcess = 0;

unsigned long _t1 = 0;
bool IsSleep = false;

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
  if(MODE == 0)
  {
    ch1.write(VALSERVO_STOP);
    ch2.write(VALSERVO_STOP);  
  }
  else if(MODE == 1)
  {
    ch1.write(VALSERVO_BACKWARD);
    ch2.write(VALSERVO_BACKWARD);  
  }
  else if(MODE == 2)
  {
    ch1.write(VALSERVO_FORWARD);
    ch2.write(VALSERVO_FORWARD);  
  }
delay(timeProcess);
}
