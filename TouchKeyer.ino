#include <CapacitiveSensor.h>
#include <stdio.h>

int led = 3;
int iambicL = 9;
int iambicR = 10;

// WPM in the context of morse code is how many times a 50 unit word can be sent in a minute.
// so the formula would be 60000ms/(50u*WPM) is the milliseconds per unit
// a dit is a unit, a dah is 3 units, a character break is 1 units
int wpm = 15;
int dit = 60000/(wpm*50); // 1 unit
int dah = 3*(60000/(wpm*50)); // 3 units

// this variable shows the mode of the keyer.
// 0: iambic paddle mode – just acts as an iambic paddle, acting as two switches with common ground
// 1: touch keyer mode - acts as a keyer, plugs into transceiver as straight key.
// 2: keyer mode - a separate iambic paddle can be plugged into the keyer
enum modes{iambicpaddle, touchkeyer, keyer};
enum modes mode = iambicpaddle; // default/initial mode is iambicpaddle mode

// button state variable
enum buttonStates{RL, LR, R, L, off};
enum buttonStates buttonState = off;

// initialize capacitive sensor objects
CapacitiveSensor LTouchPaddle = CapacitiveSensor(4,2); // Left Paddle
CapacitiveSensor RTouchPaddle = CapacitiveSensor(4,6); // Right Paddle

void setup()                    
{
  // calibrate the sensors
  LTouchPaddle.set_CS_AutocaL_Millis(0xFFFFFFFF);
  RTouchPaddle.set_CS_AutocaL_Millis(0xFFFFFFFF);
  // setup the led output pin
  pinMode(led, OUTPUT);
  pinMode(iambicL, OUTPUT);
  pinMode(iambicR, OUTPUT);
}

// checks for touch
void checktouch(){

  // Left is pressed
  if (LTouchPaddle.capacitiveSensor(30)>60 && RTouchPaddle.capacitiveSensor(30)<=60){buttonState = L;}
  // Right is pressed
  else if (RTouchPaddle.capacitiveSensor(30)>60 && LTouchPaddle.capacitiveSensor(30)<=60){buttonState = R;}

  // both are pressed and left was first
  if (LTouchPaddle.capacitiveSensor(30)>60 && RTouchPaddle.capacitiveSensor(30)>60 && buttonState == L){buttonState = LR;}
  // both are pressed and right was first
  else if (LTouchPaddle.capacitiveSensor(30)>60 && RTouchPaddle.capacitiveSensor(30)>60 && buttonState == R){buttonState = RL;}

  // no key pressed
  if (LTouchPaddle.capacitiveSensor(30)<=60 && RTouchPaddle.capacitiveSensor(30)<=60){buttonState = off;}
}

// keyer output
void morse_output(){
  switch(buttonState){
    case L: digitalWrite(led, HIGH); delay(dit); digitalWrite(led, LOW); break; // dit
    
    case R: digitalWrite(led, HIGH); delay(dah); digitalWrite(led, LOW); break; // dah
    
//    case LL: digitalWrite(led, HIGH); delay(dit); digitalWrite(led, LOW); delay(dit); break; // dit dit
//    case RR: digitalWrite(led, HIGH); delay(); digitalWrite(led, LOW); delay(dit); break; // dah dah

    case LR: digitalWrite(led, HIGH); delay(dit); digitalWrite(led, LOW); // ditdah
             delay(dit); // char pause
             digitalWrite(led, HIGH); delay(dit); digitalWrite(led, LOW); delay(dit); break; // 
             
    case RL: digitalWrite(led, HIGH); delay(dah); digitalWrite(led, LOW); // dahdit
             delay(dit); // char pause
             digitalWrite(led, HIGH); delay(dit); digitalWrite(led, LOW); delay(dit); break;
             
    case off: digitalWrite(led, LOW); break;
    default: break;
  }
}

// mode 0
void iambicPaddle(){
  checktouch();
  if(buttonState == R){digitalWrite(iambicR, HIGH); digitalWrite(iambicL, LOW); Serial.write('R');}
  else if(buttonState == L){digitalWrite(iambicL, HIGH); digitalWrite(iambicR, LOW); Serial.write('L');}
  else if(buttonState == RL || buttonState == LR){digitalWrite(iambicR, HIGH); digitalWrite(iambicL, HIGH); Serial.write('B');}
  else{digitalWrite(iambicR, LOW); digitalWrite(iambicL, LOW); Serial.write('n');}
}
//
//// mode 1
//void touchkeyer(){
//
//}
//
//// mode 2
//void keyer(){
//  
//}
//
//void morsecode(){
//}

void loop()                    
{
  iambicPaddle();
}
