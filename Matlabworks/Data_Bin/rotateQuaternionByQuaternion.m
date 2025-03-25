function p_rot = rotateQuaternionByQuaternion(q, p)
    % 输入：
    % q - 旋转四元数 [w, x, y, z]
    % p - 被旋转的四元数 [0, x, y, z] (表示一个三维向量)
    % 输出：
    % p_rot - 旋转后的四元数 [0, x', y', z']

    % 计算 q 的共轭（逆四元数）
    q_conj = [q(1), -q(2), -q(3), -q(4)];
    
    % 计算旋转后的四元数 p' = q * p * q_conj
    p_rot = quaternionMultiply(quaternionMultiply(q, p), q_conj);
end

function q_res = quaternionMultiply(q1, q2)
    % 计算两个四元数的乘积 q1 * q2
    w1 = q1(1); x1 = q1(2); y1 = q1(3); z1 = q1(4);
    w2 = q2(1); x2 = q2(2); y2 = q2(3); z2 = q2(4);
    
    w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
    z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
    
    q_res = [w, x, y, z];
end


