#include <stdio.h>
#include "src/IMU/My_Imu.h"
unsigned long lastSendTime = 0;
const long interval = 20; // 10秒

void my_board_Init()
{
  my_imu_init();  // 初始化传感器
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // 初始化串口通信 设置波特率为115200  

  
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (millis() - lastSendTime >= interval) 
  // {
  //   lastSendTime = millis();

    my_imu_update();  // 发送数据
  // }
  
}











