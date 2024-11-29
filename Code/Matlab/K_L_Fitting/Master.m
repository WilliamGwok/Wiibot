clear;
%% 1.得到系统表达式

syms mb mw I_w I_b I_z rw D L g

% 设置除腿长外的机械参数
mec_sub    = [mb          mw         I_w         I_b         I_z         rw       D       g   ];
mec_config = [4.73        0.2        0.000175    0.007034    0.023312    0.044    0.21    9.81];

% 列些求得的系统A,B矩阵
A_b = [0, 1,                                                                                                                             0, 0, 0, 0;
       0, 0,                                -(L^2*g*mb^2*rw^2)/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2), 0, 0, 0;
       0, 0,                                                                                                                             0, 1, 0, 0;
       0, 0, (L*g*mb^2*rw^2 + 2*L*g*mw*mb*rw^2 + 2*I_w*L*g*mb)/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2), 0, 0, 0;
       0, 0,                                                                                                                             0, 0, 0, 1;
       0, 0,                                                                                                                             0, 0, 0, 0];
 
B_b = [                                                                                                                   0,                                                                                                                    0;     
                  (rw*(mb*L^2 + mb*rw*L + I_b))/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2),            (rw*(mb*L^2 + mb*rw*L + I_b))/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2);
                                                                                                                          0,                                                                                                                    0;
       -(2*I_w + mb*rw^2 + 2*mw*rw^2 + L*mb*rw)/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2), -(2*I_w + mb*rw^2 + 2*mw*rw^2 + L*mb*rw)/(2*I_b*I_w + 2*I_w*L^2*mb + I_b*mb*rw^2 + 2*I_b*mw*rw^2 + 2*L^2*mb*mw*rw^2);
                                                                                                                          0,                                                                                                                    0;
                                                                                (D*rw)/(mw*D^2*rw^2 + I_w*D^2 + 2*I_z*rw^2),                                                                         -(D*rw)/(mw*D^2*rw^2 + I_w*D^2 + 2*I_z*rw^2)];
C_b = eye(6);
D_b = zeros(6, 2);
%% 2.设置拟合次数，计算对应个数的腿长值

linear_times = 500;%拟合次数
% 已知a,b杆长度，单位为米
a = 0.11461;
b = 0.11291;
alpha = 15;

% 角度范围从 0 到 90 度
A_deg = linspace(0, (90 - alpha), linear_times);  % 角度从 0 到 180 度，500个点
A_rad = deg2rad(A_deg);  % 将角度转换为弧度

% 计算角度对应腿长
length = b .* cos(A_rad) + sqrt(a^2 + b^2 .* cos(A_rad).^2 - b^2);

%% 3.代入各个腿长值求对应K矩阵

K_size_row = 2;
K_size_col = 6;

Q = double(diag([150 1 800 1 150 1]));
R = double(diag([10 10]));

K_room = cell(K_size_row,K_size_col);
for i = 1 : 1 : K_size_row
    for j = 1 : 1 : K_size_col
        K_room{i,j} = zeros(1, linear_times);
    end
end

for i = 1 : 1 : linear_times
    A_c = double(subs(A_b, [mec_sub L], [mec_config length(i)]));

    B_c = double(subs(B_b, [mec_sub L], [mec_config length(i)]));

    sys = ss(A_c, B_c, C_b, D_b);

    K = round(lqr(sys, Q, R), 5);

    for j = 1 : 1 : K_size_row
        for k = 1 : 1 : K_size_col
            K_room{j,k}(i) = K(j, k);
        end
    end
end


%% 4.对角A与K矩阵值进行多项式拟合

fitting_order = 4;

K_fit_room = cell(K_size_row,K_size_col);
K_fit_line_room = cell(K_size_row,K_size_col);
for i = 1 : 1 : K_size_row
    for j = 1 : 1 : K_size_col
        K_fit_room{i,j} = zeros(1, fitting_order + 1);
    end
end

for i = 1 : 1 : K_size_row
    for j = 1 : 1 : K_size_col
        my_n = polyfit(A_rad, K_room{i,j}, fitting_order);

        K_fit_room{i,j} = round(my_n,5);

        K_fit_line_room{i,j} = polyval(K_fit_room{i,j}, A_rad);
    end
end

% 将拟合系数存入到excel表格中
filename = 'Will_data.xlsx';
%将拟合系数保存到excel中
writetable(cell2table(K_fit_room), filename, 'Sheet', 1);
disp('数据已存入excel');
%% 5.绘制拟合曲线与实际曲线比较图

color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];

figure

set(gcf, 'Position', [200, 200, 1600, 800]);

subplot(2, 6, 1)

plot(A_rad, K_room{1,1}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,1}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left displacement K(1,1)');
title('Left displacement K(1,1) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 2)
plot(A_rad, K_room{1,2}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,2}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left velocity K(1,2)');
title('Left velocity K(1,2) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 3)
plot(A_rad, K_room{1,3}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,3}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left pitch angle K(1,3)');
title('Left pitch angle K(1,3) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 4)
plot(A_rad, K_room{1,4}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,4}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left pitch rate K(1,4)');
title('Left pitch rate K(1,4) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 5)
plot(A_rad, K_room{1,5}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,5}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left yaw angle K(1,5)');
title('Left yaw angle K(1,5) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 6)
plot(A_rad, K_room{1,6}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{1,6}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Left yaw rate K(1,6)');
title('Left yaw rate K(1,6) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 7)
plot(A_rad, K_room{2,1}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,1}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right displacement K(2,1)');
title('Right displacement K(2,1) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 8)
plot(A_rad, K_room{2,2}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,2}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right velocity K(2,2)');
title('Right velocity K(2,2) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 9)
plot(A_rad, K_room{2,3}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,3}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right pitch angle K(2,3)');
title('Right pitch angle K(2,3) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 10)
plot(A_rad, K_room{2,4}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,4}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right pitch rate K(2,4)');
title('Right pitch rate K(2,4) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 11)
plot(A_rad, K_room{2,5}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,5}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right yaw angle K(2,5)');
title('Right yaw angle K(2,5) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;

subplot(2, 6, 12)
plot(A_rad, K_room{2,6}, 'LineWidth', 2, 'Color', color_org);
hold on;
plot(A_rad, K_fit_line_room{2,6}, 'r--', 'LineWidth', 3, 'Color', color_fit);  % Plot the fitted curve
xlabel('Angle A (rad)');
ylabel('Right yaw rate K(2,6)');
title('Right yaw rate K(2,6) vs Angle A');
legend('Actual curve', 'Fitted curve');
grid on;




