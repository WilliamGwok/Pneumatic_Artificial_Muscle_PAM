#ifndef __MY_SENSOR_H__
#define __MY_SENSOR_H__
 
#include <Wire.h>
#include <stdio.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>
#include <Math.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ArduinoJson.h"
#include "AsyncUDP.h"
#include "My_button2.h"
#include "Electric_detection.h"
#include "Ip_adress.h"
#include "Kalman_filter.h"
#include "PWM_LED.h"
#include "My_time.h"
//下面部分为传感器使用类，可调用的函数内容如下，具体函数内容请前往sendData.cpp


// 外部调用函数
void initSensor();  // 传感器初始化
void getData(String name, int port);  // 读取传感器数据 处理传感器数据 发送传感器数据

//私有函数声明
String GetInString(String sensorName, int temp, int system);  // 获得传感器温度 校准状态数据 用于测试
String GetJSONString(String gettime, String sensorName, float x, float y , float z, float w, float accx, float accy, float accz, float gyrox, float gyroy, float gyroz, int system, float tag, float power, float SendRate) ;  // 将读取到的运动数据按照json格式存放，格式为{"n":"name","o":{"x":"0.0301","y":"0.3863","z":"-0.7606","w":"-0.5209","euler":"23.23","acc":"9.80"p":"11"}}
void UDPSendData(String message, String ip, int port);  // 通过udp协议发送数据，参数： string message: 需要发送的数据   string ip: ip  string port: 端口
void UDPConnect();  // UDP连接
void NOW_Set_Up(void);
void NOW_Send_Data(float w, float x, float y, float z);
void UDP_Send_Data(String name, int port, uint8_t system, float vol, float w, float x, float y, float z);


#endif