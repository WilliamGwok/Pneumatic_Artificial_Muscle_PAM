#include <Arduino.h>

#define LED1 16  // 连接到 LED1 的引脚 绿灯
#define LED2 5  // 连接到 LED2 的引脚 红灯

#define LED1_CHANNEL 0
#define LED2_CHANNEL 1
#define LED_FREQ 5000  // 频率
#define LED_RESOLUTION 8  // 分辨率

void My_LED_Init();
void My_LED_Control();