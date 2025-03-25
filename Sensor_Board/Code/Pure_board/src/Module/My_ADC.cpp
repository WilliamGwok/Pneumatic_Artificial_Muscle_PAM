#include "My_ADC.h"

#define ADC_Serial_Print 1


void My_ADC_Work()
{
  int adcValue = analogRead(ADC_PIN);  // 读取 ADC 值 (0~4095)

  float voltage = (adcValue * VOLTAGE_REF) / ADC_MAX * 2;  // 转换为电压

  #if ADC_Serial_Print
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("  Voltage: ");
  Serial.print(voltage, 3);  // 保留 3 位小数
  Serial.println(" V");
  #endif
}


