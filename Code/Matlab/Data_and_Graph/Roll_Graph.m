% 读取数据
data = readtable('roll.csv');
color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];
% 提取列数据
time = data{:, 1} / 1000000;       % 第一列：时间
roll = data{:, 2};       % 第二列：机器人横滚角
left_leg_length = data{:, 3}; % 第三列：左腿腿长
right_leg_length = data{:, 4}; % 第四列：右腿腿长

% 找到 roll 的极值点
[max_roll, max_idx] = max(roll); % 最大值及其索引
[min_roll, min_idx] = min(roll); % 最小值及其索引

% 设置标注的间隔
interval = 40;

% 创建图形
figure;

% 绘制曲线并设置线宽
hold on;
plot(time, roll, 'r', 'LineWidth', 1.5, 'DisplayName', '横滚角 (Roll)');
plot(time, left_leg_length, 'g', 'LineWidth', 1.5, 'DisplayName', '左腿腿长 (Left Leg Length)','Color', color_org);
plot(time, right_leg_length, 'b', 'LineWidth', 1.5, 'DisplayName', '右腿腿长 (Right Leg Length)','Color', color_fit);

% 在左腿腿长曲线标注值
for i = 1:interval:length(time)
    text(time(i), left_leg_length(i), sprintf('%.2f', left_leg_length(i)), ...
        'Color', color_org, 'FontSize', 8, 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'center');
end

% 在右腿腿长曲线标注值
for i = 1:interval:length(time)
    text(time(i), right_leg_length(i), sprintf('%.2f', right_leg_length(i)), ...
        'Color', color_fit, 'FontSize', 8, 'VerticalAlignment', 'top', 'HorizontalAlignment', 'center');
end

% 标注 roll 的极值点
plot(time(max_idx), max_roll, 'ro', 'MarkerSize', 8, 'DisplayName', 'Roll 最大值');
text(time(max_idx), max_roll, sprintf('Max: %.2f', max_roll), ...
    'Color', 'red', 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'right');

plot(time(min_idx), min_roll, 'ro', 'MarkerSize', 8, 'DisplayName', 'Roll 最小值');
text(time(min_idx), min_roll, sprintf('Min: %.2f', min_roll), ...
    'Color', 'red', 'VerticalAlignment', 'top', 'HorizontalAlignment', 'right');

hold off;

% 添加图例
legend show;

% 添加标签和标题
xlabel('时间 (Time)');
ylabel('值 (Value)');
title('机器人测量数据');

% 网格
grid on;
