#define motorE 4
#define motorD 9
#define motorEs1 5
#define motorEs2 6
#define motorDs1 7
#define motorDs2 8
#include <Stepper.h>
int RotationsCounter=0;
Stepper Deployer(200, 10 , 11, 12, 13); 

void SetMotorsForward(){
  digitalWrite(motorDs1, LOW);
  digitalWrite(motorDs2, HIGH);
  digitalWrite(motorEs1, LOW);
  digitalWrite(motorEs2, HIGH);
}

void SetMotorsBackwards(){
  digitalWrite(motorDs1, HIGH);
  digitalWrite(motorDs2, LOW);
  digitalWrite(motorEs1, HIGH);
  digitalWrite(motorEs2, LOW);
}

void SetMotorsTurnRight(){
  digitalWrite(motorDs1, HIGH);
  digitalWrite(motorDs2, LOW);
  digitalWrite(motorEs1, LOW);
  digitalWrite(motorEs2, HIGH);
}

void SetMotorsTurnLeft(){
  digitalWrite(motorDs1, LOW);
  digitalWrite(motorDs2, HIGH);
  digitalWrite(motorEs1, HIGH);
  digitalWrite(motorEs2, LOW);
}

void Stop(){
  analogWrite(motorE, 0);
  analogWrite(motorD, 0);
}

void DeployKit(){
  Deployer.setSpeed(90);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(200);
  Deployer.step(50);
}

