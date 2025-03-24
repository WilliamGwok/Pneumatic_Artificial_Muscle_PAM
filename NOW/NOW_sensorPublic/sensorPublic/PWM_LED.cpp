#include "PWM_LED.h"

// 定义 LED 引脚和 PWM 参数

const int freq = 5000; // PWM 信号的频率
const int ledChannel = 0; // LEDC 通道号
const int resolution = 8; // PWM 分辨率,8 位
int dutyCycle = 0;
bool tag = true;
int max_brightness = 150;//LED最高亮度
//LED灯pwm控制初始化
void LEDPWM_Init()
{
  // 设置 LEDC 通道
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
}

//呼吸灯控制
void LED_PWMcontrol()
{
  if(true == tag)
  {
    dutyCycle++;
    ledcWrite(ledChannel, dutyCycle);
    delay(10);
  }
  else if(false == tag && dutyCycle > 0)
  {
    dutyCycle--;
    ledcWrite(ledChannel, dutyCycle);
    delay(10);
  }
  if(dutyCycle >= max_brightness)
  {
    tag = false;
  }
  else if(dutyCycle <= 0)
  {
    tag = true;
  }
  
}

//点亮LED
void LED_PWMOpen()
{
  ledcWrite(ledChannel, 60);
}

//关闭LED
void LED_PWMClose()
{
  ledcWrite(ledChannel, 0);
}



