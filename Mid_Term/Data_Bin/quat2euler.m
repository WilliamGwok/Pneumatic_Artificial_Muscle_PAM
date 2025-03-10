function eulerAngles = quat2euler(q)
    % 输入：
    % q: 四元数 [qw, qx, qy, qz]，其中 qw 是标量部分
    % 输出：
    % eulerAngles: 欧拉角 [yaw, pitch, roll] (单位：弧度)

    % 提取四元数分量
    qw = q(1);
    qx = q(2);
    qy = q(3);
    qz = q(4);

    % 计算欧拉角
    yaw = atan2(2 * (qw * qz + qx * qy), 1 - 2 * (qy^2 + qz^2));  % 偏航角（Yaw）
    pitch = asin(2 * (qw * qy - qz * qx));                         % 俯仰角（Pitch）
    roll = atan2(2 * (qw * qx + qy * qz), 1 - 2 * (qx^2 + qy^2));  % 横滚角（Roll）

    % 返回欧拉角
    eulerAngles = [yaw, pitch, roll];
end
