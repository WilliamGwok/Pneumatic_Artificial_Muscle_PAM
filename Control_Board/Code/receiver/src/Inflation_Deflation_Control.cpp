#include "Inflation_Deflation_Control.h"


#define PUMP_PIN 26
#define VALVE_PIN 25

int pump_state = 0; // 0: 关闭, 1: 打开
int pump_state_last = 0;// 0: 关闭, 1: 打开

int handPosition = 1;  
int previousPosition = 1;

// 充气计时
unsigned long pumpStartTime = 0;      // 充气开始时间
unsigned long totalPumpTime = 0;      // 累计充气时间
unsigned long pumpElapsedTime = 0;     // 当前充气时间
const unsigned long pumpDurationPerStep = 2000;  // 每次升档充气时间（2秒）
const unsigned long maxTotalPumpTime = 6000;     // 最大充气时间（6秒）

void My_Inflation_Deflation_Control_Init()
{
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(VALVE_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW); // 关闭泵
    digitalWrite(VALVE_PIN, LOW); // 关闭阀门
}

void My_Flation_Realtime_Control(int hand_pos)
{
    handPosition = hand_pos;

    if (handPosition > previousPosition) {
        // Serial.println("升档，开始充气...");
        
        // 如果还没达到总充气时间限制
        if (totalPumpTime < maxTotalPumpTime) {
          // 开启气泵
          digitalWrite(PUMP_PIN, HIGH);  
          digitalWrite(VALVE_PIN, HIGH);     // 关闭电磁阀
          
          // 记录每次升档的充气开始时间
          pumpStartTime = millis();

          pump_state = 1;
        }
      } 
      // **降档（放气）**
      else if (handPosition == 1) {
        // Serial.println("降档，开始放气...");
        
        digitalWrite(PUMP_PIN, LOW);   // 关闭气泵
        digitalWrite(VALVE_PIN, LOW);    // 打开电磁阀

        totalPumpTime = 0;    // 重置总充气时间
      } 
      // **保持不变（无动作）**
      else {
        // Serial.println("挡位未变，保持当前状态。");
      }
  
    // **充气时间监测**
    if (digitalRead(PUMP_PIN) == HIGH) {
      pumpElapsedTime = millis() - pumpStartTime;
  
      // 充气时间达到2秒，停止充气，计算总充气时间
      if (pumpElapsedTime >= pumpDurationPerStep) {

        pump_state = 0; // 关闭气泵
        digitalWrite(PUMP_PIN, LOW);  // 停止气泵
        
  
        // 检查是否已经超过最大充气时间
        if (totalPumpTime >= maxTotalPumpTime) {
        //   Serial.println("总充气时间已达到上限，停止充气。");
          totalPumpTime = maxTotalPumpTime;  // 保证总充气时间不会超过最大值
        }
      }
    }

    if(pump_state == 0 && pump_state_last == 1) {
        // Serial.println("气泵已打开，开始充气...");
        totalPumpTime += pumpElapsedTime;  // 累加充气时间
        pumpElapsedTime = 0;  // 重置当前充气时间
    }

    previousPosition = handPosition;  // 更新上一个挡位
    pump_state_last = pump_state;  // 更新上一个泵状态


    Serial.printf("state: %lu\n", totalPumpTime);
}

















