#define temperaturePin A8
#define presence_leftPin 26
#define presence_rightPin 24
int temperatureValue = 20;
int presence_left=0;
int presence_right=0;
int victimcounter = 0;

void ReadTemperature(){
  temperatureValue = round(analogRead(temperaturePin)*0.05);
}
