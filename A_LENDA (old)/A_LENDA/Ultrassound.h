#define ultrassound1TrigPin 0
#define ultrassound1EchoPin 0
#define ultrassound2TrigPin 0
#define ultrassound2EchoPin 0
#define ultrassound3TrigPin 0
#define ultrassound3EchoPin 0
float USLeft;
float USRight;
float USBack;
float WallError;
int IsForwardAvailable=1;
int IsLeftAvailable=0;
int IsRightAvailable=0;

void ReadUltrassound(){
  digitalWrite(ultrassound1TrigPin, LOW);
  digitalWrite(ultrassound2TrigPin, LOW);
  digitalWrite(ultrassound3TrigPin, LOW);
  delay(50);
  digitalWrite(ultrassound1TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrassound1TrigPin, LOW);
  USLeft = pulseIn(ultrassound1EchoPin, HIGH)*0.034/2;
  digitalWrite(ultrassound2TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrassound2TrigPin, LOW);
  USRight = pulseIn(ultrassound2EchoPin, HIGH)*0.034/2;
  digitalWrite(ultrassound3TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrassound3TrigPin, LOW);
  USBack = pulseIn(ultrassound3EchoPin, HIGH)*0.034/2;
  USLeft += 5;
  USRight += 5;
  USBack += 5;
  if(USLeft<15 && USRight<15){
    WallError = USLeft-USRight;
  } else if(USLeft<15){
    WallError = USLeft-15;
  } else if(USRight<15){
    WallError = -(USRight-15);
  } else {
    WallError=0;
  }
}

