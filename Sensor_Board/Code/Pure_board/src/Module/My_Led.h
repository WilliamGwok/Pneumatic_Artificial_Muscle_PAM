#include <Arduino.h>

#define LED1 16  // ���ӵ� LED1 ������ �̵�
#define LED2 5  // ���ӵ� LED2 ������ ���

#define LED1_CHANNEL 0
#define LED2_CHANNEL 1
#define LED_FREQ 5000  // Ƶ��
#define LED_RESOLUTION 8  // �ֱ���

void My_LED_Init();
void My_LED_Control();
void My_LED_And_Posture_Control();