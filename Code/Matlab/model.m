%Hello
clc;clear;

syms    mw mb rw D I_w I_b I_z L ...
        s s_l s_r s_b h_b phi_b thetab f_l f_r fws_l fws_r fwh_l fwh_r Tw_l Tw_r...
        sd1 sd2 sd2_l sd2_r sd2_b hd2_b phid2_b phid1_b thetad2_b...
        g fwh_sum thetad1_b



%% 驱动轮
%右
fomular_Wl = [mw*sd2_l - f_l + fws_l == 0;
              I_w*(sd2_l / rw) - Tw_l + f_l*rw == 0];%1
%左
fomular_Wr = [mw*sd2_r - f_r + fws_r == 0;
              I_w*(sd2_r / rw) - Tw_r + f_r*rw == 0];%1

%% 机体
fomular_b = [mb*sd2_b - fws_r - fws_l == 0;
             mb*hd2_b - fwh_r - fwh_l + mb*g == 0;
             I_b*phid2_b - (fwh_r + fwh_l)*L*sin(phi_b) + (Tw_r + Tw_l) + (fws_r + fws_l)*L*cos(phi_b) == 0];

%% 旋转
fomular_spin = [I_z*thetad2_b - (fws_l - fws_r)*D/2 == 0];

%% 其它
fomular_other = [s_b - s - L*sin(phi_b) == 0;
                 h_b - L*cos(phi_b) == 0
                 s == (s_l + s_r) / 2];

%% 总方程组
F = [fomular_Wl; ...
     fomular_Wr; ...
     fomular_b; ...
     fomular_spin; ...
     fomular_other];

mid = [fws_r fws_l f_r f_l fwh_r fwh_l];

%% 消去中间变量(F包括所有方程，matlab无法一次性消去全部中间变量)
mid_1 = [f_l fws_l];
[sov_f_l, sov_fws_l] = solve(fomular_Wl, mid_1);

mid_2 = [f_r fws_r];
[sov_f_r, sov_fws_r] = solve(fomular_Wr, mid_2);

mid_wheel = [f_l fws_l f_r fws_r];
mid_wheel_sov = [sov_f_l, sov_fws_l sov_f_r, sov_fws_r];


% 机体方程组消去中间变量
fomular_b_sov = subs(fomular_b, mid_wheel, mid_wheel_sov);

b_1 = [fomular_b_sov(1);
       fomular_b_sov(3)];

mid_sd2_r = 2*sd2 - sd2_l;

b_2 = subs(b_1, sd2_r, mid_sd2_r);
b_2 = expand(b_2);

b_3 = fomular_b_sov(2);

sov_hd2_b = -L*(cos(phi_b)*phid1_b^2 + sin(phi_b)*phid2_b);

b_3 = subs(b_3, hd2_b, sov_hd2_b);

b_4 = [b_2(1);
       b_3;
       b_2(2)];

%手动合并一些同类项
b_4(3) = [Tw_l + Tw_r + I_b*phid2_b - L*sin(phi_b)*fwh_sum - 2*L*mw*sd2*cos(phi_b)...
         + (L*Tw_l*cos(phi_b))/rw + (L*Tw_r*cos(phi_b))/rw - (2*I_w*L*sd2*cos(phi_b))/rw^2 == 0];

sov_fwh_r = fwh_sum - fwh_l;

b_5 = subs(b_4, fwh_r, sov_fwh_r);

sov_fwh_sum = solve(b_5(2), fwh_sum);

b_6 = subs([b_4(1);b_4(3)], fwh_sum, sov_fwh_sum);

sov_sd2_b = sd2 + L*(-sin(phi_b)*phid1_b^2 + cos(phi_b)*phid2_b);

b_6 = subs(b_6, sd2_b, sov_sd2_b);
%旋转方程消去中间变量
mid_3 = [fws_l fws_r];

sov_mid_3 = [sov_fws_l sov_fws_r];

sov_mid_4 = subs(sov_mid_3, sd2_r, mid_sd2_r);


b_7 = subs(fomular_spin, mid_3, sov_mid_4);

b_7 = subs(b_7, sd2, (sd2_r + sd2_l)/2);
% expand(b_7);
mid_sd2_l = (thetad2_b*D) + sd2_r;
b_8 = subs(b_7, sd2_l, mid_sd2_l);
assume(rw ~= 0);
b_8 = simplify(b_8);

%% 得到最后简化非线性方程组

formula_final_nl = [b_6;
                    b_8];

%在平衡点附近进行线性化

formula_final_l = subs(formula_final_nl, [sin(phi_b) cos(phi_b)], [phi_b 1]);

%% 列写状态空间方程

[sd2_sov, phid2_b_sov, thetad2_b_sov] = solve(formula_final_l,[sd2 phid2_b thetad2_b]);

Xdot = [sd1 sd2_sov phid1_b phid2_b_sov thetad1_b thetad2_b_sov];

X = [s sd1 phi_b phid1_b thetab thetad1_b];

U = [Tw_l Tw_r];

A_a = jacobian(Xdot, X);
B_a = jacobian(Xdot, U);
C_a = eye(6);
D_a = zeros(6, 2);

%% 求增益矩阵K(将所有数据代入后才能求)

%代入平衡点
A_b = subs(A_a, [sd1 phi_b phid1_b thetab thetad1_b Tw_r Tw_l], [0 0 0 0 0 0 0]);
B_b = subs(B_a, [sd1 phi_b phid1_b thetab thetad1_b Tw_r Tw_l], [0 0 0 0 0 0 0]);
 
sub_var = [   mb          mw         I_w         I_b         I_z         rw       D       L         g      ];
real_var = [  4.73        0.2        0.000175    0.007034    0.023312    0.044    0.21    0.062     9.81     ];

A_b = double(subs(A_b, sub_var, real_var))
B_b = double(subs(B_b, sub_var, real_var))

Control = ctrb(A_b, B_b);
r = rank(Control);
disp(r);

x1_ = 1 / 0.2^2;
x2_ = 1 / 0.5^2;
x3_ = 1 / 0.5^2;
x4_ = 1 / 10^2;
x5_ = 1 / 0.5^2;
x6_ = 1 / 0.8^2;


Q = double(diag([20*x1_   1*x2_   50*x3_   1*x4_   30*x5_   1*x6_]))
R = double(diag([20 20]));
% Q = double(diag([80 1 30 1 10 1]));
% R = double(diag([1.5 1.5]));
sys = ss(A_b, B_b, C_a, D_a);
K = lqr(sys, Q, R);


K_real = K;
disp('实际K >>')
X1 = sprintf('  %f, %f, %f, %f, %f, %f, %f, %f, %f, %f', K_real(1,1), K_real(1,2), K_real(1,3), K_real(1,4), K_real(1,5), K_real(1,6));
X2 = sprintf('  %f, %f, %f, %f, %f, %f, %f, %f, %f, %f', K_real(2,1), K_real(2,2), K_real(2,3), K_real(2,4), K_real(2,5), K_real(2,6));

disp('.wheell_K =')
disp('{')
disp(X1)
disp('},')
disp('.wheelr_K =')
disp('{')
disp(X2)
disp('},')











