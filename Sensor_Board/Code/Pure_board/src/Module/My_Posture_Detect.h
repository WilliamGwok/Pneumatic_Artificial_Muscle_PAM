// #include <iostream>
// #include <ArduinoEigen.h>
// #include <ArduinoEigenDense.h>
// #include <ArduinoEigenSparse.h>
// #include <cmath>
#include <Arduino.h>

typedef struct {
    float q_current[4];  // ��Ԫ��
    float theta; // ������x������
    uint16_t direction; // ������y������
} ArmPostureData_t;

extern ArmPostureData_t My_Arm_Posture_Data; // �ֱ���̬���ݽṹ��
extern  float send_test[3];
void My_Arm_Posture_Realtime_Detect(float q_0, float q_1, float q_2, float q_3);

