function [dot_product, theta] = judgeArmPosture(q_ref, q_current)
    % 计算四元数的内积
    dot_product = dot(q_ref, q_current);

    % 计算旋转角度（弧度）
    theta = abs(rad2deg(2 * acos(dot_product))); % 计算旋转角度（单位：弧度）

%     % 判断手臂姿态
%     if dot_product > 0.9
%         fprintf('手臂接近下垂状态\n');
%     elseif dot_product < -0.5
%         fprintf('手臂显著抬起，可能高过头顶\n');
%     else
%         fprintf('手臂处于中间状态\n');
%     end
% 
%     % 显示计算结果
%     fprintf('内积值: %.4f\n', dot_product);
%     fprintf('旋转角度: %.2f 度\n', theta);
end


