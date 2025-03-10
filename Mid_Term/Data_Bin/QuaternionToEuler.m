function euler_angles = QuaternionToEuler(q)
    % 输入：q = [w, x, y, z] 四元数
    % 输出：euler_angles = [roll, pitch, yaw]，单位为 **弧度**
    
    w = q(1); x = q(2); y = q(3); z = q(4);
    
    % 计算 Roll (X 轴旋转)
    roll = atan2(2 * (w*x + y*z), 1 - 2 * (x^2 + y^2));
    
    % 计算 Pitch (Y 轴旋转)
    pitch = asin(2 * (w*y - z*x));
    
    % 计算 Yaw (Z 轴旋转)
    yaw = atan2(2 * (w*z + x*y), 1 - 2 * (y^2 + z^2));
    
    % 返回欧拉角 [roll, pitch, yaw] (单位：弧度)
    euler_angles = [roll, pitch, yaw];
end
