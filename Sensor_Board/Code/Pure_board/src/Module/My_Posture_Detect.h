// #include <iostream>
// #include <ArduinoEigen.h>
// #include <ArduinoEigenDense.h>
// #include <ArduinoEigenSparse.h>
// #include <cmath>
#include <Arduino.h>

typedef struct {
    float q_current[4];  // 四元数
    float theta; // 陀螺仪x轴数据
    uint16_t direction; // 陀螺仪y轴数据
} ArmPostureData_t;

extern ArmPostureData_t My_Arm_Posture_Data; // 手臂姿态数据结构体
extern  float send_test[3];
void My_Arm_Posture_Realtime_Detect(float q_0, float q_1, float q_2, float q_3);

