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

linear_times = 30;%拟合次数
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






subplot(2, 6, 1)
plot(length, K_room{1,1});
xlabel('腿长L (米)');
ylabel('左位移K(1,1)');
title('左位移K(1,1)关于腿长L的变化');
grid on;

subplot(2, 6, 2)
plot(length, K_room{1,2});
xlabel('腿长L (米)');
ylabel('左速度K(1,2)');
title('左速度K(1,2)关于腿长L的变化');
grid on;

subplot(2, 6, 3)
plot(length, K_room{1,3});
xlabel('腿长L (米)');
ylabel('左俯仰角K(1,3)');
title('左俯仰角K(1,3)关于腿长L的变化');
grid on;

subplot(2, 6, 4)
plot(length, K_room{1,4});
xlabel('腿长L (米)');
ylabel('左俯仰角速度K(1,4)');
title('左俯仰角速度K(1,4)关于腿长L的变化');
grid on;

subplot(2, 6, 5)
plot(length, K_room{1,5});
xlabel('腿长L (米)');
ylabel('左偏航角K(1,5)');
title('左偏航角K(1,5)关于腿长L的变化');
grid on;

subplot(2, 6, 6)
plot(length, K_room{1,6});
xlabel('腿长L (米)');
ylabel('左偏航角速度K(1,6)');
title('左偏航角速度K(1,6)关于腿长L的变化');
grid on;

subplot(2, 6, 7)
plot(length, K_room{2,1});
xlabel('腿长L (米)');
ylabel('右位移K(2,1)');
title('右位移K(2,1)关于腿长L的变化');
grid on;

subplot(2, 6, 8)
plot(length, K_room{2,2});
xlabel('腿长L (米)');
ylabel('右速度K(2,2)');
title('右速度K(2,2)关于腿长L的变化');
grid on;

subplot(2, 6, 9)
plot(length, K_room{2,3});
xlabel('腿长L (米)');
ylabel('右俯仰角K(2,3)');
title('右俯仰角K(2,3)关于腿长L的变化');
grid on;

subplot(2, 6, 10)
plot(length, K_room{2,4});
xlabel('腿长L (米)');
ylabel('右俯仰角速度K(2,4)');
title('右俯仰角速度K(2,4)关于腿长L的变化');
grid on;

subplot(2, 6, 11)
plot(length, K_room{2,5});
xlabel('腿长L (米)');
ylabel('右偏航角K(2,5)');
title('右偏航角K(2,5)关于腿长L的变化');
grid on;

subplot(2, 6, 12)
plot(length, K_room{2,6});
xlabel('腿长L (米)');
ylabel('右偏航角速度K(2,6)');
title('右偏航角速度K(2,6)关于腿长L的变化');
grid on;


figure
plot(A_rad, K_room{1,3});