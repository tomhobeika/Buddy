#include <Servo.h>

Servo servoX;
Servo servoZ;
int rotation = 25;
int middle = 100;
int count = 0;

void moveServo(int amount, Servo servo)
{
    amount = amount - middle;
    amount = amount / 50;  
    int move = servo.read() + amount;
    servo.write(move);
    //Serial.println(servo.read());
}

/*void centerServo(){
  int currentLocation = servo.read();
  int amount = 90 - currentLocation;
  servo.write(-amount);
}*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");              

  //SERVO X
  //Serial.println(servoX.read());
  servoX.attach(11);
  servoX.write(90);
  //Serial.println(servoX.read());

  // SERVO Z
  //Serial.println(servoZ.read());
  servoZ.attach(9);
  servoZ.write(90);
  //Serial.println(servoZ.read());
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValueX = analogRead(A0);
  int sensorValueZ = analogRead(A1);
  //Serial.println(sensorValueX - middle);
  Serial.println(sensorValueZ - middle);

  // SERVO X
  if(sensorValueX - middle < 100){
    count = 0;
    moveServo(sensorValueX,servoX);
  }
  else if(sensorValueX - middle > 100){
    moveServo(100+middle, servoX);
  }

  // SERVO Z
  if(sensorValueZ - middle < 100){
    count = 0;
    moveServo(sensorValueZ,servoZ);
  }
  else if(sensorValueZ - middle > 100){
    moveServo(100+middle, servoZ);
  }
  //Serial.println(servoX.read());
  //Serial.println(servoZ.read());
  delay(30);        // delay in between reads for stability
}
