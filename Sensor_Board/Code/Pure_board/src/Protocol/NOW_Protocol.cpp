#include "NOW_Protocol.h"
#include "../IMU/My_Imu.h"
#include "../Module/My_Posture_Detect.h"

// 接收模块的 mac 地址 c8:2e:18:67:1a:40
uint8_t broadcastAddress[] = {0xc8, 0x2e, 0x18, 0x67, 0x1a, 0x40};
// 发送模块的 编号
int Board_ID = 1;
// 协议结构体
esp_now_peer_info_t peerInfo;
//发送数据结构体
typedef struct struct_message {
  int id; // must be unique for each sender board
  float gyrox;  float gyroy;  float gyroz;
  float q_0;    float q_1;    float q_2;    float q_3;
} struct_message;

struct_message My_NOW_Send_Message = 
{
  .id = Board_ID,
};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void NOW_Set_Up(void)
{
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void NOW_Send_Data()
{
  // 设置要发送的数据
  My_NOW_Send_Message.gyrox = My_Imu_Data.gyrox;
  My_NOW_Send_Message.gyroy = My_Imu_Data.gyroy;
  My_NOW_Send_Message.gyroz = My_Imu_Data.gyroz;

  My_NOW_Send_Message.q_0 = My_Imu_Data.w;
  My_NOW_Send_Message.q_1 = My_Imu_Data.x;
  My_NOW_Send_Message.q_2 = My_Imu_Data.y;
  My_NOW_Send_Message.q_3 = My_Imu_Data.z;

  // 发送数据
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &My_NOW_Send_Message, sizeof(My_NOW_Send_Message));

  // if (result == ESP_OK) {
  //   Serial.println("Sent with success");
  // } else {
  //   Serial.println("Error sending the data");
  // }
}











