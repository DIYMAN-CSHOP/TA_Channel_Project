#define PIN_IR 2
#define TIMEOUT 2000

unsigned int count;
int OLD_LOGIC = HIGH;
unsigned long oldt = 0;
int speedRPM = 0;

void InitUI() //Khởi tạo giao diện trên smartphone
{
  Serial.println("*.kwl");
  delay(100);
  Serial.println("clear_panel()");
  delay(100);
  Serial.println("set_grid_size(14,7)");
  delay(100);
  Serial.println("add_text(4,3,large,L,SPEED:,245,240,245,)");
  delay(100);
  Serial.println("add_text(8,3,large,L,rpm,245,240,245,)");
  delay(100);
  Serial.println("add_text(1,0,large,L,NTECH,245,240,245,)");
  delay(100);
  Serial.println("add_text_box(6,3,2,L,Text,245,240,245,s)");
  delay(100);
  Serial.println("set_panel_notes(-,,,)");
  delay(100);
  Serial.println("run()");
  delay(100);
  Serial.println("*");
  delay(100);
}

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(PIN_IR, INPUT);
  Serial.begin(9600);
  delay(1000);
  while (!Serial) {
  }
  InitUI();
}

void loop()
{
  unsigned long t = millis();
  if(t - oldt >= TIMEOUT)
  {
    oldt = t;
    count = 0;
    speedRPM =0;
    Serial.print("*s");
    Serial.println(speedRPM);
  }
  
  int NEW_LOGIC = digitalRead(PIN_IR);
  digitalWrite(13, NEW_LOGIC == LOW);
  if(OLD_LOGIC == HIGH && NEW_LOGIC == LOW)
  {
    ++count;
    if(t - oldt >= 1000)
    {
      double dt = t - oldt;
      speedRPM = (count / (dt/1000.0))*60;  // (count / (dt/1000.0)): số vòng quay / s => (count / (dt/1000.0))*60: số vòng quay / phút
      oldt = t;
      count = 0;
      Serial.print("*s");
      Serial.println(speedRPM);
    }
  }
  OLD_LOGIC = NEW_LOGIC;
  //delay(1);
}
