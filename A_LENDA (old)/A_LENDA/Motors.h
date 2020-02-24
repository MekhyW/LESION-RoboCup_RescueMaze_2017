#define motorE 5
#define motorD 6
#define motorKit 0
// int RotationsCounter=0;

void Stop(){
  analogWrite(motorE, 0);
  analogWrite(motorD, 0);
  analogWrite(motorKit, 0);
}

void DeployKit(){
  
}

