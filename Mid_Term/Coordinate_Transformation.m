clc;
clear;

org_data = readtable('first_try.csv');

q0 = 0.5; q1 = -0.5; q2 = 0.5; q3 = 0.5;

%% 将设备坐标系下测得的角速度转为世界坐标系下的角速度
accx_org = table2array(org_data(:,"Var2"));
accy_org = table2array(org_data(:,"Var3"));
accz_org = table2array(org_data(:,"Var4"));

[accx, accy, accz] = Gyro2World(q0, q1, q2, q3, accx_org, accy_org, accz_org);

%% 将设备坐标系下测得的角加速度转为世界坐标系下的角加速度
gyrox_org = table2array(org_data(:,"Var5"));
gyroy_org = table2array(org_data(:,"Var6"));
gyroz_org = table2array(org_data(:,"Var7"));

[gyrox, gyroy, gyroz] = Gyro2World(q0, q1, q2, q3, gyrox_org, gyroy_org, gyroz_org);

%% 将四元数转为欧拉角
q0_org = table2array(org_data(:,"Var8"));
q1_org = table2array(org_data(:,"Var9"));
q2_org = table2array(org_data(:,"Var10"));
q3_org = table2array(org_data(:,"Var11"));


%% 数据可视化

time = datetime(org_data.A1, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');


%% 绘制加速度数据（分成3个子图）
figure;
subplot(3,1,1);
plot(time, accx, 'r', 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccX (m/s^2)');
title('Acceleration - X');
grid on;

subplot(3,1,2);
plot(time, accy, 'g', 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccY (m/s^2)');
title('Acceleration - Y');
grid on;

subplot(3,1,3);
plot(time, accz, 'b', 'LineWidth', 1.5);
xlabel('Time'); ylabel('AccZ (m/s^2)');
title('Acceleration - Z');
grid on;

%% 绘制角加速度数据（分成3个子图）
figure;
subplot(3,1,1);
plot(time, gyrox, 'r', 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroX (rad/s)');
title('Angular Velocity - X');
grid on;

subplot(3,1,2);
plot(time, gyroy, 'g', 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroY (rad/s)');
title('Angular Velocity - Y');
grid on;

subplot(3,1,3);
plot(time, gyroz, 'b', 'LineWidth', 1.5);
xlabel('Time'); ylabel('GyroZ (rad/s)');
title('Angular Velocity - Z');
grid on;



