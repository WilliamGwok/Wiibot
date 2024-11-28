

function [length] = Func_Cal_K(xc_r, yc_r, xp_r, yp_r, Ip_r, xc_l, yc_l, xp_l, yp_l, Ip_l, A_Cal, B_Cal, C_Cal, D_Cal)


% 已知a,b杆长度，单位为米
a = 0.11461;
b = 0.11291;

% 角度范围从 0 到 90 度
A_deg = linspace(0, 90, 500);  % 角度从 0 到 180 度，500个点
A_rad = deg2rad(A_deg);  % 将角度转换为弧度

% 计算表达式
expression = b .* cos(A_rad) + sqrt(a^2 + b^2 .* cos(A_rad).^2 - b^2);

% 绘制图像
figure;
plot(A_deg, expression);
xlabel('角度 A (度)');
ylabel('腿长');
title('腿长关于 A 的变化');
grid on;





