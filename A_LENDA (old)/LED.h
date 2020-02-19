#define LEDPin 0

void FlashLED(){
  for(int cont=0; cont<5; cont++){
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
  }
}

