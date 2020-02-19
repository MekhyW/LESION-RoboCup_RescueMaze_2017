#include <NewPing.h>
int USFront;
int USLeft;
int USRight;
int USBack;
int WallError;
NewPing Front(40, 38, 180);
NewPing Back(44, 42, 180);
NewPing Left(52, 50, 180);
NewPing Right(48, 46, 180);

void setup() {
  Serial.begin(9600);
}

void loop(){
  USFront = Front.ping_cm() + 8;
  USBack = Back.ping_cm() + 8;
  USLeft = Left.ping_cm() + 6;
  USRight = Right.ping_cm() + 6;
  Serial.print(USFront);
  Serial.print(" ");
  Serial.print(USBack);
  Serial.print(" ");
  Serial.print(USLeft);
  Serial.print(" ");
  Serial.print(USRight);
  Serial.println(" ");
  delay(10);
}
