import serial
import openpyxl
import threading
import time
from datetime import datetime
from collections import deque

# 宏定义: 0 = Dear PyGui, 1 = Matplotlib
USE_MATPLOTLIB = 1

if USE_MATPLOTLIB:
    import matplotlib.pyplot as plt
    import matplotlib.animation as animation
else:
    import dearpygui.dearpygui as dpg

# 串口配置
SERIAL_PORT = 'COM8'
BAUD_RATE = 115200

# 连接串口
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Excel 初始化
file_path = "arduino_data.xlsx"
wb = openpyxl.Workbook()
sheet = wb.active
sheet.append(["Time", "GyroX", "GyroY", "GyroZ"])

# 数据缓存
data_buffer = []
plot_buffer = deque(maxlen=100)
buffer_lock = threading.Lock()

running = True  # 线程运行标志

# 数据采集线程
def read_serial():
    global running
    while running:
        try:
            data = ser.readline().decode(errors="ignore").strip()
            if not data:
                continue
            values = data.split(", ")
            if len(values) == 3:
                try:
                    gyrox, gyroy, gyroz = map(float, values)
                    timestamp = datetime.now()
                    with buffer_lock:
                        data_buffer.append([timestamp, gyrox, gyroy, gyroz])
                        plot_buffer.append([timestamp, gyrox, gyroy, gyroz])
                    if len(data_buffer) >= 10:
                        for row in data_buffer:
                            sheet.append(row)
                        data_buffer.clear()
                        print("📄 批量写入 Excel")
                except ValueError:
                    print(f"⚠ 数据格式错误: {data}")
        except Exception as e:
            print(f"⚠ 读取串口数据时出错: {e}")
    print("🛑 数据采集线程已停止")

serial_thread = threading.Thread(target=read_serial, daemon=True)
serial_thread.start()

# Matplotlib 绘图
if USE_MATPLOTLIB:
    fig, ax = plt.subplots()
    ax.set_ylim(-1, 1)
    ax.set_title("Real-time Gyro Data")
    ax.set_xlabel("Time")
    ax.set_ylabel("Gyro Value")

    line1, = ax.plot([], [], 'r-', label="GyroX")
    line2, = ax.plot([], [], 'g-', label="GyroY")
    line3, = ax.plot([], [], 'b-', label="GyroZ")
    ax.legend()

    def update_plot(frame):
        if len(plot_buffer) == 0:
            return line1, line2, line3
        times = range(len(plot_buffer))
        gyrox_values = [data[1] for data in plot_buffer]
        gyroy_values = [data[2] for data in plot_buffer]
        gyroz_values = [data[3] for data in plot_buffer]
        line1.set_data(times, gyrox_values)
        line2.set_data(times, gyroy_values)
        line3.set_data(times, gyroz_values)
        ax.set_xlim(0, len(times))
        return line1, line2, line3

    ani = animation.FuncAnimation(fig, update_plot, interval=10, blit=False)
    plt.show()
else:
    # Dear PyGui 界面
    dpg.create_context()
    with dpg.window(label="实时陀螺仪数据", width=600, height=400):
        dpg.add_text("数据监测")
        with dpg.plot(label="Gyro Data", width=500, height=300):
            dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis")
            dpg.add_plot_axis(dpg.mvYAxis, label="Gyro Value", tag="y_axis")
            dpg.set_axis_limits("y_axis", -400, 400)
            dpg.set_axis_limits("x_axis", 10, 40)
            dpg.add_line_series([], [], label="GyroX", parent="y_axis", tag="GyroX")
            dpg.add_line_series([], [], label="GyroY", parent="y_axis", tag="GyroY")
            dpg.add_line_series([], [], label="GyroZ", parent="y_axis", tag="GyroZ")
        dpg.add_button(label="Close", callback=lambda: dpg.stop_dearpygui())
    dpg.create_viewport(title='IMU Data Visualization', width=600, height=400)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    while dpg.is_dearpygui_running():
        with buffer_lock:
            if len(plot_buffer) > 0:
                times = list(range(len(plot_buffer)))
                gyrox_values = [data[1] for data in plot_buffer]
                gyroy_values = [data[2] for data in plot_buffer]
                gyroz_values = [data[3] for data in plot_buffer]
                dpg.set_value("GyroX", [times, gyrox_values])
                dpg.set_value("GyroY", [times, gyroy_values])
                dpg.set_value("GyroZ", [times, gyroz_values])
        dpg.render_dearpygui_frame()
    dpg.destroy_context()

# 结束处理
def cleanup():
    global running
    print("🛑 停止数据采集...")
    running = False
    serial_thread.join()
    print("🔄 线程已停止")
    wb.save(file_path)
    ser.close()
    print("💾 数据已保存，串口已关闭")

cleanup()

