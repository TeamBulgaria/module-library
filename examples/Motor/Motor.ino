#include <Modules.h>
#include <pitches.h>

Motor motor;
int speed=0;
char inc=1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motor.rightStop();
  motor.leftStop();
}

void loop() {
  // put your main code here, to run repeatedly:
  speed+=inc;
  //0-100, positive forward and negative backward
  // motor.leftSpeed(100);
  // motor.rightSpeed(100);
  // delay(1000);
  // motor.leftSpeed(0);
  // motor.rightSpeed(0);
    motor.rightStop();
  motor.leftStop();
  delay(1000);
}