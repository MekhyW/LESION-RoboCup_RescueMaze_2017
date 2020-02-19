void FlashLED(){
  for(int cont=0; cont<10; cont++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);  
  }
}

void PingLED(){
	for(int cont=0; cont<10; cont++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(25);
    digitalWrite(LED_BUILTIN, LOW);
    delay(25);
  }  
}

void TurnLED_ON(){
  digitalWrite(LED_BUILTIN, HIGH);
}

void TurnLED_OFF(){
  digitalWrite(LED_BUILTIN, LOW);
}


