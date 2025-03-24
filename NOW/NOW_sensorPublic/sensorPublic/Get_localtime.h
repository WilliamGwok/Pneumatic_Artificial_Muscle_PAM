#ifndef _GET_LOCALTIME_
#define _GET_LOCALTIME_
#include <WiFi.h>
#include "PWM_LED.h"
void Config_localtime(void); //获取网络时间，配置本地时间
void printLocalTime(void);// 打印当前时间
String Get_localtime(void);//获取本地时间



#endif