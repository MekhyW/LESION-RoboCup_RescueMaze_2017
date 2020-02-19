#define ButtonPin 31
byte gamefloor[2][30][30] = {0};
byte gamefloorbackup[2][30][30] = {0};
byte floodfill[2][30][30] = {0};
byte floodfillbackup[2][30][30] = {0};
int currentfloor=1;
int PositionX = 15;
int PositionY = 15;
int PositionXBackup = 15;
int PositionYBackup = 15;


void PressToStart(){
	while(digitalRead(ButtonPin) == LOW){
		delay(1);
	}
	while(digitalRead(ButtonPin) == HIGH){
		delay(1);
	}
}

void CheckBackup(){
  if(digitalRead(ButtonPin) == HIGH){
    memcpy(gamefloor, gamefloorbackup, sizeof(gamefloor));
    memcpy(floodfill, floodfillbackup, sizeof(floodfill));
    PositionX=PositionXBackup;
    PositionY=PositionYBackup;
    Serial.print("Seconds ON: ");
    Serial.println(millis()/1000);
    Serial.print(PositionX);
    Serial.print(" ");
    Serial.println(PositionY);
    while(digitalRead(ButtonPin) == HIGH){
      Stop();
      delay(1);
    }
    PressToStart();
  }
}

void UpdateBackup(){
  memcpy(gamefloorbackup, gamefloor, sizeof(gamefloorbackup));
  memcpy(floodfillbackup, floodfill, sizeof(floodfillbackup));
  PositionXBackup=PositionX;
  PositionYBackup=PositionY;
}
