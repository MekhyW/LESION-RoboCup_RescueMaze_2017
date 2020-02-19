#define compassXPin 0
#define compassYPin 0
int Compass=360;
int VerticalCompass=90;
int CompassSimplified=3; //1=left 2=right 3=up 4=down
int OrientationError=0;

int ForwardX=5;
int ForwardY=3;
int LeftX=4;
int LeftY=2;
int RightX=6;
int RightY=2;
int BackwardsX=5;
int BackwardsY=1;

void ReadCompass(){
  Compass = analogRead(compassXPin);
  VerticalCompass = analogRead(compassYPin);
  // conversion to degrees
  if(Compass>=315 || Compass<=45){
    CompassSimplified=3;
  } else if(Compass>=45 && Compass<=135){
    CompassSimplified=2;
  } else if(Compass>=135 && Compass<=225){
    CompassSimplified=4;
  } else if(Compass>=225 && Compass<=315){
    CompassSimplified=1;
  }
  if(CompassSimplified==1){
    OrientationError=Compass-270;
  } else if(CompassSimplified==2){
    OrientationError=Compass-90;
  } else if(CompassSimplified==3){
    if(Compass>180){
      OrientationError=Compass-360;
    } else {
      OrientationError=Compass-0;
    }
  } else if(CompassSimplified==4){
    OrientationError=Compass-180;
  }
}

