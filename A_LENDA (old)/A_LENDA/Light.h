#define lightPin 0
int lightValue=0;

void ReadLight(){
  lightValue = analogRead(lightPin); // 0 (white) - 1023 (black)
}

