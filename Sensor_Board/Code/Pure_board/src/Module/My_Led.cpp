#include "My_Led.h"
#include "My_Posture_Detect.h"



void My_LED_Init()
{
  ledcSetup(LED1_CHANNEL, LED_FREQ, LED_RESOLUTION);

  ledcAttachPin(LED1, LED1_CHANNEL);

  ledcSetup(LED2_CHANNEL, LED_FREQ, LED_RESOLUTION);

  ledcAttachPin(LED2, LED2_CHANNEL);
}

void My_LED_Red()
{
  ledcWrite(LED1_CHANNEL, 0);
  ledcWrite(LED2_CHANNEL, 20);
}

void My_LED_Green()
{
  ledcWrite(LED1_CHANNEL, 20);
  ledcWrite(LED2_CHANNEL, 0);  
}

void My_LED_Yellow()
{

  ledcWrite(LED1_CHANNEL, 20);
  ledcWrite(LED2_CHANNEL, 20);
}

void My_LED_Off()
{
  ledcWrite(LED1_CHANNEL, 0);
  ledcWrite(LED2_CHANNEL, 0);
}

void My_LED_Control()
{
  My_LED_Green();
}

void My_LED_And_Posture_Control()
{
  float theta = My_Arm_Posture_Data.theta;


  if (theta > 120) 
  {
    My_LED_Red();
  }
  else if (theta > 90) 
  {
    My_LED_Yellow();
  }
  else if (theta > 45) 
  {
    My_LED_Green();
  }
  else 
  {
    My_LED_Off();
  }
}





