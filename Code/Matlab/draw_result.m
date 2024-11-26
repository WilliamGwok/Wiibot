clear;
close all;

alphal = linspace(0, 60, 600);  % α角度范围
Alphal = alphal * pi / 180;    % 转换为弧度
Alphal = Alphal(:);            % 转为列向量
BAD = 30 * pi / 180 + Alphal;  % 计算BAD角

% 参数定义
l1 = 114.11;
l2 = 112.9063;
l3 = 100;
l23 = 36.2318;
l4 = 66.4995;

% 计算LBD、ADC、Beta
LBD = sqrt(l2^2 + l4^2 - 2 * l2 * l4 * cos(BAD));
ADC = acos((LBD.^2 + l2^2 - l4^2) ./ (2 * LBD * l2)) + ...
      acos((LBD.^2 + l23^2 - l3^2) ./ (2 * LBD * l23));
Beta = pi - ADC - Alphal;

% 计算Ex和Ey
Ex = l2 * cos(Alphal) - l1 * cos(Beta);
Ey = -l2 * sin(Alphal) - l1 * sin(Beta);

% 符号部分
syms Alphal_sym
BAD_sym = 30 * pi / 180 + Alphal_sym;

% 用已知参数计算符号表达式
LBD_sym = sqrt(l2^2 + l4^2 - 2 * l2 * l4 * cos(BAD_sym));
ADC_sym = acos((LBD_sym^2 + l2^2 - l4^2) / (2 * LBD_sym * l2)) + ...
          acos((LBD_sym^2 + l23^2 - l3^2) / (2 * LBD_sym * l23));
Beta_sym = pi - ADC_sym - Alphal_sym;

% 符号表达式
Ex_sym = l2 * cos(Alphal_sym) - l1 * cos(Beta_sym);
Ey_sym = -l2 * sin(Alphal_sym) - l1 * sin(Beta_sym);

% 简化表达式
Ex_sym = simplify(Ex_sym, 'Steps', 100);
Ey_sym = simplify(Ey_sym, 'Steps', 100);

% 输出代入参数后的解析表达式
disp('Ex 关于 Alphal 的解析表达式:');
disp(Ex_sym);
disp('Ey 关于 Alphal 的解析表达式:');
disp(Ey_sym);

% 转换为数值函数
Ex_func = matlabFunction(Ex_sym, 'Vars', Alphal_sym);
Ey_func = matlabFunction(Ey_sym, 'Vars', Alphal_sym);

% 使用符号表达式计算并绘图
Ex_sym_calc = Ex_func(Alphal);
Ey_sym_calc = Ey_func(Alphal);

% 绘制图像
figure;
subplot(2,1,1);
plot(alphal, Ex, 'b', 'DisplayName', '原始计算');
hold on;
plot(alphal, Ex_sym_calc, 'r--', 'DisplayName', '符号表达式');
xlabel('α角度（度）');
ylabel('X坐标');
title('优化后的末端位移 Ex 与 α角度的关系');
legend;

subplot(2,1,2);
plot(alphal, Ey, 'b', 'DisplayName', '原始计算');
hold on;
plot(alphal, Ey_sym_calc, 'r--', 'DisplayName', '符号表达式');
xlabel('α角度（度）');
ylabel('Y坐标');
title('优化后的末端位移 Ey 与 α角度的关系');
legend;
