#define backupButtonPin 0
byte gamefloor[2][30][30] = {0};
byte gamefloorbackup[2][30][30] = {0};
byte floodfill[2][30][30] = {0};
byte floodfillbackup[2][30][30] = {0};
int currentfloor=1;
int PositionX = 5;
int PositionY = 2;
int PositionXBackup = 5;
int PositionYBackup = 2;

void CheckBackup(){
  if(digitalRead(backupButtonPin) == LOW){
    memcpy(gamefloor, gamefloorbackup, sizeof(gamefloor));
    memcpy(floodfill, floodfillbackup, sizeof(floodfill));
    PositionX=PositionXBackup;
    PositionY=PositionYBackup;
    while(digitalRead(backupButtonPin) == LOW){
      Stop();
      delay(1);
    }
  }
}

void UpdateBackup(){
  memcpy(gamefloorbackup, gamefloor, sizeof(gamefloorbackup));
  memcpy(floodfillbackup, floodfill, sizeof(floodfillbackup));
  PositionXBackup=PositionX;
  PositionYBackup=PositionY;
}
