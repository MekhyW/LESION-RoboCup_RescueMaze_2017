#include <NewPing.h>
float USFront;
float USLeft;
float USRight;
float USBack;
float WallError;
NewPing

void setup() {
Wire.begin();
Serial.begin(9600);
Serial.println("Initializing I2C devices...");
accelgyro.initialize();
Serial.println("Testing device connections...");
Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  accel_angle = (float)(constrain(map((ay - 200), -16800, 16800, 0, 180), 0, 180));
  Serial.print(accel_angle);
  Serial.println(" ");
}
