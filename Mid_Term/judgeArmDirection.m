function judgeArmDirection(q_ref, q_current)
    % 计算四元数的内积
    dot_product = dot(q_ref, q_current);

    % 计算旋转角度（弧度）
    theta = 2 * acos(dot_product); % 计算旋转角度（单位：弧度）
    fprintf('旋转角度: %.2f 度\n', rad2deg(theta));

    % 计算旋转轴
    cross_product = cross(q_ref(2:4), q_current(2:4)); % 计算旋转轴的叉积
    rotation_axis = cross_product / norm(cross_product); % 归一化旋转轴
    fprintf('旋转轴: [%.2f, %.2f, %.2f]\n', rotation_axis(1), rotation_axis(2), rotation_axis(3));

    % 根据旋转轴的方向判断手臂的姿势
    % 假设参考四元数表示手臂垂直下垂时的姿态
    if -rotation_axis(3) > 0.7
        fprintf('手臂向前举\n');
    elseif rotation_axis(1) > 0.7
        fprintf('手臂向侧面举\n');
    else
        fprintf('手臂可能处于其他方向\n');
    end
end

% % 示例：定义参考四元数（手臂下垂时）和当前四元数（手臂举起时）
% q_ref = [0.6857, 0.0365, -0.7267, 0];  % 初始参考四元数（手臂下垂）
% q_current = [0.8917, 0.0441, 0.4401, 0.0958];  % 读取到的四元数（手臂抬起）
% 
% % 调用函数进行判断
% judgeArmDirection(q_ref, q_current);
