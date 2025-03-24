#include "My_Imu.h"

static float roll = 0;
static float pitch = 0;
static float yaw = 0;
static float accx = 0;
static float accy = 0;
static float accz = 0;
static float gyrox = 0;
static float gyroz = 0;
static float gyroy = 0;   
static int is_cali = 0;

Adafruit_BNO055 sensor = Adafruit_BNO055();

void my_imu_init()
{
  sensor.begin();//打开传感器
  delay(500);//延迟秒等待传感器开启
  int8_t temp = sensor.getTemp();//获取传感器温度存入变量temp中
  sensor.setExtCrystalUse(true);//使用外部晶振
}

void my_imu_update()
{

  imu::Quaternion quat = sensor.getQuat();            // 获取四元数数据

  // 获得传感器校准状态 判断是否校准
  uint8_t system, gyro, accel, mg = 0;
  if(is_cali == 1){
    system = 3;
  }else{
    sensor.getCalibration(&system, &gyro, &accel, &mg); // 显示校准数据
    if(system > 2){
      system = 3;
      is_cali = 1;
    }
  }

  // 获得传感器欧拉角数据
  imu::Vector<3> euler_v = quat.toEuler();   // 转换为欧拉角
  yaw = euler_v.y() * 180/M_PI;              // 偏航
  pitch = euler_v.x() * 180/M_PI;            // 俯仰
  roll = euler_v.z() * 180/M_PI;             // 翻滚
  //euler = sqrt(yaw * yaw + pitch * pitch + roll * roll);  // 计算欧拉角

  
  // 获得传感器四元数数据
  quat.normalize ();                        // 归一化
  float w,x,y,z;
  w=quat.w();    
  x=quat.x();
  y=quat.y();
  z=quat.z();

  // 获得传感器加速度数据
  imu::Vector<3> acc_v = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);     // 获取加速度
  imu::Vector<3> gyro_v = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);     // 获取陀螺仪角速度
  // if(WiFi_Read_Choose() == 0)
  // {
    accx = acc_v.x();  //获取x轴加速度
    accy = acc_v.y();  //获取y轴加速度
    accz = acc_v.z();  //获取z轴加速度
    gyrox = gyro_v.x();  //获取x轴角速度
    gyroy = gyro_v.y();  //获取y轴角速度
    gyroz = gyro_v.z();  //获取z轴角速度
    //Serial.println("选择不使用滤波"); //调试用
  // }
  // else if(WiFi_Read_Choose() == 1)
  // {
  //   accx = KMFilter(&ax, acc_v.x());  //获取x轴加速度
  //   accy = KMFilter(&ay, acc_v.y());  //获取y轴加速度
  //   accz = KMFilter(&az, acc_v.z());  //获取z轴加速度
  //   gyrox = KMFilter(&grx, gyro_v.x());  //获取x轴角速度
  //   gyroy = KMFilter(&gry, gyro_v.y());  //获取y轴角速度
  //   gyroz = KMFilter(&grz, gyro_v.z());  //获取z轴角速度
  //   //Serial.println("使用滤波"); //调试用
  // }
  // else
  // {
  //   Serial.println("my_sensor.cpp处执行错误：内容为选择是否关断自带滤波算法选择有问题，现在重启imu");
  //   esp_deep_sleep_start();                 // 进入低功耗模式（关机）
  // }
  // float vol = readPower();
  
  // if(SendMode == 0)
  // {
  //   UDP_Send_Data(name, port, system, vol, w, x, y, z);
  // }
  // else
  // {
  //   NOW_Send_Data(w, x, y, z);
  // }

  Serial.print("Gyroscope: x="); Serial.print(gyrox);
    Serial.print(" rad/s, y="); Serial.print(gyroy);
    Serial.print(" rad/s, z="); Serial.println(gyroz);
}
