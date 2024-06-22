#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int turnspeed = 117;//97
int forwardspeed = 103;//83
int balancespeed = 40;
#define IN1 9
#define IN2 6
#define IN3 5
#define IN4 3
#define IR1 A0
#define IR2 A1
#define hita 4 //for startstop
#define hitb 2 //for loading
#define Trig 11
#define Echo 10

#define buzzer A3 //buzzer will be connected to A3 on W/L
#define tempo 220
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
int melody[] = {
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

long pulse;
int distance;
int Move = 0;
int programmed = 0;
int station = 4;
int stationcurrent = 0;
int array[] = {0, 0, 0, 0};
int x = 1;
int loaded = 0;


void startstop(void);
void programming(void);
void load(void);
void disp(void);
int notenough(void);
void forward(void);
void leftline(void);
void rightline(void);
void detection(void);
void brake(void);
void nokia(void);

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT); //Ultrasound
  pinMode(Echo, INPUT);
  pinMode(IN1, OUTPUT); //Motor ( i have no clue which is pwm)
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  lcd.init(); // initialise the LCD
  lcd.setCursor(0, 0); //position cursor at top row left hand corner
  lcd.begin(16, 2); // 16 characters, 2 lines
  lcd.backlight(); // turn backlight on

  pinMode(hita, INPUT);
  pinMode(hitb, INPUT);

}

void loop() {
  if (programmed == 0) //make sure it only runs once
  {
    Serial.println("\nEnter boxes for stations 1,2,3,4 respectively");
    Serial.println("Hit enter after each station");
    lcd.print("Awaiting input"); //note to user
    lcd.setCursor(0, 0);
    programming();

  }
  load();
  disp();

  forward();

  if ((digitalRead(IR2) == 1) && (Move == 1)) //follows black line
  {
    leftline();
  }

  if ((digitalRead(IR1) == 1) && (Move == 1)    || (digitalRead(IR1) == 1) && ((digitalRead(IR2) == 1)) && (Move == 1))
  {
    rightline();
  }

  if (!digitalRead(hita)) startstop(); //shoot to start and stop

  detection(); //measures distance
  brake();
  if (!digitalRead(hita)) startstop();

}
void programming() { //REMEMBER TO SET TO "NO LINE ENDING" OR ELSE WONT WORK

  while (Serial.available() == 0) {} // while the buffer/storage has 0 input ( 0 digits)
  while (Serial.available() > 0) {
    for (int i = 0; i < station; i++) {
      while (Serial.available() == 0) {}
      array[i] =   Serial.parseInt(SKIP_ALL);

      if (x == station) {}
      else {
        if (Serial.available() == 0)
        {
          x++;
        }
        Serial.println("Please enter next input");
      }
    }
  }
  Serial.print("Boxes for the staions respectively are ");
  for (int i = 0; i < 4; i++) {
    Serial.print(", ");
    Serial.print(array[i]);
  }
  programmed = 1;
}
void forward()
{
  switch (Move) //move forward if shot
  {
    case 0:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      break;
    case 1:
      analogWrite(IN1, forwardspeed);
      digitalWrite(IN2, LOW);
      analogWrite(IN3, forwardspeed + balancespeed); // because of hardware problems, both wheels are not supplied the same speed
      digitalWrite(IN4, LOW);
  }
}
void detection()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig, LOW);
  pulse = pulseIn(Echo, HIGH);
  distance = pulse / 58;
  delay(10);
}
void brake()
{
  if (distance < 15)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    while (distance < 15)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Obstructed");
      nokia(); //alert to let others know the car is stuck
      delay(500); // lcd updates every 0.5 seconds
      detection(); //continues to measure distance
    }
  }
}
void leftline()
{

  digitalWrite(IN2, LOW);
  analogWrite(IN1, turnspeed);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, LOW);



}
void rightline()
{

  digitalWrite(IN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(IN3, turnspeed + balancespeed);


}

void startstop() {
  int canmove = 0;
  
  switch (Move)
  {
    case 0:
      canmove = notenough(); //make sure correct number of boxes are present
      if (canmove == 0)
      {
        Move = 1;
        stationcurrent++; //moves to next station
        loaded = 0; //reset number of loaded boxes
      }
      break;
    case 1:
      Move = 0;
      break;
  }
  while (!digitalRead(hita));
  forward();
  delay(150);

}
void load()
{
  int t = 0; //timer
  int f = 0; //!loaded/minus
  if (!digitalRead(hitb))
  {
    while (!digitalRead(hitb));
    delay(10);
    while (t < 800) //if shoot again within 0.4s, unload
    {
      if (!digitalRead(hitb))
      {
        while (!digitalRead(hitb));
        delay(10);
        loaded--;
        f = 1;
        delay(300);//prevent spam
        break;
      }
      t++;
      delayMicroseconds(500);
    }
    if (f == 0) loaded++;

  }
}
void disp()
{
  int timer= 0;
  if (stationcurrent > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("No of boxes: ");
    lcd.print(loaded);
    lcd.print("/");
    lcd.print(array[stationcurrent - 1]); //it now displays the expected number of boxes at that station
    lcd.setCursor(0, 1); //NEW LINE
    lcd.print("Station ");
    lcd.print(stationcurrent); //prints the current/upcoming station
    lcd.setCursor(0, 0);
    if (stationcurrent> station)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Finished");
      stationcurrent = 0;
      while(timer<6000)
      {
        forward();
        if ((digitalRead(IR2) == 1) && (Move == 1)) //follows black line
        {
        leftline();
        }

        if ((digitalRead(IR1) == 1) && (Move == 1)    || (digitalRead(IR1) == 1) && ((digitalRead(IR2) == 1)) && (Move == 1))
        {
        rightline();
        }
        timer++;
        delayMicroseconds(500);
      }
      Move = 0;
      forward();
      lcd.setCursor(0, 0);
      lcd.print("Car stopped.");
      lcd.setCursor(0, 1); //NEW LINE
      lcd.print("Reset to restart");
      while (stationcurrent == 0); // end of program
    }
  }
  else
  {
    lcd.print("Shoot to start"); //note to user
    lcd.setCursor(0, 0);
    if (!digitalRead(hita)) startstop();
  }
  if (loaded < 0) loaded = 0;
}
int notenough()
{
  int o = 0; //!correct number of boxes
  if ((loaded < array[stationcurrent - 1]) && (stationcurrent > 0)) //if loaded boxes<expeted boxes
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NOT ENOUGH BOXES");
    delay(2000);
    o = 1;
    return 1;
  }
  if ((loaded > array[stationcurrent - 1]) && (stationcurrent > 0)) //if loaded boxes>expected boxes
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TOO MANY BOXES");
    delay(2000);
    o = 1;
    return 1;
  }
  if (o == 0) return 0;
}
void nokia()
{
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the speciefied duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
