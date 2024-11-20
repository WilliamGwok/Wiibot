%Hello
clc;clear;

syms    mw mb rw D I_w I_b I_z L ...
        s s_l s_r s_b h_b phi_b thetab f_l f_r fws_l fws_r fwh_l fwh_r Tw_l Tw_r...
        sd1 sd2 sd2_l sd2_r sd2_b hd2_b phid2_b phid1_b thetad2_b...
        g fwh_sum thetad1_b

Equ_Lin = [sd2*(mb + 2*mw + (2*I_w/rw^2)) + mb*L*thetad2_b == (1/rw) * (Tw_r + Tw_l);

           I_b*thetad2_b == mb*g*L*thetab - (1 + L/rw)*(Tw_r + Tw_l) + 2*L*(mw + I_w/rw^2)*sd2;

           (D*mw + 2*I_z/D + D*I_w/rw^2)*phid2_b == (1/rw)*(Tw_l + Tw_r)];



[sd2_sov, phid2_b_sov, thetad2_b_sov] = solve(Equ_Lin,[sd2 phid2_b thetad2_b]);

Xdot = [sd1 sd2_sov thetad1_b thetad2_b_sov phid1_b phid2_b_sov ];

X = [s sd1 thetab thetad1_b phi_b phid1_b];

U = [Tw_l Tw_r];

A_a = jacobian(Xdot, X);
B_a = jacobian(Xdot, U);
C_a = eye(6);
D_a = zeros(6, 2);

%% 求增益矩阵K(将所有数据代入后才能求)

%代入平衡点
% A_b = subs(A_a, [sd1 phi_b phid1_b thetab thetad1_b Tw_r Tw_l], [0 0 0 0 0 0 0]);
% B_b = subs(B_a, [sd1 phi_b phid1_b thetab thetad1_b Tw_r Tw_l], [0 0 0 0 0 0 0]);
 
sub_var = [   mb       mw       I_w       I_b       I_z       rw       D       L       g      ];
real_var = [  4        2        0.0034    0.0055    0.0023    0.16     0.48    0.4     9.8     ];

A_a = double(subs(A_a, sub_var, real_var))
B_a = double(subs(B_a, sub_var, real_var))


