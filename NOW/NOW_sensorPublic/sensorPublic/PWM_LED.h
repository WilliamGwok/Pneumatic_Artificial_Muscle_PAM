#ifndef _PWM_LED_
#define _PWM_LED_

#include <Arduino.h>
#include "driver/ledc.h"

#define LED 16                   // 状态指示灯（led双色灯珠）连接的io口为16 
void LED_PWMcontrol();           //呼吸灯控制
void LEDPWM_Init();              //LED灯pwm控制初始化
void LED_PWMOpen();              //点亮LED
void LED_PWMClose();             //关闭LED


#endif
