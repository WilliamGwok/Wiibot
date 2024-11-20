%Hello
clc;clear;

syms L11 L12 L1 L2 L3 L4 ...
     theta1 theta2 theta3 theta4 theta5...
     theta1_d1 theta2_d1 theta3_d1 theta4_d1 theta5_d1...
     theta1_d2 theta2_d2 theta3_d2 theta4_d2 theta5_d2...

% 定义方程 (5) 中的等式
eq1 = L2 * sin(theta2 - theta1) - L4 * sin(theta2 + theta4 - theta1) == L3 * sin(theta3 - theta1);
eq2 = L2 * cos(theta2 - theta1) - L4 * cos(theta2 + theta4 - theta1) == L12 + L3 * cos(theta3 - theta1);

% 对 theta4 求偏导数
partial_eq1_theta4 = diff(lhs(eq1), theta4);
partial_eq2_theta4 = diff(lhs(eq2), theta4);

% 显示结果
disp('偏导数 ∂eq1/∂theta4 =');
pretty(partial_eq1_theta4)

disp('偏导数 ∂eq2/∂theta4 =');
pretty(partial_eq2_theta4)

solution = solve([partial_eq1_theta4 == theta2_d1;partial_eq2_theta4 == theta3_d1], [theta2_d1, theta3_d1]);
disp(solution)

a = diff(theta2_d1, theta4);