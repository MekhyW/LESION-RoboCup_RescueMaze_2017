#define temperaturePin 0
int temperatureValue = 20;
int victimcounter = 0;

void ReadTemperature(){
  temperatureValue = analogRead(temperaturePin);
  //conversion to celsius
}
