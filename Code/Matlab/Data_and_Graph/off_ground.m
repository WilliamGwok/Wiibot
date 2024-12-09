% 数据加载
data = readtable('off_ground.csv', 'Delimiter', ';');
data.Properties.VariableNames = {'Timestamp', 'Torque_Left', 'Torque_Right'};
color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];
% 转换数据类型
time = data.Timestamp / 1000000;
torque_left = -data.Torque_Left / 8;
torque_right = -data.Torque_Right / 8;
a = zeros(1,1285);
a = a + 0.5;
% 作图
figure;
plot(time, torque_left, 'b-', 'LineWidth', 1.5,'Color', color_org); % 左关节扭矩，蓝色
hold on;
plot(time, torque_right, 'r-', 'LineWidth', 1.5,'Color', color_fit); % 右关节扭矩，红色
hold on;
plot(time, a, 'r-', 'LineWidth', 1.5); % 右关节扭矩，红色
hold off;

% 图形设置
xlabel('时间 (s)');
ylabel('输出扭矩 (Nm)');
title('机器人关节电机输出扭矩');
legend('左关节扭矩', '右关节扭矩');
grid on;
