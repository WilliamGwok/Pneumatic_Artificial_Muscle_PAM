#include <stdio.h>
#include "src/IMU/My_Imu.h"
#include "src/Protocol/NOW_Protocol.h"
#include "src/Module/My_Led.h"
#include "src/Module/My_ADC.h"

unsigned long lastSendTime = 0;
const long interval = 20;

void My_Board_Init()
{
  My_Imu_Init();

  // NOW_Set_Up();

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

    // My_Imu_Update();

    // NOW_Send_Data();

    My_ADC_Work();
  }
  
  My_LED_Control();
}











