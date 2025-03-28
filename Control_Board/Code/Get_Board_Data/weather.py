import requests
import tkinter as tk
from tkinter import messagebox
import json
import os

# 函数：获取天气数据
def get_weather(city_name, api_key):
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city_name}&appid={api_key}&units=metric&lang=zh_cn"
    response = requests.get(url)
    
    if response.status_code == 200:
        data = response.json()
        main = data['main']
        weather = data['weather'][0]
        temperature = main['temp']
        humidity = main['humidity']
        description = weather['description']
        city = data['name']
        
        return {
            "city": city,
            "temperature": temperature,
            "humidity": humidity,
            "description": description
        }
    else:
        return None

# 函数：保存历史记录
def save_history(weather_info):
    if not os.path.exists('weather_history.json'):
        with open('weather_history.json', 'w', encoding='utf-8') as f:
            json.dump([], f, ensure_ascii=False)
    
    with open('weather_history.json', 'r+', encoding='utf-8') as f:
        history = json.load(f)
        history.append(weather_info)
        f.seek(0)
        json.dump(history, f, ensure_ascii=False)

# 函数：展示天气信息
def display_weather():
    city_name = city_entry.get()
    if not city_name:
        messagebox.showerror("错误", "请输入城市名称")
        return
    
    weather_info = get_weather(city_name, api_key)
    if weather_info:
        weather_text = f"{weather_info['city']} 当前天气情况：\n"
        weather_text += f"温度: {weather_info['temperature']}°C\n"
        weather_text += f"湿度: {weather_info['humidity']}%\n"
        weather_text += f"天气描述: {weather_info['description']}"
        
        # 显示天气信息
        weather_label.config(text=weather_text)
        
        # 保存历史记录
        save_history(weather_info)
    else:
        messagebox.showerror("错误", "无法获取天气数据，请检查城市名。")

# 函数：查看历史记录
def view_history():
    if not os.path.exists('weather_history.json'):
        messagebox.showinfo("历史记录", "暂无历史记录。")
        return
    
    with open('weather_history.json', 'r', encoding='utf-8') as f:
        history = json.load(f)
        if not history:
            messagebox.showinfo("历史记录", "暂无历史记录。")
        else:
            history_text = ""
            for record in history:
                history_text += f"{record['city']} - 温度: {record['temperature']}°C, 湿度: {record['humidity']}%, 描述: {record['description']}\n"
            messagebox.showinfo("历史记录", history_text)

# 创建 GUI 窗口
def create_gui():
    global city_entry, weather_label
    
    window = tk.Tk()
    window.title("天气应用")

    # 输入框
    city_label = tk.Label(window, text="请输入城市名称:")
    city_label.pack(pady=5)
    city_entry = tk.Entry(window, width=30)
    city_entry.pack(pady=5)

    # 获取天气按钮
    get_weather_button = tk.Button(window, text="获取天气", command=display_weather)
    get_weather_button.pack(pady=10)

    # 显示天气信息的标签
    weather_label = tk.Label(window, text="", justify="left")
    weather_label.pack(pady=10)

    # 查看历史记录按钮
    history_button = tk.Button(window, text="查看历史记录", command=view_history)
    history_button.pack(pady=5)

    window.mainloop()

# 主程序
if __name__ == "__main__":
    api_key = "23db81853fb9280abd9c81b48f21d059"  # 替换为你自己的 API 密钥
    create_gui()
