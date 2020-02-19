#define infraredLPin 0
#define infraredFPin 0
#define infraredRPin 0
int IRFront;
int IRFront_inches;
int IRLeft;
int IRRight;

void ReadInfrared(){
  IRFront = round(10650.08*pow(analogRead(infraredFPin), -0.935)-10);
  IRLeft = round(10650.08*pow(analogRead(infraredLPin), -0.935)-10);
  IRRight = round(10650.08*pow(analogRead(infraredRPin), -0.935)-10);
  IRFront += 5;
  IRLeft += 5;
  IRRight += 5;
  IRFront_inches = round(IRFront*2.54);
}

