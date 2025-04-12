#include <stdio.h>
#include "src/IMU/My_Imu.h"
#include "src/Protocol/NOW_Protocol.h"
#include "src/Module/My_Led.h"
#include "src/Module/My_ADC.h"
#include "src/Module/My_Posture_Detect.h"

unsigned long lastSendTime = 0;
const long interval = 10;

void My_Board_Init()
{
  My_Imu_Init();

  NOW_Set_Up();

  My_LED_Init();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  My_Board_Init();
}

void loop() {
  //put your main code here, to run repeatedly:
  if (millis() - lastSendTime >= interval) 
  {
    lastSendTime = millis();

    My_Imu_Update();

    My_Arm_Posture_Realtime_Detect(
      My_Imu_Data.w,
      My_Imu_Data.x,
      My_Imu_Data.y,
      My_Imu_Data.z
   
    );
  // My_ADC_Work();
    NOW_Send_Data();

   
  }
  
  My_LED_And_Posture_Control();

  delay(50);
}











