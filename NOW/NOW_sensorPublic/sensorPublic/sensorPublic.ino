#include "WiFiUser.h"
#include "My_Sensor.h"
#include "My_time.h"
#include "My_button2.h"
#include "Electric_detection.h"
#include "Kalman_filter.h"

#define SendMode 1 //UDP 0 NOW 1
unsigned long lastSendTime = 0;
const long interval = 20; // 10秒

int port = 8080;                                  // 设置默认的udp传输发送端口
extern bool WiFiU_conflag;
const char *NVS_Name = "myNVS";//创建一个nvs键空间名字为myNVS
extern String MySendName; 
extern int MySendRate;
bool Time_set_flag;


void setup() {
    Serial.begin(115200);  // 初始化串口通信 设置波特率为115200  
    initSensor();  // 初始化传感器
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);  // 设置唤醒设置关机->开机 （从低功耗进入正常工作模式）；唤醒引脚为io33 ；当1 = High或 0 = Low 唤醒
    Init_timeset();  // 初始化定时器设置
    pinMode(LED, OUTPUT);  // ledio设置为输出

    /************************************
      Init_KeyDetectionSet()函数注释：
      1.长按，进入低功耗模式（待机）
      2.短按，打印11，状态标志位power = 1 
      3.双击，清空网络信息，重新配网
    ************************************/
    Init_KeyDetectionSet();  // 按键检测初始化设置
    LEDPWM_Init();              //LED灯pwm控制初始化
    digitalWrite(LED, HIGH);    // 开机 开机指示灯亮 
    power = 1;  // 状态位置1
    inter = 0;
    Time_set_flag = true;
    // 用于电池电量读取 
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);
    pinMode(34, INPUT);
    PoweOff();//开机电量检测
    NVSINIT(NVS_Name);//初始化创建一个nvs键空间的名称
    
    Struct_arr_Init();//卡尔曼滤波数值初始化      

    if(SendMode == 1)
    {
      NOW_Set_Up();
    }
}



void loop() {
  // 启动按钮状态检测
  button.loop();
  // 处于开机状态
  if (power == 1)
  {
    if(SendMode == 1)
    {
      if (millis() - lastSendTime >= interval) 
      {
        lastSendTime = millis();

        getData("na", 0);  // 发送数据
      }
    }
    else
    {
      checkDNS_HTTP();                    // 检测客户端DNS&HTTP请求，也就是检查配网页面那部分
      checkConnect(true);                 // 检测网络连接状态，参数true表示如果断开重新连接
      if(WiFiU_conflag == true)//判断WiFi是否已经连接
      {
        port = getSensorPort();             // 获得配网时输入的端口
        if(port == 0) 
        {
          port = 8080;          // 若没输入 则默认为8080
        }
        if(flag_Rate == true && Time_set_flag == true)
        {
          Rerate();//设置定时器发送频率
          Time_set_flag = false;
        }
        //Serial.println("test");
        if(flag == true)
        {
          getData(MySendName, port);  // 发送数据
          flag = false;//复原标志位
        }
      }
    }
  }
  PoweOff();//电量检测,低电量关机
}





























