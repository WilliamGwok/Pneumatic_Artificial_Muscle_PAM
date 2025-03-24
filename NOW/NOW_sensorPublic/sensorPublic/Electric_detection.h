#ifndef _ELECTRIC_DETECTION_H
#define _ELECTRIC_DETECTION_H
#include "MY_Sensor.h"
#define voltagePin 32  // 测电引脚号 
float readPower(void);//读取 GPIO 引脚的模拟值，获取电压
void PoweOff(void);//低电量关机函数
#endif