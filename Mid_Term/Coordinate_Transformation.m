clc;
clear;

org_data = readtable('side_front.csv');

q0 = 0.5; q1 = -0.5; q2 = 0.5; q3 = 0.5;
rot_dev2std = [
    0 -1 0;
    0 0 -1;
    1 0 0;];
rot_q = [0.5;-0.5;-0.5;0.5];


% q_current = [0.8917, 0.0441, 0.4401, 0.0958];  % 读取到的四元数（手臂抬起）



%% 将设备坐标系下测得的角速度转为世界坐标系下的角速度
accx_org = table2array(org_data(:,"Var2"))';
accy_org = table2array(org_data(:,"Var3"))';
accz_org = table2array(org_data(:,"Var4"))';

acc = rot_dev2std*[accx_org;accy_org;accz_org];
accx = acc(1,:);
accy = acc(2,:);
accz = acc(3,:);

%% 将设备坐标系下测得的角加速度转为世界坐标系下的角加速度
gyrox_org = table2array(org_data(:,"Var5"))';
gyroy_org = table2array(org_data(:,"Var6"))';
gyroz_org = table2array(org_data(:,"Var7"))';

gyro = rot_dev2std*[gyrox_org;gyroy_org;gyroz_org];
gyrox = gyro(1,:);
gyroy = gyro(2,:);
gyroz = gyro(3,:);

%% 将四元数转为欧拉角
w = table2array(org_data(:,"Var11"));
x = table2array(org_data(:,"Var8"));
y = table2array(org_data(:,"Var9"));
z = table2array(org_data(:,"Var10"));

q_current = [w, x, y, z];

% q_ref = [0.6857, 0.0365, -0.7267, 0];  % 初始参考四元数（手臂下垂）
w_=0;
x_=0;
y_=0;
z_=0;
for i = 1:1:100
    w_ = w_ + w(i,:);
    x_ = x_ + x(i,:);
    y_ = y_ + y(i,:);
    z_ = z_ + z(i,:);
end
q_ref = [w_, x_, y_, z_]/100;

dot_product = zeros(height(q_current),1);
theta = zeros(height(q_current),1);

% 调用函数进行判断
% judgeArmPosture(q_ref, q_current);
for i = 1:1:height(q_current)
    [dot_product(i,:),theta(i,:)] = judgeArmPosture(q_ref, q_current(i,:));
    judgeArmDirection(q_ref, q_current(i,:));
end


%% 数据可视化
time = datetime(org_data.A1, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');

color1 = [0.33, 0.53, 0.53];  % 颜色1
color2 = [0.28, 0.20, 0.21];  % 颜色2
color3 = [0.74, 0.12, 0.12];  % 颜色3

%%绘制加速度数据
figure;
subplot(3,1,1);
plot(time, accx, 'Color', color1, 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccX (m/s^2)');
title('Acceleration - X');
grid on;

subplot(3,1,2);
plot(time, accy, 'Color', color2, 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccY (m/s^2)');
title('Acceleration - Y');
grid on;

subplot(3,1,3);
plot(time, accz, 'Color', color3, 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccZ (m/s^2)');
title('Acceleration - Z');
grid on;

%%绘制角加速度数据
figure;
subplot(3,1,1);
plot(time, gyrox, 'Color', color1, 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroX (rad/s)');
title('Angular Velocity - X');
grid on;

subplot(3,1,2);
plot(time, gyroy, 'Color', color2, 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroY (rad/s)');
title('Angular Velocity - Y');
grid on;

subplot(3,1,3);
plot(time, gyroz, 'Color', color3, 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroZ (rad/s)');
title('Angular Velocity - Z');
grid on;

%%绘制角度数据
% figure;
% subplot(3,1,1);
% plot(time, yaw, 'Color', color1, 'LineWidth', 1.5);
% xlabel('Time'); ylabel('Yaw (°)');
% title('Euler Angle - Yaw');
% grid on;
% 
% subplot(3,1,2);
% plot(time, pitch, 'Color', color2, 'LineWidth', 1.5);
% xlabel('Time'); ylabel('Pitch (°)');
% title('Euler Angle - Pitch');
% grid on;
% 
% subplot(3,1,3);
% plot(time, roll, 'Color', color3, 'LineWidth', 1.5);
% xlabel('Time'); ylabel('Roll (°)');
% title('Euler Angle - Roll');
% grid on;
% figure;
% plot(time, yaw, 'Color', color1, 'LineWidth', 1.5); hold on;
% plot(time, pitch, 'Color', color2, 'LineWidth', 1.5);
% plot(time, roll, 'Color', color3, 'LineWidth', 1.5);
% hold off;
% 
% xlabel('Time');
% ylabel('Angle (°)');
% title('Euler Angles');
% legend({'Yaw', 'Pitch', 'Roll'});
% grid on;

figure;
plot(time, theta, 'Color', color1, 'LineWidth', 1.5);

xlabel('Time');
ylabel('Angle (°)');
title('Arm Angles');
grid on;
