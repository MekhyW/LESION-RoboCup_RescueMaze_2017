#include <stdio.h>
#include <math.h>
#define pi 3.141592
#include "Motors.h"
#include "LED.h"
#include "Compass.h"
#include "Infrared.h"
#include "Ultrassound.h"
#include "Light.h"
#include "Temperature.h"
#include "Backup.h"

void setup(){
  Serial.begin(9600);
  Serial.println("CSBC RescueB 2.1 - Online");
  pinMode(motorE, OUTPUT);
  pinMode(motorD, OUTPUT);
  pinMode(motorKit, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(compassXPin, INPUT);
  pinMode(compassYPin, INPUT);
  pinMode(temperaturePin, INPUT); 
  pinMode(lightPin, INPUT);
  pinMode(infraredFPin, INPUT);
  pinMode(infraredRPin, INPUT);
  pinMode(infraredLPin, INPUT);
  pinMode(ultrassound1TrigPin, OUTPUT);
  pinMode(ultrassound2TrigPin, OUTPUT);
  pinMode(ultrassound3TrigPin, OUTPUT);
  pinMode(ultrassound1EchoPin, INPUT);
  pinMode(ultrassound2EchoPin, INPUT);
  pinMode(ultrassound3EchoPin, INPUT);
  pinMode(backupButtonPin, INPUT);
  floodfill[currentfloor][PositionX][PositionY] = 1;
}

void loop(){
  ReadLight();
  ReadUltrassound();
  ReadInfrared();
  ReadTemperature();
  ReadCompass();
  RelativeXY();
  CheckBackup();
  if(lightValue>700){
    BlackZone();
  }
  else if(lightValue<50 && lightValue>0 && (PositionX!=PositionXBackup || PositionY!=PositionYBackup)){
    UpdateBackup();
  }
  else if((IRFront-15)%30==0 || IRFront<10){
    CalculateDisplacement();
    CalculateWallCase();
    if(temperatureValue>30 && gamefloor[currentfloor][PositionX][PositionY]<100){
       Stop();
       FlashLED();
       DeployKit();
       gamefloor[currentfloor][PositionX][PositionY]+=100;
       victimcounter++;
    }
    if(PositionX==5 && PositionY==2 && (victimcounter>5 || millis()>400000)){
    	Stop();
    	FlashLED();
    	FlashLED();
    }
    NextMove();
    while((IRFront-15)%30==0){
      analogWrite(motorE, 100);
      analogWrite(motorD, 100);
      ReadInfrared();
    }
  }
  else if(VerticalCompass!=0 && (VerticalCompass>110 || VerticalCompass<70)){
    Ramp();
  }
  else {
    MoveForward();
  }
}











void MoveForward(){
    analogWrite(motorE, 150-(WallError*3));
    analogWrite(motorD, 150+(WallError*3));
}

void Ramp(){
  while(VerticalCompass>=110 || VerticalCompass<=70){
    analogWrite(motorE, 150-(WallError*3));
    analogWrite(motorD, 150+(WallError*3));
    ReadCompass();
    ReadUltrassound();
  }
  if(currentfloor==1){
    currentfloor++;
  } else if(currentfloor==2){
    currentfloor--;
  }
}

void BlackZone(){
  PositionX=ForwardX;
  PositionY=ForwardY;
  gamefloor[currentfloor][ForwardX][ForwardY]=16;
  while((IRFront-10)%30!=0){
      analogWrite(motorE, -50);
      analogWrite(motorD, -50);
      ReadInfrared();
  }
}

void NextMove(){
  if(IRFront<15){
    analogWrite(motorE, 50);
    analogWrite(motorD, 50);
    delay(2000);
    analogWrite(motorE, -50);
    analogWrite(motorD, -50);
    delay(1000);
  }
  IsForwardAvailable=1;
  IsRightAvailable=1;
  IsLeftAvailable=1;
  if(IRFront<=15 || gamefloor[currentfloor][ForwardX][ForwardY]==16){
    IsForwardAvailable=0;
  }
  if(IRLeft<=15 || gamefloor[currentfloor][LeftX][LeftY]==16){
    IsLeftAvailable=0;
  }
  if(IRRight<=15 || gamefloor[currentfloor][RightX][RightY]==16){
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
    if(floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][LeftX][LeftY] || (floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][LeftX][LeftY] && IRRight>IRLeft)){
      TurnRight();
    }
    else {
      TurnLeft();
    }
  }
  else if(IsForwardAvailable==1 && IsLeftAvailable==1 && IsRightAvailable==1){
    if((floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][LeftX][LeftY]||(floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][LeftX][LeftY] && IRRight>IRLeft)) && (floodfill[currentfloor][RightX][RightY]<floodfill[currentfloor][ForwardX][ForwardY]||(floodfill[currentfloor][RightX][RightY]==floodfill[currentfloor][ForwardX][ForwardY] && IRRight>IRFront))){
      TurnRight();
    }
    else if((floodfill[currentfloor][LeftX][LeftY]<floodfill[currentfloor][RightX][RightY]||(floodfill[currentfloor][LeftX][LeftY]==floodfill[currentfloor][RightX][RightY] && IRLeft>IRRight)) && (floodfill[currentfloor][LeftX][LeftY]<floodfill[currentfloor][ForwardX][ForwardY]||(floodfill[currentfloor][LeftX][LeftY]==floodfill[currentfloor][ForwardX][ForwardY] && IRLeft>IRFront))){
      TurnLeft();
    }
  }
  if(USBack<15){
    analogWrite(motorE, -50);
    analogWrite(motorD, -50);
    delay(2000);
    analogWrite(motorE, 50);
    analogWrite(motorD, 50);
    delay(1000);
  }
}

void TurnBackwards(){
  if(CompassSimplified==1){
    while(Compass<80 || Compass>100){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
  if(CompassSimplified==2){
    while(Compass<260 || Compass>280){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }  
  }
  if(CompassSimplified==3){
    while(Compass<170 || Compass>190){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    } 
  }
  if(CompassSimplified==4){
    while((Compass<350&&Compass>180) || (Compass>10&&Compass<180)){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
}

void TurnLeft(){
	if(CompassSimplified==1){
    while(Compass<170 || Compass>190){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
  if(CompassSimplified==2){
    while((Compass<350&&Compass>180) || (Compass>10&&Compass<180)){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }  
  }
  if(CompassSimplified==3){
    while(Compass<260 || Compass>280){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    } 
  }
  if(CompassSimplified==4){
    while(Compass<80 || Compass>100){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
}

void TurnRight(){
	if(CompassSimplified==1){
    while((Compass<350&&Compass>180) || (Compass>10&&Compass<180)){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
  if(CompassSimplified==2){
    while(Compass<170 || Compass>190){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }  
  }
  if(CompassSimplified==3){
    while(Compass<80 || Compass>100){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    } 
  }
  if(CompassSimplified==4){
    while(Compass<260 || Compass>280){
      analogWrite(motorE, 50);
      analogWrite(motorD, -50);
      ReadCompass();
    }
  }
}

void CalculateDisplacement(){
  PositionX=ForwardX;
  PositionY=ForwardY;
  if(floodfill[currentfloor][PositionX][PositionY]==0){
  	floodfill[currentfloor][PositionX][PositionY] = floodfill[currentfloor][BackwardsX][BackwardsY]+1;
  }
}

void CalculateWallCase(){
  if(CompassSimplified==1){
    if((USLeft<=15 || gamefloor[currentfloor][PositionX][PositionY-1]==16) && USRight>15){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=4;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=13;
      	}
      }
    } else if(USLeft>15 && (USRight<=15 || gamefloor[currentfloor][PositionX][PositionY+1]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=3;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=11;
      	}
      }
    } else if((USLeft<=15 || gamefloor[currentfloor][PositionX][PositionY-1]==16) && (USRight<=15 || gamefloor[currentfloor][PositionX][PositionY+1]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=6;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=21;
      	}
      }
    }
  }
  if(CompassSimplified==2){
    if((USLeft<=15 || gamefloor[currentfloor][PositionX][PositionY+1]==16) && USRight>15){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=3;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=12;
      	}
      }
    } else if(USLeft>15 && (USRight<=15 || gamefloor[currentfloor][PositionX][PositionY-1]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=4;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
     	 	gamefloor[currentfloor][PositionX][PositionY]=14;
      	}
      }
    } else if((USLeft<=15 || gamefloor[currentfloor][PositionX][PositionY+1]==16) && (USRight<=15 || gamefloor[currentfloor][PositionX][PositionY-1]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=6;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=22;
      	}
      }
    }
  }
  if(CompassSimplified==3){
    if((USLeft<=15 || gamefloor[currentfloor][PositionX-1][PositionY]==16) && USRight>15){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=1;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=11;
      	}
      }
    } else if(USLeft>15 && (USRight<=15 || gamefloor[currentfloor][PositionX+1][PositionY]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=2;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=12;
      	}
      }
    } else if((USLeft<=15 || gamefloor[currentfloor][PositionX-1][PositionY]==16) && (USRight<=15 || gamefloor[currentfloor][PositionX+1][PositionY]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=5;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=23;
      	}
      }
    }
  }
  if(CompassSimplified==4){
    if((USLeft<=15 || gamefloor[currentfloor][PositionX+1][PositionY]==16) && USRight>15){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=2;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=14;
      	}
      }
    } else if(USLeft>15 && (USRight<=15 || gamefloor[currentfloor][PositionX-1][PositionY]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=1;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=13;
      	}
      }
    } else if((USLeft<=15 || gamefloor[currentfloor][PositionX+1][PositionY]==16) && (USRight<=15 || gamefloor[currentfloor][PositionX-1][PositionY]==16)){
      if(gamefloor[currentfloor][PositionX][PositionY]==0){
      	gamefloor[currentfloor][PositionX][PositionY]=5;
      }
      if(IRFront<=15){
        if(gamefloor[currentfloor][PositionX][PositionY]==0){
      		gamefloor[currentfloor][PositionX][PositionY]=24;
      	}
      }
    }
  }
}

void RelativeXY(){
  if(CompassSimplified==1){
  	LeftX=PositionX;
  	LeftY=PositionY-1;
  	RightX=PositionX;
  	RightY=PositionY+1;
  	ForwardX=PositionX-1;
  	ForwardY=PositionY;
  	BackwardsX=PositionX+1;
  	BackwardsY=PositionY;
  } else if(CompassSimplified==2){
  	LeftX=PositionX;
  	LeftY=PositionY+1;
  	RightX=PositionX;
  	RightY=PositionY-1;
  	ForwardX=PositionX+1;
  	ForwardY=PositionY;
  	BackwardsX=PositionX-1;
  	BackwardsY=PositionY;
  } else if(CompassSimplified==3){
  	LeftX=PositionX-1;
  	LeftY=PositionY;
  	RightX=PositionX+1;
  	RightY=PositionY;
  	ForwardX=PositionX;
  	ForwardY=PositionY+1;
  	BackwardsX=PositionX;
  	BackwardsY=PositionY-1;
  } else if(CompassSimplified==4){
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

