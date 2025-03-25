#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>
#include <Math.h>
#include <Wire.h>

void My_Imu_Init();
void My_Imu_Update();

typedef struct
{
  //姿态数据
  float w;
  float x;
  float y;
  float z;

  float roll;
  float pitch;
  float yaw;

  float accx;
  float accy;
  float accz;

  float gyrox;
  float gyroz;
  float gyroy;   

  //校准数据
  int is_cali;
} Imu_Data_t;

extern Imu_Data_t My_Imu_Data;
