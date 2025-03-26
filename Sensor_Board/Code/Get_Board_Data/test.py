import serial
import openpyxl
import os
import time
from datetime import datetime

# 连接 Arduino 串口
ser = serial.Serial('COM8', 115200, timeout=1)  # 增加 timeout，避免程序卡死

# 创建 Excel 文件
wb = openpyxl.Workbook()
sheet = wb.active
sheet.append(["Time", "GyroX", "GyroY", "GyroZ"])  # 添加 3 轴数据列

# 文件路径
file_path = "arduino_data.xlsx"

# 定义一个带自动重试的保存函数
def save_with_retry(wb, file_path, retries=3, delay=1):
    """尝试多次保存 Excel，防止文件占用报错"""
    for i in range(retries):
        try:
            wb.save(file_path)
            print(f"✅ 数据成功保存到: {file_path}")
            return
        except PermissionError:
            print(f"⚠️ 文件被占用，保存失败，重试 {i + 1}/{retries} ...")
            time.sleep(delay)  # 等待一段时间再重试
    print("❌ 保存失败，请关闭 Excel 文件后重试！")

# **主循环**
try:
    data_buffer = []  # 用于批量存储数据，提高效率
    while True:
        try:
            data = ser.readline().decode(errors="ignore").strip()  # 忽略解码错误
        except Exception as e:
            print(f"❌ 读取串口数据时出错: {e}")
            continue  # 跳过错误，继续采集

        if data:
            print(f"📡 Received: {data}")

            # 按逗号分割数据
            values = data.split(", ")

            # 确保数据格式正确（应有 3 个数值）
            if len(values) == 3:
                try:
                    gyrox = float(values[0])  # 转换为浮点数
                    gyroy = float(values[1])
                    gyroz = float(values[2])

                    # 存入数据缓冲区
                    data_buffer.append([datetime.now(), gyrox, gyroy, gyroz])

                    # **每 10 行数据写入 Excel**
                    if len(data_buffer) >= 10:
                        for row in data_buffer:
                            sheet.append(row)
                        data_buffer.clear()  # 清空缓冲区
                        print("📄 批量写入 Excel，暂不保存")

                except ValueError:
                    print(f"❌ 数据格式错误: {data}")  # 防止转换错误
            else:
                print(f"⚠️ 收到不完整数据: {data}")

except KeyboardInterrupt:
    print("🛑 数据采集结束，正在保存 Excel...")
    
    # **写入剩余数据**
    if data_buffer:
        for row in data_buffer:
            sheet.append(row)

    # **程序结束时保存 Excel**
    save_with_retry(wb, file_path)
    ser.close()
    print("✅ 串口已关闭，数据保存完成")
