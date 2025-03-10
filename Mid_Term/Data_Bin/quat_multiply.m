function q3 = quat_multiply(q1, q2)
    % 输入：q1 和 q2 是两个四元数，形式为 [w, x, y, z]
    % 输出：q3 是它们的乘积，形式为 [w3, x3, y3, z3]

    w1 = q1(:,1); x1 = q1(:,2); y1 = q1(:,3); z1 = q1(:,4);
    w2 = q2(1); x2 = q2(2); y2 = q2(3); z2 = q2(4);
    
    w3 = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    x3 = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    y3 = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
    z3 = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
    
    q3 = [w3, x3, y3, z3];  % 返回四元数结果
end