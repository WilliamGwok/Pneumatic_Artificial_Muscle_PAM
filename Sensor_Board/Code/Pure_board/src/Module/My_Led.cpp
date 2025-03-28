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
  // ���� LED2�������Ǻ�ɫ��
  ledcWrite(LED1_CHANNEL, 0);
  ledcWrite(LED2_CHANNEL, 20);
}

void My_LED_Green()
{
  // ���� LED1����������ɫ��
  ledcWrite(LED1_CHANNEL, 20);  // ���� 0~255
  ledcWrite(LED2_CHANNEL, 0);  // ���� 0~255
}

void My_LED_Yellow()
{
  // ���� LED1����������ɫ��
  ledcWrite(LED1_CHANNEL, 20);  // ���� 0~255
  ledcWrite(LED2_CHANNEL, 20);
}

void My_LED_Off()
{
  // �ر����� LED
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void My_LED_Control()
{
  My_LED_Green();
}

void My_LED_And_Posture_Control()
{
  float theta = My_Arm_Posture_Data.theta; // ��ȡ�ֱ�̧��Ƕ�

  // ���� LED �Ƶ���ɫ������
  if (theta > 100) 
  {
    My_LED_Red();
  } 
  else if (theta > 50) 
  {
    My_LED_Yellow();
  } else 
  {
    My_LED_Green();
  }
}

// void loop() {
    
//     delay(1000);

    
//     delay(1000);

    
//     delay(1000);
// }



