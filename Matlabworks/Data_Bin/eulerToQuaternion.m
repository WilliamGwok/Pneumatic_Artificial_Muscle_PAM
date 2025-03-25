function q = eulerToQuaternion(roll, pitch, yaw)
    % 输入：欧拉角（roll, pitch, yaw）
    % 输出：四元数 q = [w, x, y, z]

    % 计算欧拉角对应的四元数
    cy = cos(yaw * 0.5);
    sy = sin(yaw * 0.5);
    cr = cos(roll * 0.5);
    sr = sin(roll * 0.5);
    cp = cos(pitch * 0.5);
    sp = sin(pitch * 0.5);
    
    % 四元数的分量
    w = cy * cr * cp + sy * sr * sp;
    x = cy * sr * cp - sy * cr * sp;
    y = cy * cr * sp + sy * sr * cp;
    z = sy * cr * cp - cy * sr * sp;
    
    % 输出四元数 [w, x, y, z]
    q = [w, x, y, z];
end
