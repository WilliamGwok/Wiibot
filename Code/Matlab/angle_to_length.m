
% 得到求解c的公式，多解情况选正解
% syms a b c A
% 
% f = [0 == a^2 - b^2 - c^2 + 2*b*c*cos(A)];
% 
% as = solve(f, c);
% 
% double(subs(as, [a b A], [0.11461 0.11291 1.3]))

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





