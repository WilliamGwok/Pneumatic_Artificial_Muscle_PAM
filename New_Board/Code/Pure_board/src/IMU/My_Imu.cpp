#include "My_Imu.h"

static float roll = 0;
static float pitch = 0;
static float yaw = 0;
static float accx = 0;
static float accy = 0;
static float accz = 0;
static float gyrox = 0;
static float gyroz = 0;
static float gyroy = 0;   
static int is_cali = 0;

Adafruit_BNO055 sensor = Adafruit_BNO055();

void my_imu_init()
{
  sensor.begin();//�򿪴�����
  delay(500);//�ӳ���ȴ�����������
  int8_t temp = sensor.getTemp();//��ȡ�������¶ȴ������temp��
  sensor.setExtCrystalUse(true);//ʹ���ⲿ����
}

void my_imu_update()
{

  imu::Quaternion quat = sensor.getQuat();            // ��ȡ��Ԫ������

  // ��ô�����У׼״̬ �ж��Ƿ�У׼
  uint8_t system, gyro, accel, mg = 0;
  if(is_cali == 1){
    system = 3;
  }else{
    sensor.getCalibration(&system, &gyro, &accel, &mg); // ��ʾУ׼����
    if(system > 2){
      system = 3;
      is_cali = 1;
    }
  }

  // ��ô�����ŷ��������
  imu::Vector<3> euler_v = quat.toEuler();   // ת��Ϊŷ����
  yaw = euler_v.y() * 180/M_PI;              // ƫ��
  pitch = euler_v.x() * 180/M_PI;            // ����
  roll = euler_v.z() * 180/M_PI;             // ����
  //euler = sqrt(yaw * yaw + pitch * pitch + roll * roll);  // ����ŷ����

  
  // ��ô�������Ԫ������
  quat.normalize ();                        // ��һ��
  float w,x,y,z;
  w=quat.w();    
  x=quat.x();
  y=quat.y();
  z=quat.z();

  // ��ô��������ٶ�����
  imu::Vector<3> acc_v = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);     // ��ȡ���ٶ�
  imu::Vector<3> gyro_v = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);     // ��ȡ�����ǽ��ٶ�
  // if(WiFi_Read_Choose() == 0)
  // {
    accx = acc_v.x();  //��ȡx����ٶ�
    accy = acc_v.y();  //��ȡy����ٶ�
    accz = acc_v.z();  //��ȡz����ٶ�
    gyrox = gyro_v.x();  //��ȡx����ٶ�
    gyroy = gyro_v.y();  //��ȡy����ٶ�
    gyroz = gyro_v.z();  //��ȡz����ٶ�
    //Serial.println("ѡ��ʹ���˲�"); //������
  // }
  // else if(WiFi_Read_Choose() == 1)
  // {
  //   accx = KMFilter(&ax, acc_v.x());  //��ȡx����ٶ�
  //   accy = KMFilter(&ay, acc_v.y());  //��ȡy����ٶ�
  //   accz = KMFilter(&az, acc_v.z());  //��ȡz����ٶ�
  //   gyrox = KMFilter(&grx, gyro_v.x());  //��ȡx����ٶ�
  //   gyroy = KMFilter(&gry, gyro_v.y());  //��ȡy����ٶ�
  //   gyroz = KMFilter(&grz, gyro_v.z());  //��ȡz����ٶ�
  //   //Serial.println("ʹ���˲�"); //������
  // }
  // else
  // {
  //   Serial.println("my_sensor.cpp��ִ�д�������Ϊѡ���Ƿ�ض��Դ��˲��㷨ѡ�������⣬��������imu");
  //   esp_deep_sleep_start();                 // ����͹���ģʽ���ػ���
  // }
  // float vol = readPower();
  
  // if(SendMode == 0)
  // {
  //   UDP_Send_Data(name, port, system, vol, w, x, y, z);
  // }
  // else
  // {
  //   NOW_Send_Data(w, x, y, z);
  // }

  Serial.print("Gyroscope: x="); Serial.print(gyrox);
    Serial.print(" rad/s, y="); Serial.print(gyroy);
    Serial.print(" rad/s, z="); Serial.println(gyroz);
}
