#include "Inflation_Deflation_Control.h"


#define PUMP_PIN 26
#define VALVE_PIN 27

int handPosition = 1;  
int previousPosition = 1;

// ������ʱ
unsigned long pumpStartTime = 0;      // ������ʼʱ��
unsigned long totalPumpTime = 0;      // �ۼƳ���ʱ��
const unsigned long pumpDurationPerStep = 2000;  // ÿ����������ʱ�䣨2�룩
const unsigned long maxTotalPumpTime = 6000;     // ������ʱ�䣨6�룩

void My_Inflation_Deflation_Control_Init()
{
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(VALVE_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW); // �رձ�
    digitalWrite(VALVE_PIN, LOW); // �رշ���
}

void My_Flation_Realtime_Control(int hand_pos)
{
    handPosition = hand_pos;

    if (handPosition > previousPosition) {
        Serial.println("��������ʼ����...");
        
        // �����û�ﵽ�ܳ���ʱ������
        if (totalPumpTime < maxTotalPumpTime) {
          // ��������
          digitalWrite(PUMP_PIN, HIGH);  
          digitalWrite(VALVE_PIN, LOW);     // �رյ�ŷ�
          
          // ��¼ÿ�������ĳ�����ʼʱ��
          pumpStartTime = millis();
        }
      } 
      // **������������**
      else if (handPosition == 1) {
        Serial.println("��������ʼ����...");
        
        digitalWrite(PUMP_PIN, LOW);   // �ر�����
        digitalWrite(VALVE_PIN, HIGH);    // �򿪵�ŷ�

        totalPumpTime = 0;    // �����ܳ���ʱ��
      } 
      // **���ֲ��䣨�޶�����**
      else {
        Serial.println("��λδ�䣬���ֵ�ǰ״̬��");
      }
  
    // **����ʱ����**
    if (digitalRead(PUMP_PIN) == HIGH) {
      unsigned long pumpElapsedTime = millis() - pumpStartTime;
  
      // ����ʱ��ﵽ2�룬ֹͣ�����������ܳ���ʱ��
      if (pumpElapsedTime >= pumpDurationPerStep) {
        digitalWrite(PUMP_PIN, LOW);  // ֹͣ����
        totalPumpTime += pumpDurationPerStep;  // �ۼӳ���ʱ��
  
        // ����Ƿ��Ѿ�����������ʱ��
        if (totalPumpTime >= maxTotalPumpTime) {
          Serial.println("�ܳ���ʱ���Ѵﵽ���ޣ�ֹͣ������");
          totalPumpTime = maxTotalPumpTime;  // ��֤�ܳ���ʱ�䲻�ᳬ�����ֵ
        }
      }
    }

    previousPosition = handPosition;  // ������һ����λ
}

















