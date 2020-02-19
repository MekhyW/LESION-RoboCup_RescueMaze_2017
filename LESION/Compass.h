#include <MPU6050.h>
#include <I2Cdev.h>
MPU6050 accelgyro;
int16_t GyroscopeX=0;
int16_t GyroscopeY=0;
int16_t GyroscopeZ=0;
int16_t AccelerationX=0;
int16_t AccelerationY=0;
int16_t AccelerationZ=0;
float VerticalCompass=90;
float VerticalCompass_last=90;
int Orientation=3; //1=left 2=right 3=up 4=down
int ForwardX=5;
int ForwardY=3;
int LeftX=4;
int LeftY=2;
int RightX=6;
int RightY=2;
int BackwardsX=5;
int BackwardsY=1;

void ReadCompass(){
  accelgyro.getMotion6(&AccelerationX, &AccelerationY, &AccelerationZ, &GyroscopeX, &GyroscopeY, &GyroscopeZ);
  VerticalCompass_last = VerticalCompass;
  VerticalCompass = (float)(constrain(map((AccelerationY - 200), -16800, 16800, 0, 180), 0, 180));
}


