function [x_, y_, z_] = Gyro2World(q0, q1, q2, q3, x, y, z)
    % 输入：
    % q0, q1, q2, q3: 四元数（旋转部分）
    % x, y, z: 传感器坐标系下的加速度值
    
    % 输出：
    % ax, ay, az: 世界坐标系下的加速度值

    % 四元数分量
    a = q0;
    b = q1;
    c = q2;
    d = q3;

    % 加速度计算
    x_ = z * (2 * a * c + 2 * b * d) - y * (2 * a * d - 2 * b * c) + x * (a^2 + b^2 - c^2 - d^2);
    y_ = x * (2 * a * d + 2 * b * c) - z * (2 * a * b - 2 * c * d) + y * (a^2 - b^2 + c^2 - d^2);
    z_ = y * (2 * a * b + 2 * c * d) - x * (2 * a * c - 2 * b * d) + z * (a^2 - b^2 - c^2 + d^2);
end
