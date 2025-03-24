#include "WiFiUser.h"


//修改这部分名称
const char* AP_SSID = WIFI_NAME;        // 设置AP热点名称
const char* HOST_NAME = WIFI_NAME;        // 设置设备名



const byte DNS_PORT = 53;                  // 设置DNS端口号
const int webPort = 80;                    // 设置Web端口号

//const char* AP_PASS  = "";               // 这里不设置设置AP热点密码

String scanNetworksID = "";                // 用于储存扫描到的WiFi ID
bool WiFiU_conflag = false;                 //判断WiFi是否连接标志位
IPAddress apIP(192, 168, 6, 1);            // 设置AP的IP地址
 
String wifi_ssid = "";                     // 暂时存储wifi账号密码
String wifi_pass = "";                     // 暂时存储wifi账号密码
String MySendName = "ESP32_test";          // 初始化发送数据的名称
String Wifi_ip = "192.168.1.103";
int Rate = 100;
static String Choose = "OFF";               // 选择是否采用自带算法，OFF默认不带
bool Config_localtime_flag = true;         //本地时间第一次配置标志位
extern const char *NVS_Name;

RTC_DATA_ATTR int sensor_port = 8080;                   // 暂时存储设备名字 存入RTC_DATA_ATTR 关机重启也不会改变
 
int connectTimeOut_s = 10;                 // WiFi连接超时时间，单位秒
DNSServer dnsServer;                       // 创建dnsServer实例
WebServer server(webPort);                 // 开启web服务, 创建TCP SERVER,参数: 端口号,最大连接数
 
const char ROOT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>WIFI Configuration</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style type="text/css">
body {
    background-color: #f2f2f2;
    font-family: Arial, sans-serif;
}
.container {
    max-width: 500px;
    margin: 0 auto;
    padding: 20px;
    background-color: #ffffff;
    border-radius: 5px;
    box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
}
.logo {
    text-align: center;
    margin-bottom: 20px;
}
.logo img {
    width: 100px;
}
.label {
    display: block;
    margin-top: 10px;
    font-weight: bold;
}
.input-container {
    display: flex;
    align-items: center;
    margin-bottom: 15px;
    font-weight: bold;
}
.input-container span {
    width: 150px;
    margin-right: 10px;
}
.input-container input {
    flex-grow: 1;
    height: 30px;
}
.btn-container {
    text-align: center;
}
.btn {
    width: 150px;
    height: 35px;
    background-color: #000000;
    border: none;
    color: #ffffff;
    font-weight: bold;
    cursor: pointer;
}
.nearby-wifi {
    margin-top: 20px;
    text-align: center;
    font-style: italic;
    color: #808080;
}
.small-text {
    font-size: smaller;
    color: red;
}
</style>
</head>
<body>
<div class="container">
    <form method="POST" action="configwifi">
        <label class="label">
           <span class="small-text">Tip: Enter the wifi account password that is connected in the ssid and password</span><br><br>
            
        </label>
        <div class="input-container">
            <span>WiFi SSID:</span>
            <input type="text" name="ssid" value="">
        </div>
        <label class="label">
            
        </label>
        <div class="input-container">
            <span>WiFi Password:</span>
            <input type="text" name="pass">
        </div>
        <label class="label">
        
        </label>
        <div class="input-container">
            <span>Send Name:</span>
            <input type="text" name="SendName">
        </div>
        <label class="label">

        </label>
        <div class="input-container">
            <span>Send Rate:</span>
            <input type="text" name="SendRate">
        </div>
        <label class="label">

        </label>
        <div class="input-container">
            <span>Wifi Ip:</span>
            <input type="text" name="WifiIp">
        </div>
        <label class="label">

        </label>
        <div class="input-container">
            <span>Sensor Port:</span>
            <input type="text" name="port">
        </div>

        <div class="input-container">
            <span>Filter Selection:</span>
            <select name="selection" style="flex-grow: 1;text-align-last: center; height: 30px;">
                <option value="OFF">OFF</option>
                <option value="ON">ON</option>
            </select>
        </div>

        <div class="btn-container">
            <input class="btn" type="submit" name="submit" value="Submit">
        </div>
    </form>
    <p class="nearby-wifi">
        Nearby WiFi networks: 
    </p>
</div>
</body>
</html>
)rawliteral";
/*
 * 处理网站根目录的访问请求
 */
void handleRoot() 
{
  if (server.hasArg("selectSSID")) {
    server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");   //scanNetWprksID是扫描到的wifi
  } else {
    server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>");   
  }
}
 
/*
 * 提交数据后的提示页面
 */
void handleConfigWifi()               // 返回http状态
{
  if (server.hasArg("ssid"))          // 判断是否有账号参数
  {
    Serial.print("got ssid:");
    wifi_ssid = server.arg("ssid");   // 获取html表单输入框name名为"ssid"的内容
 
    Serial.println(wifi_ssid);
  } 
  else                                // 没有参数
  { 
    Serial.println("error, not found ssid");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid");     // 返回错误页面
    return;
  }
  // 密码与账号同理
  if (server.hasArg("pass")) 
  {
    Serial.print("got password:");
    wifi_pass = server.arg("pass");      // 获取html表单输入框name名为"pwd"的内容
    Serial.println(wifi_pass);
  } 
  else 
  {
    Serial.println("error, not found password");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
    return;
  }

  // 获取imu发送数据的名称
  if (server.hasArg("SendName")) 
  {
    MySendName = server.arg("SendName");      // 获取html表单输入框name名为"SendName"的内容
    if(MySendName.length() == 0)
    {
      MySendName = "ESP32_test";
    }
    Input_NVSString(NVS_Name, "MyString", MySendName);//将"num"字符串存入创建的nvs空间，并取键名为MyString
    Serial.print("got SendName:");
    Serial.println(MySendName);
  }
  else 
  {
    MySendName = "ESP32_test";
    Input_NVSString(NVS_Name, "MyString", MySendName);//将"num"字符串存入创建的nvs空间，并取键名为MyString
  }

  // 获取imu的发数据频率
  if (server.hasArg("SendRate")) 
  {
    Rate = server.arg("SendRate").toInt();      // 获取html表单输入框name名为"SendRate"的内容
    if(Rate <= 0 || Rate > 100)
    {
      Rate = 100;
    }
    Save_Rerate(Rate);
    Input_NVSInt(NVS_Name, "MyINT", Rate);//将Num_Int变量的数值存入创建的nvs空间，并取键名为MyINT
    Serial.print("got SendRate:");
    Serial.println(Rate);
  } 

  // 获取wifi要连接的地址，默认为192.168.1.103
  if (server.hasArg("WifiIp")) 
  {
    Wifi_ip = server.arg("WifiIp");      // 获取html表单输入框name名为"WifiIp"的内容
    if(Wifi_ip.length() == 0)
    {
      Wifi_ip = "192.168.1.103";
    }
    Input_NVSString(NVS_Name, "MyIP", Wifi_ip);//将"num"字符串存入创建的nvs空间，并取键名为MyString
    Serial.print("got Wifi_ip1:");
    Serial.println(Wifi_ip);
  }
  else 
  {
    Wifi_ip = "192.168.1.103";
    Input_NVSString(NVS_Name, "MyIP", Wifi_ip);//将"num"字符串存入创建的nvs空间，并取键名为MyString
  }

  // // id name
  // if (server.hasArg("name")) 
  // {
  //   Serial.print("got name:");
  //   sensor_name = server.arg("name");  //获取html表单输入框name名为"pwd"的内容
  //   Serial.println(sensor_name);
  // } 


    // port
  if (server.hasArg("port")) 
  {
    Serial.print("got port:");
    sensor_port = server.arg("port").toInt();  // 获取html表单输入框name名为"pwd"的内容
    Serial.println(sensor_port);
  } 
  else 
  { 
    sensor_port = 8080;
  }

  // 获取选择是否使用自带的滤波算法
  if (server.hasArg("selection")) // 判断是否有选择参数
  {
    Serial.print("got selection:");
    Choose = server.arg("selection"); // 获取html表单名为 "selection" 的选择
    Serial.println(Choose);
    Input_NVSString(NVS_Name, "Myselection", Choose);//将"num"字符串存入创建的nvs空间，并取键名为MyString
  } 
  else // 没有参数
  {
    Serial.println("error, not found selection");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found selection");
    return;
  }

  server.send(200, "text/html", "<meta charset='UTF-8'>SSID：" + wifi_ssid + "<br />password:" + wifi_pass + "<br />Send Name:" + MySendName + "<br />Send Rate:" + Rate + "<br />Wifi ip:" + Wifi_ip + "<br />已取得WiFi信息,正在尝试连接,请手动关闭此页面。"); //返回保存成功页面
  delay(2000);
  WiFi.softAPdisconnect(true);     // 参数设置为true，设备将直接关闭接入点模式，即关闭设备所建立的WiFi网络。
  server.close();                  // 关闭web服务
  WiFi.softAPdisconnect();         // 在不输入参数的情况下调用该函数,将关闭接入点模式,并将当前配置的AP热点网络名和密码设置为空值.
  Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass + "Send Name:" + MySendName + "Send Rate:" + Rate);
 
  if (WiFi.status() != WL_CONNECTED)    //wifi没有连接成功
  {
    Serial.println("开始调用连接函数connectToWiFi()..");
    connectToWiFi(connectTimeOut_s);
  } 
  else {
    Serial.println("提交的配置信息自动连接成功..");
  }
}





/*
 * 处理404情况的函数'handleNotFound'
 */
void handleNotFound()           // 当浏览器请求的网络资源无法在服务器找到时通过此自定义函数处理
{           
  handleRoot();                 // 访问不存在目录则返回配置页面
  //   server.send(404, "text/plain", "404: Not found");
}
 
/*
 * 进入AP模式
 */
void initSoftAP() {
  WiFi.mode(WIFI_AP);                                           // 配置为AP模式
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // 设置AP热点IP和子网掩码
  if (WiFi.softAP(AP_SSID))                                     // 开启AP热点,如需要密码则添加第二个参数
  {                           
    // 打印相关信息
    Serial.println("ESP-32S SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());                                                // 接入点ip
    Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());    // 接入点mac
  } 
  else                                                  // 开启AP热点失败
  { 
    Serial.println("WiFiAP Failed");
    delay(1000);
    Serial.println("restart now...");
    ESP.restart();                                      // 重启复位esp32
  }
}
 
/*
 * 开启DNS服务器
 */
void initDNS() 
{
  if (dnsServer.start(DNS_PORT, "*", apIP))   // 判断将所有地址映射到esp32的ip上是否成功
  {
    Serial.println("start dnsserver success.");
  } else {
    Serial.println("start dnsserver failed.");
  }
}
 
/*
 * 初始化WebServer
 */
void initWebServer() 
{
  if (MDNS.begin("esp32"))      // 给设备设定域名esp32,完整的域名是esp32.local
  {
    Serial.println("MDNS responder started");
  }
  //必须添加第二个参数HTTP_GET，以下面这种格式去写，否则无法强制门户
  server.on("/", HTTP_GET, handleRoot);                      //  当浏览器请求服务器根目录(网站首页)时调用自定义函数handleRoot处理，设置主页回调函数，必须添加第二个参数HTTP_GET，否则无法强制门户
  server.on("/configwifi", HTTP_POST, handleConfigWifi);     //  当浏览器请求服务器/configwifi(表单字段)目录时调用自定义函数handleConfigWifi处理
                                                            
  server.onNotFound(handleNotFound);                         // 当浏览器请求的网络资源无法在服务器找到时调用自定义函数handleNotFound处理
 
  server.begin();                                           // 启动TCP SERVER
 
  Serial.println("WebServer started!");
}
 
/*
 * 扫描附近的WiFi，为了显示在配网界面
 */
bool scanWiFi() {
  Serial.println("scan start");
  Serial.println("--------->");
  // 扫描附近WiFi
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    scanNetworksID = "no networks found";
    return false;
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
      delay(10);
    }
    return true;
  }
}
 
/*
 * 连接WiFi
 */
void connectToWiFi(int timeOut_s) 
{
  WiFi.hostname(HOST_NAME);             //设置设备名
  WiFi.mode(WIFI_STA);                        //设置为STA模式并连接WIFI
  WiFi.setAutoConnect(true);                  //设置自动连接    
  Serial.print("WiFi模式为:");
  Serial.println("终端模式");  
  if (wifi_ssid != "")                        //wifi_ssid不为空，意味着从网页读取到wifi
  {
    Serial.println("正在使用网页端设置的账号密码连接.");
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str()); //c_str(),获取该字符串的指针
    wifi_ssid = "";
    wifi_pass = "";
  } 
  else                                        //未从网页读取到wifi
  {
    Serial.println("正在使用nvs保存的账号密码连接.");
    MySendName = Output_NVSString(NVS_Name, "MyString");
    Choose = Output_NVSString(NVS_Name, "Myselection");
    Wifi_ip = Output_NVSString(NVS_Name, "MyIP");
    //Serial.print("Choose的内容为:");
    //Serial.println(Choose);
    if(Output_NVSInt(NVS_Name, "MyINT") > 0)
    {
      Rate = Output_NVSInt(NVS_Name, "MyINT");
      Serial.print("Rate:");
      Serial.print(Rate);
      Save_Rerate(Rate);
    }
    else
    {
      Save_Rerate(100);
    }
    WiFi.begin();                             //begin()不传入参数，默认连接上一次连接成功的wifi
  }
  Serial.print("WiFi连接中");
  int i = 0;
  ///*
  while (WiFi.status() != WL_CONNECTED)       //等待WIFI连接成功
  {  
    button.loop();
    Serial.print(".");                        //一共打印30个点
    i++;
    if(i > 12000)//等待连接时间超过2分钟过长，则重启esp32
    {
      ESP.restart();              // 重启复位esp32
    }
    if(0 == Output_NVSInt(NVS_Name, "MyTAG"))
    {
      LED_PWMcontrol();           //呼吸灯控制 
    }
    if(1 == Output_NVSInt(NVS_Name, "MyTAG"))
    {
      for(int j = 0; j < 20; j++)
      {
        LED_PWMcontrol();           //呼吸灯控制 
      }
      LED_PWMClose();
      wifiConfig();                           //开始配网功能
      Input_NVSInt(NVS_Name, "MyTAG", 0);//MYTAG设为1，则不会进入AP模式
      return;                                 //跳出防止无限初始化
      
    }
  }
  //*/

  if (WiFi.status() == WL_CONNECTED)          //如果连接成功
  {
    //Input_NVSInt(NVS_Name, "MyTAG", 0);//MYTAG设为1，则不会进入AP模式
    if(Config_localtime_flag == true)
    {
      Config_localtime(); //获取网络时间，配置本地时间
      Config_localtime_flag = false;
    }
    Serial.println(" ");
    Serial.println("WIFI连接成功");
    Serial.printf("SSID:%s", WiFi.SSID().c_str());
    Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
    Serial.print("LocalIP:");
    Serial.print(WiFi.localIP());
    Serial.print(" ,GateIP:");
    Serial.println(WiFi.gatewayIP());
    LED_PWMOpen();
    server.stop();                            // 停止开发板所建立的网络服务器。
  }
}

/*
 * 配置配网功能
 */
void wifiConfig() 
{
  initSoftAP();   
  initDNS();        
  initWebServer();  
  scanWiFi();       
}


/*
 * 删除保存的wifi信息，这里的删除是删除存储在flash的信息。删除后wifi读不到上次连接的记录，需重新配网
 */
void restoreWiFi() {
  delay(500);
  //esp_wifi_stop();
  esp_wifi_restore();  //删除保存的wifi信息
  delay(10);
  Serial.println("连接信息已清空..");
  //blinkLED(LED, 5, 500); //LED闪烁5次         //关于LED，不需要可删除 
  //digitalWrite(LED, LOW);                    //关于LED，不需要可删除
}

/*
 * 检查wifi是否已经连接
 */
void checkConnect(bool reConnect) 
{
  if (WiFi.status() != WL_CONNECTED)           //wifi连接失败
  {
    if (reConnect == true && WiFi.getMode() != WIFI_AP && WiFi.getMode() != WIFI_AP_STA ) 
    {
      connectToWiFi(connectTimeOut_s);          //连接wifi函数 
    }
    WiFiU_conflag = false;
    //digitalWrite(LED, !digitalRead(LED));//等待配置网页，LED快速闪烁
    LED_PWMOpen();
    delay(30);
    LED_PWMClose();
    delay(30);
  } 
  else
  {
    LED_PWMOpen();                    //wifi连接成功
    //Serial.println("WiFi连接成功!!");
    //Input_NVSInt(NVS_Name, "MyTAG", 0);//MYTAG设为1，则不会进入AP模式
    if(Config_localtime_flag == true)
    {
      Config_localtime(); //获取网络时间，配置本地时间
      Config_localtime_flag = false;
    }
    WiFiU_conflag = true;
  }
}

/*
 * LED闪烁函数        //用不上LED可删除
 */
void blinkLED(int led, int n, int t) 
{
  for (int i = 0; i < 2 * n; i++) 
  {
    digitalWrite(led, !digitalRead(led));
    delay(t);
  }
}


/*
 * LED初始化
 */
void LEDinit()
{
  pinMode(LED, OUTPUT);                 // 配置LED口为输出口
  digitalWrite(LED, LOW);               // 初始灯灭
}

/*
 * 检测客户端DNS&HTTP请求
 */
void checkDNS_HTTP()
{
  dnsServer.processNextRequest();   // 检查客户端DNS请求
  server.handleClient();            // 检查客户端(浏览器)http请求
}


// String getSensorName()
// {
//   return sensor_name;
// }

int getSensorPort(){
  return sensor_port;
}


/*
 * 读取当前设置的发生频率
 */
int WiFi_Read_Rate(void)
{
  return Rate;
}


/*
 * 读取当前滤波选择
 */

int WiFi_Read_Choose(void)
{
  if(Choose == "OFF")
  {
    //Serial.println("Choose:"); //调试用
    //Serial.println(Choose); //调试用
    return 0;
  }
  return 1;
}
 