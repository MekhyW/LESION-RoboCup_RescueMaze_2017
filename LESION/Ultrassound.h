#include <NewPing.h>
int USFront;
int USLeft;
int USRight;
int USBack;
int WallError;
int IsForwardAvailable=1;
int IsLeftAvailable=0;
int IsRightAvailable=0;
NewPing Front(40, 38, 40);
NewPing Back(44, 42, 40);
NewPing Left(52, 50, 40);
NewPing Right(48, 46, 40);

void ReadUltrassound(){
  USFront = Front.ping_cm() + 8;
  USBack = Back.ping_cm() + 8;
  USLeft = Left.ping_cm() + 6;
  USRight = Right.ping_cm() + 6;
  if(USFront==8){
  	USFront=180;
  }
  if(USBack==8){
  	USBack=180;
  }
  if(USLeft==6){
  	USLeft=180;
  }
  if(USRight==6){
  	USRight=180;
  }
  if(USLeft<=15 && USLeft!=6 && USRight<=15 && USRight!=6){
    WallError = USLeft-USRight;
  } else {
    WallError=0;
  }
}

