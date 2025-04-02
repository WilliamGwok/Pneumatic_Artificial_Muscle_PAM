#include <esp_now.h>
#include <WiFi.h>
#include "src/posture_detect.h"
#include "src/Inflation_Deflation_Control.h"

// Updated structure definition with floating point values
typedef struct struct_message {
  int id;        // Must be unique for each sender board
  // float gyrox;
  // float gyroy;
  // float gyroz;
  // float q_0;
  // float q_1;
  // float q_2;
  // float q_3;
  int arm_current_state;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  // Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.println(macStr);
  
  // Copy incoming data to myData structure
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  
  // Update the structures with the new incoming data
  // boardsStruct[myData.id - 1].gyrox = myData.gyrox;
  // boardsStruct[myData.id - 1].gyroy = myData.gyroy;
  // boardsStruct[myData.id - 1].gyroz = myData.gyroz;
  // boardsStruct[myData.id - 1].q_0 = myData.q_0;
  // boardsStruct[myData.id - 1].q_1 = myData.q_1;
  // boardsStruct[myData.id - 1].q_2 = myData.q_2;
  // boardsStruct[myData.id - 1].q_3 = myData.q_3;

  boardsStruct[myData.id - 1].arm_current_state = myData.arm_current_state;

  // My_Arm_Posture_Realtime_Detect(myData.q_0, myData.q_1, myData.q_2, myData.q_3);

  // Print the received values for debugging
  // Serial.printf("accx: %.2f, accy: %.2f, accz: %.2f\n", boardsStruct[myData.id - 1].accx, boardsStruct[myData.id - 1].accy, boardsStruct[myData.id - 1].accz);
  // Serial.printf("%.2f, %.2f, %.2f\n", boardsStruct[myData.id - 1].gyrox, boardsStruct[myData.id - 1].gyroy, boardsStruct[myData.id - 1].gyroz);
  // Serial.printf("q_0: %.2f, q_1: %.2f, q_2: %.2f, q_3: %.2f\n", boardsStruct[myData.id - 1].q_0, boardsStruct[myData.id - 1].q_1, boardsStruct[myData.id - 1].q_2, boardsStruct[myData.id - 1].q_3);
  // Serial.println();

  // Serial.printf("state: %d\n", boardsStruct[myData.id - 1].arm_current_state);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  My_Inflation_Deflation_Control_Init();
  
  // Once ESPNow is successfully initialized, register for the receive callback function
  esp_now_register_recv_cb(OnDataRecv);

  // pinMode(25, OUTPUT); // 设置GPIO0为输出模�?
  // digitalWrite(25, HIGH); // 设置GPIO0输出高电�?

  // pinMode(26, OUTPUT); // 设置GPIO0为输出模�?
  // digitalWrite(26, HIGH); // 设置GPIO0输出高电�?
}

void loop() {
  // Delay to allow for new data to be received
  // delay(20);

  My_Flation_Realtime_Control(boardsStruct[0].arm_current_state);
}


// #include <esp_now.h>
// #include <WiFi.h>

// // Structure example to receive data
// // Must match the sender structure
// typedef struct struct_message {
//   int id;
//   int x;
//   int y;
// }struct_message;

// // Create a struct_message called myData
// struct_message myData;

// // Create a structure to hold the readings from each board
// struct_message board1;e:\Files\大四下\毕业设�??\Github\NOW\receiver\src\posture_detect.cpp
// struct_message board2;
// struct_message board3;

// // Create an array with all the structures
// struct_message boardsStruct[3] = {board1, board2, board3};

// // callback function that will be executed when data is received
// void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
//   char macStr[18];
//   Serial.print("Packet received from: ");
//   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//   Serial.println(macStr);
//   memcpy(&myData, incomingData, sizeof(myData));
//   Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
//   // Update the structures with the new incoming data
//   boardsStruct[myData.id-1].x = myData.x;
//   boardsStruct[myData.id-1].y = myData.y;
//   Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
//   Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
//   Serial.println();
// }
 
// void setup() {
//   //Initialize Serial Monitor
//   Serial.begin(115200);
  
//   //Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   //Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
  
//   // Once ESPNow is successfully Init, we will register for recv CB to
//   // get recv packer info
//   esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
// }

// void loop() {
//   // Acess the variables for each board
//   /*int board1X = boardsStruct[0].x;
//   int board1Y = boardsStruct[0].y;
//   int board2X = boardsStruct[1].x;
//   int board2Y = boardsStruct[1].y;
//   int board3X = boardsStruct[2].x;
//   int board3Y = boardsStruct[2].y;*/

//   delay(20);  
// }
