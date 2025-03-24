#include "Electric_detection.h"

/***************************************************************
  *  函数名称：readPower
  *  函数作用：读取 GPIO 引脚的模拟值，获取电压
  *  输入参数：无
  *  返 回 值：无
  *  函数的使用方法：在主函数中创建类名传入参数并直接调用函数，示例：My_Sensor my_sensor；//创建对象  my_sensor.readPower(); //函数调用 
  *  日    期： 2023.8.29
 **************************************************************/

 float readPower(){
    // 读取 GPIO 引脚的模拟值
    int analogValue = analogRead(voltagePin);
    float voltage = (analogValue*3.3/4095)*2 + 0.2; // 根据实际参考电压和模数转换位数进行调整
    float pown = (voltage - 2.8) / 1.4 * 100;// 输出电源百分比
    if(pown >= 100)
    {
      pown = 100;
    }
    /*
    Serial.print("Voltage: ");
    //Serial.print(analogValue);       
    Serial.print(voltage);
    Serial.print("pown: ");
    //Serial.print(analogValue);       
    Serial.print(pown);                                                                           
    Serial.println(" %");
    Serial.print("34: ");
    Serial.print(analogRead(34));
    */
    return pown;
 }

//低电量关机函数
 void PoweOff()
 {
   if(readPower() <= 0)
   {
     delay(50);
     if(readPower() <= 0)
     {
       int a = 0;//设置计数变量
       do
        {
          LED_PWMOpen();    //点亮LED
          delay(100);
          LED_PWMClose();             //关闭LED
          delay(100);
          a++;
        }while(a <= 5);
        esp_deep_sleep_start();//关机函数
     }
   }
 }