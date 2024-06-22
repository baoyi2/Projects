#define BUTTONK1 8
#define BUTTONK2 9
#include <Wire.h>
#include "RichShieldTM1637.h"
#define CLK 10 //CLK of the TM 1637 IC connected to D10 of OPEN-SMART UNO R3
#define DIO 11 //DIO of the TM 1637 IC connected to D10 of OPEN-SMART UNO R3
TM1637 disp(CLK, DIO);
#define LED_GREEN 5

using namespace std;
int z = 0;
void ssdisp(int count);
int calc(void);
void LED(int);

void setup() {
    pinMode(BUTTONK1, INPUT_PULLUP);
    pinMode(BUTTONK2, INPUT_PULLUP);
    pinMode(LED_GREEN, OUTPUT);
    Serial.begin(9600);
    disp.init();
}

void loop()
{
    
    int a = calc();
    ssdisp(a);
    LED(a);
}

int calc()
{
    int x, y, count;
    count = z;
        if (digitalRead(BUTTONK2) == 0)  
        {
            if (digitalRead(BUTTONK1) == 1) {   //Simulating someone entering
                z += 1;
                delay(1000);
            }

        }

        if (digitalRead(BUTTONK1) == 0) 
        {
            if (digitalRead(BUTTONK2) == 1) {  //Simulating someone entering
                z -= 1; 
                delay(1000);
            }
        }
    return z;
}

void ssdisp(int count1) /* This function converts an integer type variable of maximum 99 into an integer array(so that it can be displayed
seven segment display )and also display the array*/
{
    int8_t Disp[4] = { 0,0,0,0 }; // Declare and initialize array Disp with int type elements 0,0,0,0
    Disp[3] = count1 % 10; 
        while (count1 >= 10)// This while loop is only needed when the number of people in the room, count1 is more than 10
        {
            Disp[2]++; // This computes the third digit of the array by adding it by 1
            count1 = count1 - 10.0;/*This is to prevents an infinite loop the from happening and so that this while loop runs until count1 is
            less than 10 as it works together with the previous line of code to count the third digit of the array*/
        }
    disp.display(Disp); //  Displays the array Disp
    return;
}

void LED(int a)//This function decides whether to switch on or off the green LED depending of the number of people in the room (a)
{
    if (a > 0)
    {
        digitalWrite(LED_GREEN, HIGH);// If there is more than 0 person in the room, the green LED turns on
    }
    else
    {
        digitalWrite(LED_GREEN, LOW);//If the number of people in the room is not more than 0(0 people is in the room) 
    }
    return;
}