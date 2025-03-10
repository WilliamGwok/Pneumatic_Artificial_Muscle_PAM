clc;
clear;
close all;

% === 配置串口 ===
port = "COM3"; % 修改为你的BNO055串口号
baudrate = 115200;
s = serialport(port, baudrate);
pause(2);

% === 创建3D模型 ===
figure('Name', 'BNO055 实时姿态可视化');
axis([-1 1 -1 1 -1 1]);
grid on;
hold on;
xlabel('X'); ylabel('Y'); zlabel('Z');
title('BNO055 实时姿态可视化');
view(45,30);

% === 创建3D姿态对象 ===
[x, y, z] = sphere(10);
h = surf(0.1*x, 0.1*y, 0.1*z, 'FaceColor', 'b');

% === 读取数据并可视化 ===
while true
    % === 读取一行数据 ===
    line = readline(s);
    data = str2double(split(line, ','));
    if length(data) ~= 4
        continue;
    end

    % === 获取四元数 ===
    w = data(1); x = data(2); y = data(3); z = data(4);

    % === 将四元数转换为欧拉角 ===
    [yaw, pitch, roll] = quat2euler(w, x, y, z);

    % === 计算旋转矩阵 ===
    R = eul2rotm([yaw pitch roll] * pi/180, 'ZYX');

    % === 更新3D模型姿态 ===
    rotate(h, [1 0 0], roll);
    rotate(h, [0 1 0], pitch);
    rotate(h, [0 0 1], yaw);

    % === 显示角度 ===
    title(sprintf('Yaw: %.2f, Pitch: %.2f, Roll: %.2f', yaw, pitch, roll));
    drawnow;
end

function [yaw, pitch, roll] = quat2euler(w, x, y, z)
    roll = atan2(2*(w*x + y*z), 1 - 2*(x^2 + y^2));
    pitch = asin(2*(w*y - z*x));
    yaw = atan2(2*(w*z + x*y), 1 - 2*(y^2 + z^2));

    roll = rad2deg(roll);
    pitch = rad2deg(pitch);
    yaw = rad2deg(yaw);
end
