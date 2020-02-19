#define lightPin A9
int lightValue = 0;

void ReadLight(){
  lightValue = analogRead(lightPin); // 0 (full black) - 1023 (full white)
}
