/*
 * 4/28/2018 : Logic behind a persistence of vision clock with an infared sensor to count
 * rotations, and LED strip which blinks to output the time, and three buttons to set
 * the time.
 * Crafted with love and care by:
 * Connor Flack (Pumpernickel)
 * John Gallagher (The Suburban Sage)
 * Joseph Sizemore (Kansas)
 * 
 * 12/3/2020 : Updates to code to work with ATtiny microcontroller on custom PCB. Built
 * for JDboards# 11-20494-0
 * 
 * 'Tools' Settings:
 *  BOARD: ATtiny412/402/212/202
 *  Chip: ATtiny402
 *  Clock Speed: 10 MHz Internal
 *  Programmer: jtag2updi (megaTinyCore)   
 * 
 * ATtinyX02 pin mapping: 
 *  => pin# (pin# in code)[Function]
 *           ________
 *        1-|        |-8
 *     (0)2-| ATtiny |-7(4)
 *     (1)3-|   X02  |-6(5)[UPDI]
 *     (2)4-|________|-5(3)
 */
 
#include <TimeLib.h>                  // From Time.h by Michael Margolis/Paul Stroffregen

#define Default_Time 1357041600       // Default time to which set the clock when RST is pressed
#define Rotation_Time 7920
#define Hand_Width 100
#define Increment 132

#define LED_Pin 0
#define RST_Pin 1
#define Hr_Pin  2
#define Min_Pin 3
#define IR_Pin  4

int Sec_delay = 0;
int Min_delay = 0;
int Hr_delay = 0;

void setup() {
  pinMode(LED_Pin,OUTPUT);
  pinMode(RST_Pin,INPUT);
  pinMode(Hr_Pin,INPUT);
  pinMode(Min_Pin,INPUT);
  pinMode(IR_Pin,INPUT);
  setTime(Default_Time);
}

void loop() {
  if (digitalRead(RST_Pin) == HIGH){
    setTime(Default_Time);
    Sec_delay = 7920;
    Min_delay = 7920;
  }
  if (digitalRead(Min_Pin) == HIGH){
    adjustTime(60);
    while (true){
      if (digitalRead(Min_Pin) == LOW){
        break;
      }
    }
  }
  if (digitalRead(Hr_Pin) == HIGH){
    adjustTime(3600);
    while (true){
      if (digitalRead(Hr_Pin) == LOW){
        break;
      }
    }
  }
  
  Sec_delay = Rotation_Time - 132 * second() + (7920/2);
  Min_delay = Rotation_Time - (132 * minute())+ (7920/2);
  Hr_delay = Rotation_Time - 132 * (hour() % 12) * 5 + (7920/2);
  
  if (digitalRead(IR_Pin) == HIGH){
    while ((Sec_delay >= 0) or (Min_delay >= 0) or (Hr_delay >= 0)){
      if ((Sec_delay == 0) or (Min_delay == 0) or (Hr_delay == 0)){
        digitalWrite(LED_Pin,HIGH);
        delayMicroseconds(Hand_Width);
        digitalWrite(LED_Pin,LOW);
        delayMicroseconds(Increment-Hand_Width);
      }
      else{
        delayMicroseconds(132);
      }
      Sec_delay = Sec_delay - 132;
      Min_delay = Min_delay - 132;
      Hr_delay = Hr_delay -132;
    }
  }
}
