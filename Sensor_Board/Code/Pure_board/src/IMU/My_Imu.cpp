#include "My_Imu.h"

Adafruit_BNO055 sensor = Adafruit_BNO055();

#define Imu_Print_Data 0

void My_Imu_Posture_Data_Update();
void My_Imu_Calibration();
void My_Imu_Data_Serial_Print();

Imu_Data_t My_Imu_Data;

void My_Imu_Init()
{
  sensor.begin();

  delay(500);

  sensor.setExtCrystalUse(true);//�?用�?�部晶振
}

void My_Imu_Update()
{
  My_Imu_Posture_Data_Update();

  My_Imu_Calibration();

  #if Imu_Print_Data
    My_Imu_Data_Serial_Print();
  #endif
}

void My_Imu_Posture_Data_Update()
{
  //四元�?
  imu::Quaternion quat = sensor.getQuat();

  quat.normalize ();

  My_Imu_Data.w = quat.w();
  My_Imu_Data.x = quat.x();
  My_Imu_Data.y = quat.y();
  My_Imu_Data.z = quat.z();

  //欧拉�?
  //euler = sqrt(yaw * yaw + pitch * pitch + roll * roll);
  imu::Vector<3> euler_v = quat.toEuler();   
  My_Imu_Data.yaw = euler_v.y() * 180/M_PI;         
  My_Imu_Data.pitch = euler_v.x() * 180/M_PI;        
  My_Imu_Data.roll = euler_v.z() * 180/M_PI; 

  //角速度
  imu::Vector<3> gyro_v = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  My_Imu_Data.gyrox = gyro_v.x();
  My_Imu_Data.gyroy = gyro_v.y();
  My_Imu_Data.gyroz = gyro_v.z();

  //角加速度
  imu::Vector<3> acc_v = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  My_Imu_Data.accx = acc_v.x();
  My_Imu_Data.accy = acc_v.y();
  My_Imu_Data.accz = acc_v.z();
}

void My_Imu_Calibration()
{
  uint8_t system, gyro, accel, mg = 0;

  if(My_Imu_Data.is_cali == 1)
  {
    system = 3;
  }else
  {
    sensor.getCalibration(&system, &gyro, &accel, &mg);

    if(system > 2)
    {
      system = 3;
      My_Imu_Data.is_cali = 1;
    }
  }
}

void My_Imu_Data_Serial_Print()
{
  Serial.print(" gyrox: ");
  Serial.print(My_Imu_Data.gyrox, 4);

  Serial.print(" gyroy: ");
  Serial.print(My_Imu_Data.gyroy, 4);

  Serial.print(" gyroz: ");
  Serial.print(My_Imu_Data.gyroz, 4);

  Serial.print(" is_cali: ");
  Serial.print(My_Imu_Data.is_cali);

  Serial.println();
}


// /*.......................Mahony.......................*/
// // ��������
// #define DEG2RAD 0.01745329251
// #define RAD2DEG 57.2957795131

// // ������ṹ��
// struct Axis3f {
//     float x;
//     float y;
//     float z;
// };

// // Mahony �˲����ṹ��
// struct MAHONY_FILTER_t {
//     // �������
//     float Kp, Ki;          // �����ͻ�������
//     float dt;              // ����ʱ����
//     Axis3f gyro, acc;      // �����Ǻͼ��ٶȼ�����

//     // ���̲���
//     float exInt, eyInt, ezInt; // ��������ۼ�
//     float q0, q1, q2, q3;      // ��Ԫ��
//     float rMat[3][3];          // ��ת����

//     // �������
//     float pitch, roll, yaw;    // ��̬�ǣ������ǣ���ת�ǣ�ƫ����

//     // ����ָ��
//     void (*mahony_init)(struct MAHONY_FILTER_t* mahony_filter, float Kp, float Ki, float dt);
//     void (*mahony_input)(struct MAHONY_FILTER_t* mahony_filter, Axis3f gyro, Axis3f acc);
//     void (*mahony_update)(struct MAHONY_FILTER_t* mahony_filter);
//     void (*mahony_output)(struct MAHONY_FILTER_t* mahony_filter);
//     void (*RotationMatrix_update)(struct MAHONY_FILTER_t* mahony_filter);
// };

// // ����ƽ��������
// float invSqrt(float x) {
//   return 1.0f / sqrt(x);
// }

// // ���������Ǻͼ��ٶȼ�����
// void mahony_input(struct MAHONY_FILTER_t* mahony_filter, Axis3f gyro, Axis3f acc) {
//     mahony_filter->gyro = gyro;
//     mahony_filter->acc = acc;
// }

// // �����˲���
// void mahony_update(struct MAHONY_FILTER_t* mahony_filter) {
//     float normalise;
//     float ex, ey, ez;

//     // ���ٶ�תΪ����
//     mahony_filter->gyro.x *= DEG2RAD;
//     mahony_filter->gyro.y *= DEG2RAD;
//     mahony_filter->gyro.z *= DEG2RAD;

//     // ��λ�����ٶȼ�����
//     normalise = invSqrt(mahony_filter->acc.x * mahony_filter->acc.x + 
//                         mahony_filter->acc.y * mahony_filter->acc.y + 
//                         mahony_filter->acc.z * mahony_filter->acc.z);
//     mahony_filter->acc.x *= normalise;
//     mahony_filter->acc.y *= normalise;
//     mahony_filter->acc.z *= normalise;

//     // ������
//     ex = (mahony_filter->acc.y * mahony_filter->rMat[2][2] - mahony_filter->acc.z * mahony_filter->rMat[2][1]);
//     ey = (mahony_filter->acc.z * mahony_filter->rMat[2][0] - mahony_filter->acc.x * mahony_filter->rMat[2][2]);
//     ez = (mahony_filter->acc.x * mahony_filter->rMat[2][1] - mahony_filter->acc.y * mahony_filter->rMat[2][0]);

//     // �������
//     mahony_filter->exInt += mahony_filter->Ki * ex * mahony_filter->dt;
//     mahony_filter->eyInt += mahony_filter->Ki * ey * mahony_filter->dt;
//     mahony_filter->ezInt += mahony_filter->Ki * ez * mahony_filter->dt;

//     // PI ����������ƫ
//     mahony_filter->gyro.x += mahony_filter->Kp * ex + mahony_filter->exInt;
//     mahony_filter->gyro.y += mahony_filter->Kp * ey + mahony_filter->eyInt;
//     mahony_filter->gyro.z += mahony_filter->Kp * ez + mahony_filter->ezInt;

//     // ��Ԫ���˶�ѧ���̻���
//     float q0Last = mahony_filter->q0;
//     float q1Last = mahony_filter->q1;
//     float q2Last = mahony_filter->q2;
//     float q3Last = mahony_filter->q3;
//     float halfT = mahony_filter->dt * 0.5f;
    
//     mahony_filter->q0 += (-q1Last * mahony_filter->gyro.x - q2Last * mahony_filter->gyro.y - q3Last * mahony_filter->gyro.z) * halfT;
//     mahony_filter->q1 += (q0Last * mahony_filter->gyro.x + q2Last * mahony_filter->gyro.z - q3Last * mahony_filter->gyro.y) * halfT;
//     mahony_filter->q2 += (q0Last * mahony_filter->gyro.y - q1Last * mahony_filter->gyro.z + q3Last * mahony_filter->gyro.x) * halfT;
//     mahony_filter->q3 += (q0Last * mahony_filter->gyro.z + q1Last * mahony_filter->gyro.y - q2Last * mahony_filter->gyro.x) * halfT;

//     // ��λ����Ԫ��
//     normalise = invSqrt(mahony_filter->q0 * mahony_filter->q0 + 
//                         mahony_filter->q1 * mahony_filter->q1 + 
//                         mahony_filter->q2 * mahony_filter->q2 + 
//                         mahony_filter->q3 * mahony_filter->q3);
//     mahony_filter->q0 *= normalise;
//     mahony_filter->q1 *= normalise;
//     mahony_filter->q2 *= normalise;
//     mahony_filter->q3 *= normalise;

//     // ������ת����
//     mahony_filter->RotationMatrix_update(mahony_filter);
// }

// // ����ת��������ȡ��̬��
// void mahony_output(struct MAHONY_FILTER_t* mahony_filter) {
//     // ���㸩���ǡ���ת�ǡ�ƫ����
//     // ��ӡ��̬��
//   Serial.print("Pitch: ");
//   Serial.print(mahony_filter->rMat[2][0]);
//   // Serial.print("\tRoll: ");
//   // Serial.print(mahony_filter.roll);
//   // Serial.print("\tYaw: ");
//   // Serial.println(mahony_filter.yaw);

//     mahony_filter->pitch = -asin(mahony_filter->rMat[2][0]) * RAD2DEG;
//     mahony_filter->roll = atan2f(mahony_filter->rMat[2][1], mahony_filter->rMat[2][2]) * RAD2DEG;
//     mahony_filter->yaw = atan2f(mahony_filter->rMat[1][0], mahony_filter->rMat[0][0]) * RAD2DEG;
// }



// // ������ת����
// void RotationMatrix_update(struct MAHONY_FILTER_t* mahony_filter) {
//     float q1q1 = mahony_filter->q1 * mahony_filter->q1;
//     float q2q2 = mahony_filter->q2 * mahony_filter->q2;
//     float q3q3 = mahony_filter->q3 * mahony_filter->q3;

//     float q0q1 = mahony_filter->q0 * mahony_filter->q1;
//     float q0q2 = mahony_filter->q0 * mahony_filter->q2;
//     float q0q3 = mahony_filter->q0 * mahony_filter->q3;
//     float q1q2 = mahony_filter->q1 * mahony_filter->q2;
//     float q1q3 = mahony_filter->q1 * mahony_filter->q3;
//     float q2q3 = mahony_filter->q2 * mahony_filter->q3;

//     mahony_filter->rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
//     mahony_filter->rMat[0][1] = 2.0f * (q1q2 + -q0q3);
//     mahony_filter->rMat[0][2] = 2.0f * (q1q3 - -q0q2);

//     mahony_filter->rMat[1][0] = 2.0f * (q1q2 - -q0q3);
//     mahony_filter->rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
//     mahony_filter->rMat[1][2] = 2.0f * (q2q3 + -q0q1);

//     mahony_filter->rMat[2][0] = 2.0f * (q1q3 + -q0q2);
//     mahony_filter->rMat[2][1] = 2.0f * (q2q3 - -q0q1);
//     mahony_filter->rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
// }



// // ��ʼ�� Mahony �˲���
// void mahony_init(struct MAHONY_FILTER_t* mahony_filter, float Kp, float Ki, float dt) {
//   mahony_filter->Kp = Kp;
//   mahony_filter->Ki = Ki;
//   mahony_filter->dt = dt;
//   mahony_filter->q0 = 1.0f;
//   mahony_filter->mahony_input = mahony_input;
//   mahony_filter->mahony_update = mahony_update;
//   mahony_filter->mahony_output = mahony_output;
//   mahony_filter->RotationMatrix_update = RotationMatrix_update;
// }

// struct MAHONY_FILTER_t mahony_filter;

// void My_Imu_Mahony_Init()
// {
//   // ��ʼ���˲���
//   mahony_init(&mahony_filter, 2.0f, 0.1f, 0.01f);
// }

// void My_Imu_Mahony_Update()
// {
//   // �����˲���
//   Axis3f gyro = {My_Imu_Data.gyrox, My_Imu_Data.gyroy, My_Imu_Data.gyroz};
//   Axis3f acc = {My_Imu_Data.accx, My_Imu_Data.accy, My_Imu_Data.accz};
//   mahony_input(&mahony_filter, gyro, acc);
//   mahony_update(&mahony_filter);
//   mahony_output(&mahony_filter);

//   // // ��ӡ��̬��
//   // Serial.print("Pitch: ");
//   // Serial.print(mahony_filter.pitch);
//   // Serial.print("\tRoll: ");
//   // Serial.print(mahony_filter.roll);
//   // Serial.print("\tYaw: ");
//   // Serial.println(mahony_filter.yaw);
// }