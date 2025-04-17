#include "My_ADC.h"

#define ADC_Serial_Print 1


void My_ADC_Work()
{
  int adcValue = analogRead(ADC_PIN);  // ��ȡ ADC ֵ (0~4095)

  float voltage = (adcValue * VOLTAGE_REF) / ADC_MAX * 2;  // ת��Ϊ��ѹ

  #if ADC_Serial_Print
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("  Voltage: ");
  Serial.print(voltage, 3);  // ���� 3 λС��
  Serial.println(" V");
  #endif
}


