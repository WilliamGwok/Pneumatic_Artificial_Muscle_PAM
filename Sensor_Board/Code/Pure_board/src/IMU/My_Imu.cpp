#include "My_Imu.h"

Adafruit_BNO055 sensor = Adafruit_BNO055();

#define Imu_Print_Data 1

void My_Imu_Posture_Data_Update();
void My_Imu_Calibration();
void My_Imu_Data_Serial_Print();

Imu_Data_t My_Imu_Data;

void My_Imu_Init()
{
  sensor.begin();

  delay(500);

  sensor.setExtCrystalUse(true);//启用外部晶振
}

void My_Imu_Update()
{
  My_Imu_Posture_Data_Update();

  My_Imu_Calibration();

  #if Imu_Print_Data
    My_Imu_Data_Serial_Print();
  #endif
}

void My_Imu_Posture_Data_Update()
{
  //四元数
  imu::Quaternion quat = sensor.getQuat();

  quat.normalize ();

  My_Imu_Data.w = quat.w();
  My_Imu_Data.x = quat.x();
  My_Imu_Data.y = quat.y();
  My_Imu_Data.z = quat.z();

  //欧拉角
  //euler = sqrt(yaw * yaw + pitch * pitch + roll * roll);
  imu::Vector<3> euler_v = quat.toEuler();   
  My_Imu_Data.yaw = euler_v.y() * 180/M_PI;         
  My_Imu_Data.pitch = euler_v.x() * 180/M_PI;        
  My_Imu_Data.roll = euler_v.z() * 180/M_PI; 

  //角速度
  imu::Vector<3> gyro_v = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  My_Imu_Data.gyrox = gyro_v.x();
  My_Imu_Data.gyroy = gyro_v.y();
  My_Imu_Data.gyroz = gyro_v.z();

  //角加速度
  imu::Vector<3> acc_v = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  My_Imu_Data.accx = acc_v.x();
  My_Imu_Data.accy = acc_v.y();
  My_Imu_Data.accz = acc_v.z();
}

void My_Imu_Calibration()
{
  uint8_t system, gyro, accel, mg = 0;

  if(My_Imu_Data.is_cali == 1)
  {
    system = 3;
  }else
  {
    sensor.getCalibration(&system, &gyro, &accel, &mg);

    if(system > 2)
    {
      system = 3;
      My_Imu_Data.is_cali = 1;
    }
  }
}

void My_Imu_Data_Serial_Print()
{
  Serial.print(" gyrox: ");
  Serial.print(My_Imu_Data.gyrox, 4);

  Serial.print(" gyroy: ");
  Serial.print(My_Imu_Data.gyroy, 4);

  Serial.print(" gyroz: ");
  Serial.print(My_Imu_Data.gyroz, 4);

  Serial.print(" is_cali: ");
  Serial.print(My_Imu_Data.is_cali);

  Serial.println();
}


