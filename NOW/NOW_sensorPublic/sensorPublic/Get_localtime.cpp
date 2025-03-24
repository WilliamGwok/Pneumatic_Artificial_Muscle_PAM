#include "Get_localtime.h"
unsigned long bootTimeMillis = 0;
time_t bootTime;
char timeStr[64]; // 保存获取的网络时间

// NTP 服务器设置
const char *ntpServer = "europe.pool.ntp.org";
const long gmtOffset_sec = 8 * 3600; // UTC+8 时区（北京时间）
const int daylightOffset_sec = 0;
static bool tag = false;

void Config_localtime(void)
{
  // 从网络时间服务器上获取并设置时间
  // 获取成功后芯片会使用RTC时钟保持时间的更新
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // 等待时间同步
  struct tm timeinfo;
  int i = 0;
  Serial.print("tag1");
  Serial.print("等待获取本地时间");
  while (!getLocalTime(&timeinfo))
  {
    i++;
    Serial.print("i:");
    Serial.println(i);
    LED_PWMOpen();              //点亮LED
    delay(50);
    LED_PWMClose();             //关闭LED
    delay(50);
    Serial.print(".");
    if(i>5)
    {
      Serial.println(" ");
      Serial.println("获取本地时间失败失败!");
      return;
    }
  }
  Serial.println("网络时间同步成功，本地时间获取成功!");
  // 记录启动时间的毫秒数和秒数
  bootTimeMillis = millis();
  time(&bootTime);
  tag = true;
  printLocalTime();
}

void printLocalTime(void)
{
  struct tm timeinfo;

  if (!tag)
  {
    Serial.println("Failed to obtain time");
    return;
  }

  // 获取当前时间
  time_t currentTime = time(NULL);
  localtime_r(&currentTime, &timeinfo);

  // 将时间保存为字符串格式
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

  // 获取当前毫秒数
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - bootTimeMillis;

  // 打印时间字符串和毫秒数
  Serial.printf("%s.%03lu\n", timeStr, elapsedMillis % 1000);
}

String Get_localtime(void) // 获取本地时间
{
  struct tm timeinfo;

  if (!tag)
  {
    return String("Failed to obtain time"); // 返回一个错误信息
  }

  // 获取当前时间
  time_t currentTime = time(NULL);
  localtime_r(&currentTime, &timeinfo);

  // 将时间保存为字符串格式
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

  // 获取当前毫秒数
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - bootTimeMillis;

  // 构建带毫秒数的时间字符串
  char fullTimeStr[64];
  snprintf(fullTimeStr, sizeof(fullTimeStr), "%s.%03d", timeStr, static_cast<int>(elapsedMillis % 1000));

  return String(fullTimeStr);
}
