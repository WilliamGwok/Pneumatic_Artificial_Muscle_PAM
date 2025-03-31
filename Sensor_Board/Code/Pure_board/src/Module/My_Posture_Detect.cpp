#include "My_Posture_Detect.h"

// 定义四元数数据
const int dataSize = 100;  // 数据数量
float q_ref[4] = {0.6857, 0.0365, -0.7267, 0};  // 参考四元数(手臂自然下垂时的四元数)

float send_test[3];

ArmPostureData_t My_Arm_Posture_Data; // 手臂姿态数据结构体

// // 计算q_ref（初始化参考四元数）
// void calculateQRef() 
// {
//     float w_ = 0, x_ = 0, y_ = 0, z_ = 0;
//     for (int i = 0; i < dataSize; i++) {
//         w_ += w[i];
//         x_ += x[i];
//         y_ += y[i];
//         z_ += z[i];
//     }
//     q_ref[0] = w_ / dataSize;
//     q_ref[1] = x_ / dataSize;
//     q_ref[2] = y_ / dataSize;
//     q_ref[3] = z_ / dataSize;
// }


//动作分4档（1手垂下，2手微微抬起，3手水平，4手高过头顶），充气（手越抬越高，气泵控制）分3次，充气在挡位变更时触发，放气（手越放越低，由电磁阀控制）同理
//现在我有两个引脚控制气泵和电磁阀的通断，气泵和电磁阀不能同时通电，请为我写出c++控制代码

/**
  * @brief  计算手臂抬起角度
  * @param  float q_ref[], float q_current[], float &dot_product, float &theta
  * @retval None
  */
float judgeArmPosture(float q_ref[], float q_current[]) 
{
    // 计算四元数点积
    float dot_product = q_ref[0] * q_current[0] + q_ref[1] * q_current[1] + q_ref[2] * q_current[2] + q_ref[3] * q_current[3];

    // 计算旋转角度（度）
    float theta = abs(2 * acos(dot_product) * 180.0 / PI);

    return theta;
}

/**
  * @brief  计算手臂抬起方向
  * @param  float q_ref[], float q_current[]
  * @retval None
  */
void judgeArmDirection(float q_ref[], float q_current[]) 
{
    float theta;
    theta = judgeArmPosture(q_ref, q_current);

    // Serial.print("旋转角度: ");
    // Serial.print(theta);
    // Serial.println(" 度");

    // 计算旋转轴
    float cross_product[3] = {
        q_ref[1] * q_current[2] - q_ref[2] * q_current[1],
        q_ref[2] * q_current[3] - q_ref[3] * q_current[2],
        q_ref[3] * q_current[1] - q_ref[1] * q_current[3]
    };
    
    float norm = sqrt(cross_product[0] * cross_product[0] +
                      cross_product[1] * cross_product[1] +
                      cross_product[2] * cross_product[2]);
    
    float rotation_axis[3] = {cross_product[0] / norm, cross_product[1] / norm, cross_product[2] / norm};

    send_test[0] = rotation_axis[0];
    send_test[1] = rotation_axis[1];
    send_test[2] = rotation_axis[2];
    
    // Serial.print("旋转轴: [");
    // Serial.print(rotation_axis[0]);
    // Serial.print(", ");
    // Serial.print(rotation_axis[1]);
    // Serial.print(", ");
    // Serial.print(rotation_axis[2]);
    // Serial.println("]");

    // 方向判断
    // if (-rotation_axis[2] > 0.7) {
    //     Serial.println("手臂向前举");
    // } else if (rotation_axis[0] > 0.7) {
    //     Serial.println("手臂向侧面举");
    // } else {
    //     Serial.println("手臂可能处于其他方向");
    // }
}

/**
  * @brief  根据手臂姿态控制充放气
  * @param  float theta, float gyrox, float gyroy, int &control_, int &control_pro
  * @retval None
  */
void controlAirPressure(float theta, float gyrox, float gyroy, int &control_, int &control_pro) 
{
    const float theta_hold = 40;
    const float theta_hold_pro_1 = 15;
    const float theta_hold_pro_2 = 105;
    const float velocity_hold = 100;
    const int on = 1;
    const int off = 0;

    if (theta > theta_hold) {
        control_ = on;
        control_pro = on;
    } else if (theta > theta_hold_pro_1 && (gyrox < -velocity_hold || gyroy < -velocity_hold)) {
        control_pro = on;
    } else {
        control_ = off;
        control_pro = off;
    }

    if (theta > theta_hold && theta < theta_hold_pro_2 && (gyrox > velocity_hold || gyroy > velocity_hold)) 
    {
        control_pro = off;
    }
}

/**
  * @brief  手臂姿态实时检测
  * @param  float q_0, float q_1, float q_2, float q_3
  * @retval None
  */
void My_Arm_Posture_Realtime_Detect(float q_0, float q_1, float q_2, float q_3)
{
    My_Arm_Posture_Data.q_current[0] = q_0;
    My_Arm_Posture_Data.q_current[1] = q_1;
    My_Arm_Posture_Data.q_current[2] = q_2;
    My_Arm_Posture_Data.q_current[3] = q_3;

    My_Arm_Posture_Data.theta = judgeArmPosture(q_ref, My_Arm_Posture_Data.q_current);

    Serial.printf("theta: %.2f\n", My_Arm_Posture_Data.theta);

    judgeArmDirection(q_ref, My_Arm_Posture_Data.q_current);
}


