#include "My_Led.h"

void My_LED_Init()
{
  ledcSetup(LED1_CHANNEL, LED_FREQ, LED_RESOLUTION);

  ledcAttachPin(LED1, LED1_CHANNEL);

  ledcSetup(LED2_CHANNEL, LED_FREQ, LED_RESOLUTION);

  ledcAttachPin(LED2, LED2_CHANNEL);
}

void My_LED_Red()
{
  // 点亮 LED2（假设是红色）
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
}

void My_LED_Green()
{
  // 点亮 LED1（假设是绿色）
  ledcWrite(LED1_CHANNEL, 20);  // 亮度 0~255
  digitalWrite(LED2, LOW);
}

void My_LED_Off()
{
  // 关闭所有 LED
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void My_LED_Control()
{
  My_LED_Green();
}

// void loop() {
    
//     delay(1000);

    
//     delay(1000);

    
//     delay(1000);
// }



