% 加载数据
data = readtable('jump.csv', 'Delimiter', ';');
color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];
% 转换时间为秒
time = data.Timestamp / 1e6; % 从微秒转换为秒

% 提取其他变量
position = data.out;       % 测得位置（外环输出）
speed = data.out_1;        % 测得速度（内环输出）
target_leg_length = data.target; % 机器人腿长目标值

% 筛选3.5到7秒的数据
filter_idx = (time >= 3.5) & (time <= 7);
time_filtered = time(filter_idx);
position_filtered = position(filter_idx);
speed_filtered = speed(filter_idx);
target_leg_length_filtered = target_leg_length(filter_idx)*10;

% 创建图形
figure;

% 绘制位置
yyaxis left;
plot(time_filtered, position_filtered, '-b', 'DisplayName', 'Measured Position','Color', color_org, 'LineWidth', 2);
ylabel('Position (Outer Loop)');
hold on;

% 绘制速度
yyaxis right;
plot(time_filtered, speed_filtered, '-r', 'DisplayName', 'Measured Speed','Color', color_fit, 'LineWidth', 2);
hold on;

% 绘制目标腿长
yyaxis right;
plot(time_filtered, target_leg_length_filtered, '-g', 'DisplayName', 'Target Leg Length', 'LineWidth', 2);
ylabel('Speed and Target Leg Length');

% 添加标签和标题
xlabel('Time (s)');
title('Position, Speed, and Target Leg Length (3.5 to 7s)');
legend('show');
grid on;
