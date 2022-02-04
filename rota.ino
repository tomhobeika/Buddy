/*******Interrupt-based Rotary Encoder Sketch*******
by Simon Merrett, based on insight from Oleg Mazurov, Nick Gammon, rt, Steve Spence
*/
#include <Bounce2.h> // https://github.com/thomasfredericks/Bounce2
Bounce button = Bounce(); 

#include <Servo.h>
Servo servoX;
Servo servoY;

int mult = 1;

static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

boolean buttonPressed = false;
boolean xAxis = true;

void setup() {
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode (A0, INPUT_PULLUP); // Pushdown
  button.attach(A0, INPUT_PULLUP);
  servoX.attach(10);
  servoX.write(5);

  servoX.attach(11);
  servoX.write(5);
  
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  Serial.begin(9600); // start the serial monitor link
}

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void loop(){
  if(oldEncPos != encoderPos) {
    if(encoderPos >= 180 / mult){
      encoderPos = 180 / mult;
    }
    if(encoderPos <= 5){
      encoderPos = 5;
    }
    if(xAxis == true){
      Serial.println(encoderPos);
      servoX.write(encoderPos * mult);
      oldEncPos = encoderPos;
    }
    if(xAxis == false){
      Serial.println(encoderPos);
      servoY.write(encoderPos * mult);
      oldEncPos = encoderPos;
    }
  }
  button.update();
  if(!button.read()){
    buttonPressed = true;
    Serial.println("Swapped Axis!");
    Serial.println(servoX.read());
    Serial.println(servoY.read());
    if(xAxis == true){
      Serial.println("Swapped OFF");
      xAxis = false;
    }
    else if(xAxis == false){
      Serial.println("Swapped ON");
      xAxis = true;
    }
    
    while(buttonPressed){
      button.update();
      
      if(!button.read()){
        
      }
      else{
        break;
      }
    }
    buttonPressed = false;
  }
  
}
