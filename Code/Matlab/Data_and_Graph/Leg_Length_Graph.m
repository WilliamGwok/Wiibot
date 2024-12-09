% 1. 导入数据
data = readtable('Leg_length.csv'); % 使用 readtable 读取 CSV 文件
color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];
% 2. 提取各列数据
time = data{:, 1};  % 第一列是时间
time_p = time / 1000000;
target = data{:, 2};  % 第二列是目标值
measurement = data{:, 3};  % 第三列是测量值

% 3. 设置起始时间
t_start = 9099000;  % 假设你想从时间 t_start = 5 开始绘图
t_end = 13490000;  % 假设你想在时间 t_end = 15 停止绘图
% 4. 筛选出从 t_start 开始的数据
idx = time >= t_start & (time <= t_end);  % 找到时间大于等于 t_start 的数据行
time_filtered = (time(idx) / 1000000) - 9.237;
target_filtered = target(idx);
measurement_filtered = measurement(idx);

% 5. 绘制图形
figure;
plot(time_filtered, target_filtered, 'r--', 'LineWidth', 2,'Color', color_org); % 画目标值
hold on;
plot(time_filtered, measurement_filtered, 'b-', 'LineWidth', 2,'Color', color_fit); % 画测量值
xlabel('Time(s)');
ylabel('Angle(rad)');
legend('Tar', 'Mea');
title(['Comparison between the target angle value and the measured value']);
grid on;
xlim([0,3])
% 6. 计算超调量、峰值时间、调节时间
% 首先，找到目标值的最终稳态（假设目标值在最后的几个点稳定）
steady_state_target = target_filtered(end);  % 目标值的稳态，取最后一个数据点

% 计算目标值的峰值和峰值时间
[peak_value, peak_idx] = max(measurement_filtered);  % 找到测量值的最大值及其索引
peak_time = time_filtered(peak_idx);  % 峰值时间

% 计算超调量
overshoot = (peak_value - steady_state_target) / steady_state_target * 100;

% 计算调节时间（以2%为标准）
settling_idx = find(abs(measurement_filtered - steady_state_target) >= 0.02 * steady_state_target, 1, 'last');
settling_value = 0.02 * steady_state_target + steady_state_target;
settling_time = time_filtered(settling_idx);  % 最后一个符合2%标准的时间点

% 打印结果
fprintf('超调量: %.2f%%\n', overshoot);
fprintf('峰值时间: %.2f 秒\n', peak_time);
fprintf('调节时间: %.2f 秒\n', settling_time);

% 7. 在图形中标出峰值时间和稳态
hold on;
plot(peak_time, peak_value, 'ko', 'MarkerFaceColor', 'k'); % 标出峰值点
text(peak_time + 0.1, peak_value, sprintf('Peak Time: %.2f s', peak_time), 'Color', 'black');

% % 绘制调节时间的标记
plot(settling_time, settling_value, 'ms', 'MarkerFaceColor', 'm'); % 标出调节时间点
text(settling_time + 0.1, steady_state_target, sprintf('Settling Time: %.2f s', settling_time), 'Color', 'magenta');

% 8. 显示超调量
text(peak_time + 0.1, peak_value * 0.9, sprintf('Overshoot: %.2f%%', overshoot), 'Color', 'black');

