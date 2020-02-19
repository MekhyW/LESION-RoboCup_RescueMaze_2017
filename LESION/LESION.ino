//LESION Rescue Maze Robot
//Team CSBS RescueB

#include <Wire.h>
#include <math.h>
#include "Motors.h"
#include "LED.h"
#include "Compass.h"
#include "Ultrassound.h"
#include "Light.h"
#include "Temperature.h"
#include "Button.h"

void setup(){
  Serial.begin(9600);
  Serial.println("CSBC RescueB 4.2.3 - Online");
  pinMode(motorE, OUTPUT);
  pinMode(motorD, OUTPUT);
  pinMode(motorEs1, OUTPUT);
  pinMode(motorEs2, OUTPUT);
  pinMode(motorDs1, OUTPUT);
  pinMode(motorDs2, OUTPUT);
  SetMotorsForward();
  Wire.begin();
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(temperaturePin, INPUT); 
  pinMode(presence_leftPin, INPUT);
  pinMode(presence_rightPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(ButtonPin, INPUT);
  floodfill[currentfloor][PositionX][PositionY] = 1;
  PressToStart();
}

void loop(){
  RotationsCounter=0;
  while(RotationsCounter<13){
  	ReadAll();
  	analogWrite(motorE, (200-(WallError*3)));
    analogWrite(motorD, 200+(WallError*3));
    delay(20);
  	if(USRight<=11 || USLeft<=11){
  		SetMotorsBackwards();
  		analogWrite(motorE, 100);
  		analogWrite(motorD, 100);
  		delay(300);
  		if(USLeft<=USRight){
  			Serial.println("Aligning left...");
  			SetMotorsTurnRight();
  		} else {
  			Serial.println("Aligning right...");
  			SetMotorsTurnLeft();
  		}
     analogWrite(motorE, 150);
     analogWrite(motorD, 150);
     delay(100);
  	  SetMotorsForward();
      analogWrite(motorE, 100);
      analogWrite(motorD, 100);
      delay(300);
      ReadAll();
  	}
  	ReadAll();
    if(lightValue<450 && lightValue>0){
    	Serial.println("Black zone detected! Going back...");
    	BlackZone();
    	break;
  	}
  	else if((lightValue>800 || (lightValue>=600 && lightValue<=650)) && (PositionX!=PositionXBackup || PositionY!=PositionYBackup)){
    	Serial.println("Backup updated!");
    	UpdateBackup();
  	}
  	else if(VerticalCompass!=0 && VerticalCompass_last!=0 && ((VerticalCompass>110 && VerticalCompass_last>110) || (VerticalCompass<60 && VerticalCompass_last<60))){
   	 	Serial.println("Going on ramp...");
   	 	Ramp();
     	break;
  	}
  	else if(((digitalRead(22)==HIGH && USRight<=20) || (digitalRead(26)==HIGH && USLeft<=20)) && gamefloor[currentfloor][PositionX][PositionY]<100 && gamefloor[currentfloor][BackwardsX][BackwardsY]<100){
  		Serial.println("Victim found!");
  		SetMotorsBackwards();
  		analogWrite(motorE, 100);
  		analogWrite(motorD, 100);
  		Stop();
  		SetMotorsForward();
     	Serial.println("Depositing now...");
     	TurnLED_ON();
    	FlashLED();
    	TurnLED_OFF();
    	gamefloor[currentfloor][PositionX][PositionY]+=100;
    	victimcounter++;
  	}
  	RotationsCounter++;
  }
  Serial.println("Tile achieved!");
  CalculateDisplacement();
  Stop();
  PingLED();
  if(PositionX==15 && PositionY==15 && currentfloor==1 && (victimcounter>5 || millis()>400000)){
   Serial.println("Fetching exit bonus...");
   Stop();
   FlashLED();
   FlashLED();
  }
  NextMove();
}











void ReadAll(){
  ReadLight();
  ReadUltrassound();
  ReadTemperature();
  
  RelativeXY();
  CheckBackup();
}

void Ramp(){
	SetMotorsBackwards();
	analogWrite(motorE, 150);
	analogWrite(motorD, 150);
	delay(400);
    ReadAll();
  	if(USLeft<=USRight){
  		SetMotorsTurnRight();
  	} else {
  		SetMotorsTurnLeft();
  	}
	SetMotorsTurnRight();
 analogWrite(motorE, 250);
 analogWrite(motorD, 250);
 delay(900);
 SetMotorsBackwards();
  analogWrite(motorE, 250);
  analogWrite(motorD, 250);
  delay(2000);
  ReadAll();
  while(VerticalCompass>=100 && VerticalCompass_last>=100){
  	analogWrite(motorE, 255);
    analogWrite(motorD, 255);
    ReadAll();
  }
  while(VerticalCompass<=60 && VerticalCompass_last<=60){
  	analogWrite(motorE, 255);
    analogWrite(motorD, 255);
    ReadAll();
  }
  if(currentfloor==1){
    currentfloor++;
  } else if(currentfloor==2){
    currentfloor--;
  }
}

void BlackZone(){
  PositionX=BackwardsX;
  PositionY=BackwardsY;
  gamefloor[currentfloor][ForwardX][ForwardY]=16;
  SetMotorsBackwards();
  analogWrite(motorE, 200);
  analogWrite(motorD, 200);
  delay(340);
  SetMotorsForward();
  ReadAll();
}

void NextMove(){
  ReadAll();
  if(USFront<30){
    analogWrite(motorE, 250);
    analogWrite(motorD, 250);
    delay(1000);
    SetMotorsBackwards();
    ReadAll();
    while(USFront<13){
      Serial.println(USFront);
    	analogWrite(motorE, 100);
    	analogWrite(motorD, 100);
    	ReadAll();
    }
    SetMotorsForward();
  }
  IsForwardAvailable=1;
  IsRightAvailable=1;
  IsLeftAvailable=1;
  if((USFront<30) || gamefloor[currentfloor][ForwardX][ForwardY]==16){
    IsForwardAvailable=0;
  }
  if((USLeft<30) || gamefloor[currentfloor][LeftX][LeftY]==16){
    IsLeftAvailable=0;
  }
  if((USRight<30) || gamefloor[currentfloor][RightX][RightY]==16){
    IsRightAvailable=0;
  }
  if(IsForwardAvailable==0 && IsLeftAvailable==0 && IsRightAvailable==0){
    TurnBackwards();
  }
  else if(IsForwardAvailable==0 && IsLeftAvailable==0 && IsRightAvailable==1){
    TurnRight();
  }
  else if(IsForwardAvailable==0 && IsLeftAvailable==1 && IsRightAvailable==0){
    TurnLeft();
  }
  else if(IsForwardAvailable==1 && IsLeftAvailable==1 && IsRightAvailable==0 && floodfill[currentfloor][LeftX][LeftY]<floodfill[currentfloor][ForwardX][ForwardY]){
    TurnLeft();
  }
  else if(IsForwardAvailable==1 && IsLeftAvailable==0 && IsRightAvailable==1 && floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][ForwardX][ForwardY]){
    TurnRight();
  }
  else if(IsForwardAvailable==0 && IsLeftAvailable==1 && IsRightAvailable==1){
    if(floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][LeftX][LeftY] || (floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][LeftX][LeftY] && sqrt(((LeftX-4)*(LeftX-4))+((LeftY-1)*(LeftY-1)))<sqrt(((RightX-4)*(RightX-4))+((RightY-1)*(RightY-1))))){
      TurnRight();
    }
    else {
      TurnLeft();
    }
  }
  else if(IsForwardAvailable==1 && IsLeftAvailable==1 && IsRightAvailable==1){
    if((floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][LeftX][LeftY]||(floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][LeftX][LeftY] && sqrt(((LeftX-4)*(LeftX-4))+((LeftY-1)*(LeftY-1)))<sqrt(((RightX-4)*(RightX-4))+((RightY-1)*(RightY-1))))) && (floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][ForwardX][ForwardY]||(floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][ForwardX][ForwardY] && sqrt(((ForwardX-4)*(ForwardX-4))+((ForwardY-1)*(ForwardY-1)))<sqrt(((RightX-4)*(RightX-4))+((RightY-1)*(RightY-1)))))){
      TurnRight();
    }
    else if((floodfill[currentfloor][LeftX][LeftY]<floodfill[currentfloor][RightX][RightY]||(floodfill[currentfloor][LeftX][LeftY]==floodfill[currentfloor][RightX][RightY] && sqrt(((LeftX-4)*(LeftX-4))+((LeftY-1)*(LeftY-1)))>sqrt(((RightX-4)*(RightX-4))+((RightY-1)*(RightY-1))))) && (floodfill[currentfloor][LeftX][LeftY]<floodfill[currentfloor][ForwardX][ForwardY]||(floodfill[currentfloor][LeftX][LeftY]==floodfill[currentfloor][ForwardX][ForwardY] && sqrt(((LeftX-4)*(LeftX-4))+((LeftY-1)*(LeftY-1)))>sqrt(((ForwardX-4)*(ForwardX-4))+((ForwardY-1)*(ForwardY-1)))))){
      TurnLeft();
    }
  }
  ReadAll();
  if(USBack<30){
    SetMotorsBackwards();
    analogWrite(motorE, 250);
    analogWrite(motorD, 250);
    delay(1000);
    SetMotorsForward();
    ReadAll();
    while(USBack<15){
      Serial.println(USBack);
    	analogWrite(motorE, 100);
    	analogWrite(motorD, 100);
    	ReadAll();
    }
  }
}

void TurnBackwards(){
  Serial.println("Turning backwards...");
  if(USLeft>USRight && gamefloor[LeftX][LeftY]!=16){
    SetMotorsTurnLeft();
  } else {
    SetMotorsTurnRight();
  }
  analogWrite(motorE, 250);
  analogWrite(motorD, 250);
  delay(380);
  ReadAll();
  if(USBack<30){
    SetMotorsBackwards();
    analogWrite(motorE, 250);
    analogWrite(motorD, 250);
    delay(500);
    SetMotorsForward();
    ReadAll();
    while(USBack<15){
      analogWrite(motorE, 100);
      analogWrite(motorD, 100);
      ReadAll();
    }
  }
  if(USLeft>USRight && gamefloor[ForwardX][ForwardY]!=16){
    SetMotorsTurnLeft();
  } else {
    SetMotorsTurnRight();
  }
  analogWrite(motorE, 250);
  analogWrite(motorD, 250);
  delay(380);
  Stop();
  delay(100);
  SetMotorsForward();
  if(Orientation==1){
    Orientation=2;
  } else if(Orientation==2){
    Orientation=1;
  } else if(Orientation==3){
    Orientation=4;
  } else if(Orientation==4){
    Orientation=3;
  }
}

void TurnLeft(){
	Serial.println("Turning left...");
    SetMotorsTurnLeft();
    analogWrite(motorE, 250);
    analogWrite(motorD, 250);
    delay(380);
    Stop();
  	delay(100);
    SetMotorsForward();
    if(Orientation==1){
      Orientation=4;
    } else if(Orientation==2){
      Orientation=3;
    } else if(Orientation==3){
      Orientation=1;
    } else if(Orientation==4){
      Orientation=2;
    }
}

void TurnRight(){
	Serial.println("Turning right...");
    SetMotorsTurnRight();
    analogWrite(motorE, 250);
    analogWrite(motorD, 250);
    delay(380);
    Stop();
  	delay(100);
    SetMotorsForward();
    if(Orientation==1){
      Orientation=3;
    } else if(Orientation==2){
      Orientation=4;
    } else if(Orientation==3){
      Orientation=2;
    } else if(Orientation==4){
      Orientation=1;
    }
}

void CalculateDisplacement(){
  ReadAll();
  Serial.print(PositionX);
  Serial.print(" ");
  Serial.println(PositionY);
  PositionX=ForwardX;
  PositionY=ForwardY;
  ReadAll();
  if(floodfill[currentfloor][PositionX][PositionY]==0){
  	floodfill[currentfloor][PositionX][PositionY] = floodfill[currentfloor][BackwardsX][BackwardsY]+1;
  }
}

void RelativeXY(){
  if(Orientation==1){
    LeftX=PositionX;
    LeftY=PositionY-1;
    RightX=PositionX;
    RightY=PositionY+1;
    ForwardX=PositionX-1;
    ForwardY=PositionY;
    BackwardsX=PositionX+1;
    BackwardsY=PositionY;
  } else if(Orientation==2){
    LeftX=PositionX;
    LeftY=PositionY+1;
    RightX=PositionX;
    RightY=PositionY-1;
    ForwardX=PositionX+1;
    ForwardY=PositionY;
    BackwardsX=PositionX-1;
    BackwardsY=PositionY;
  } else if(Orientation==3){
    LeftX=PositionX-1;
    LeftY=PositionY;
    RightX=PositionX+1;
    RightY=PositionY;
    ForwardX=PositionX;
    ForwardY=PositionY+1;
    BackwardsX=PositionX;
    BackwardsY=PositionY-1;
  } else if(Orientation==4){
    LeftX=PositionX+1;
    LeftY=PositionY;
    RightX=PositionX-1;
    RightY=PositionY;
    ForwardX=PositionX;
    ForwardY=PositionY-1;
    BackwardsX=PositionX;
    BackwardsY=PositionY+1;
  }
}
