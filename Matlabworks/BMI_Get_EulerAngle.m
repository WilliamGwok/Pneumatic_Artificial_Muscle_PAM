function [pitch, roll, yaw] = BMI_Get_EulerAngle(gx, gy, gz, ax, ay, az)
    Kp = 0; % 增益
    halfT = 0.001;
    
    persistent q0 q1 q2 q3;
    if isempty(q0)
        q0 = 1.0; q1 = 0.0; q2 = 0.0; q3 = 0.0;
    end
    
    % 归一化加速度计数据
    if ax * ay * az ~= 0
        norm_val = sqrt(ax^2 + ay^2 + az^2);
        ax = ax / norm_val;
        ay = ay / norm_val;
        az = az / norm_val;
        
        vx = 2 * (q1 * q3 - q0 * q2);
        vy = 2 * (q0 * q1 + q2 * q3);
        vz = q0^2 - q1^2 - q2^2 + q3^3;
        
        ex = (ay * vz - az * vy);
        ey = (az * vx - ax * vz);
        ez = (ax * vy - ay * vx);
        
        gx = gx + Kp * ex;
        gy = gy + Kp * ey;
        gz = gz + Kp * ez;
    end
    
    % 更新四元数
    q0temp = q0;
    q1temp = q1;
    q2temp = q2;
    q3temp = q3;
    
    q0 = q0temp + (-q1temp * gx - q2temp * gy - q3temp * gz) * halfT;
    q1 = q1temp + (q0temp * gx + q2temp * gz - q3temp * gy) * halfT;
    q2 = q2temp + (q0temp * gy - q1temp * gz + q3temp * gx) * halfT;
    q3 = q3temp + (q0temp * gz + q1temp * gy - q2temp * gx) * halfT;
    
    % 归一化四元数
    norm_val = sqrt(q0^2 + q1^2 + q2^2 + q3^2);
    q0 = q0 / norm_val;
    q1 = q1 / norm_val;
    q2 = q2 / norm_val;
    q3 = q3 / norm_val;
    
    % 计算欧拉角
    roll = atan2(2 * (q2 * q3 + q0 * q1), q0^2 - q1^2 - q2^2 + q3^2) * (180/pi);
    pitch = -asin(2 * (q1 * q3 - q0 * q2)) * (180/pi);
    yaw = -atan2(2 * (q1 * q2 + q0 * q3), q0^2 + q1^2 - q2^2 - q3^2) * (180/pi);
end
