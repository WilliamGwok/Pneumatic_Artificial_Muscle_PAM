# -*- coding: utf-8 -*-
import serial
import openpyxl
import threading
import time
from datetime import datetime
from collections import deque
import re

# �Ƿ�ʹ�� Matplotlib ��ʾʵʱͼ��
USE_MATPLOTLIB = 1

if USE_MATPLOTLIB:
    import matplotlib.pyplot as plt
    import matplotlib.animation as animation

# ��������
SERIAL_PORT = 'COM9'
BAUD_RATE = 115200

# ���Ӵ���
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Excel ��ʼ��
file_path = r"E:\Files\大四下\毕业设计\Github\Sensor_Board\Code\Get_Voltage_Data\adc_voltage_log.xlsx"
wb = openpyxl.Workbook()
sheet = wb.active
sheet.append(["ʱ��", "ADC ֵ", "��ѹ (V)"])

# ���ݻ���
data_buffer = []
plot_buffer = deque(maxlen=100)
buffer_lock = threading.Lock()

running = True  # �����߳�����

# ���ݲɼ��߳�
def read_serial():
    global running
    while running:
        try:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue

            # ʹ������ƥ�� "ADC Value: xxxx  Voltage: x.xxx V"
            match = re.search(r'ADC Value:\s*(\d+)\s*Voltage:\s*([\d.]+)', line)
            if match:
                adc_val = int(match.group(1))
                voltage = float(match.group(2))
                timestamp = datetime.now()
                with buffer_lock:
                    data_buffer.append([timestamp, adc_val, voltage])
                    plot_buffer.append(voltage)
                if len(data_buffer) >= 10:
                    for row in data_buffer:
                        sheet.append(row)
                    data_buffer.clear()
                    print("? ����д�� Excel")
            else:
                print(f"?? ���ݸ�ʽ��ƥ��: {line}")

        except Exception as e:
            print(f"? ��ȡ�������ݳ���: {e}")
    print("? ���ݲɼ��߳���ֹͣ")

serial_thread = threading.Thread(target=read_serial, daemon=True)
serial_thread.start()

# ��ѡ��Matplotlib ʵʱ��ʾ
if USE_MATPLOTLIB:
    fig, ax = plt.subplots()
    line, = ax.plot([], [], lw=2)
    ax.set_ylim(0, 5)
    ax.set_xlim(0, 100)
    ax.set_ylabel("Voltage (V)")
    ax.set_title("Real-time Voltage Plot")

    def update(frame):
        with buffer_lock:
            y_data = list(plot_buffer)
        x_data = list(range(len(y_data)))
        line.set_data(x_data, y_data)
        return line,

    ani = animation.FuncAnimation(fig, update, blit=True, interval=200)
    plt.show()

# �رճ���ʱ���� Excel
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    running = False
    serial_thread.join()
    wb.save(file_path)
    print(f"? �����ѱ��浽: {file_path}")
