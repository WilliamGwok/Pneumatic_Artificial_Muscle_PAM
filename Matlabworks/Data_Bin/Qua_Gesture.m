% 示例：定义参考四元数（手臂下垂时）和当前四元数（手臂抬起时）
q_ref = [0.6857, 0.0365, -0.7267, 0];  % 初始参考四元数（手臂下垂）
q_current = [0.8917, 0.0441, 0.4401, 0.0958];  % 读取到的四元数（手臂抬起）

% 调用函数进行判断
judgeArmPosture(q_ref, q_current);



% quat = zeros(height(yaw),4);
% quat_rot = zeros(height(yaw),4);
% euler_angles = zeros(height(yaw),3);
% 
% for i = 1:1:height(yaw)
%     quat(i,:) = eulerToQuaternion(deg2rad(roll(i)) ,deg2rad(pitch(i)),deg2rad(yaw(i)));
% 
%     quat_rot(i,:) = rotateQuaternionByQuaternion(rot_q, quat(i,:));
% 
%     euler_angles(i,:) = QuaternionToEuler(quat_rot(i,:));
% 
%     roll(i,:) = rad2deg(euler_angles(i,1));
%     pitch(i,:) = rad2deg(euler_angles(i,2));
%     yaw(i,:) = rad2deg(euler_angles(i,3));
%     
% end


% q_org = zeros(length(q0_org),1);
% for i = 1:1:length(q0_org)
%     qrg(i) = quat_multiply([q0_org, q1_org, q2_org, q3_org], rot_q);
% end

% q_org = zeros(length(q0_org),4);
% for i = 1:1:length(q0_org)
%     q = [q0_org(i), q1_org(i), q2_org(i), q3_org(i)];
%     
%     % 将四元数转换为旋转矩阵
%     R_q = quat2rotm(q);
%     
%     % 计算新坐标系下的旋转矩阵
%     R_q_new = rot_dev2std * R_q;% * rot_dev2std';
% %     R_q_new = rot_dev2std' * R_q * rot_dev2std;
%     
%     % 将新的旋转矩阵转换回四元数
%     temp = rotm2quat(R_q_new);
%     for j = 1:1:4
%         q_org(i,j) = temp(j);
%     end
% end

% q_org = [q0_org, q1_org, q2_org, q3_org];
% 
% eulerAngles = zeros(height(q_org),3);
% 
% for i = 1 : height(q_org)
% %     eulerAngles(i,:) = [rot_dev2std(3,:);rot_dev2std(2,:);rot_dev2std(1,:)]*rad2deg(quat2euler(q_org(i,:)))';
%     eulerAngles(i,:) = rad2deg(quat2euler(q_org(i,:)));
% end

% q_org = [q0_org, q1_org, q2_org, q3_org];
% eulerAngles = zeros(height(q_org),3);
% % eulerAngles = QuaternionToEuler(q__);
% 
% for i = 1 : height(q_org)
%     
%     [a,b,c] = BMI_Get_EulerAngle(gyrox(i),gyroy(i),gyroz(i),accx(i),accy(i),accz(i));
% 
%     eulerAngles(i,1) = a;%pitch
%     eulerAngles(i,2) = b;%roll
%     eulerAngles(i,3) = c;%yaw
% end
% 
% q_org = [q0_org, q1_org, q2_org, q3_org];
% eulerAngles = zeros(height(q_org),3);
% % eulerAngles = QuaternionToEuler(q__);
% 
% for i = 1 : height(q_org)
%     
%     eulerAngles(i,:) = rad2deg(quat2euler(q_org(i,:))) ;
% 
% %     eulerAngles(i,1) = a;%pitch
% %     eulerAngles(i,2) = b;%roll
% %     eulerAngles(i,3) = c;%yaw
% end

