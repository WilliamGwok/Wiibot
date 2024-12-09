% Load the data
data = readtable('length_speed_2.csv', 'Delimiter', ';');
color_org = [0.44, 0.62, 0.98];
color_fit = [0.68, 0.42, 0.89];
% Extract the variables
time = data.Timestamp / 1000000;
target_leg_length = data.target;
measured_leg_length = data.measure;
target_leg_speed = data.target_1;
measured_leg_speed = data.measure_1;

% Create the plot
figure;

% Plot leg length (target and measured)
yyaxis left;
plot(time, target_leg_length, '-b', 'DisplayName', 'Target Leg Length', 'LineWidth', 2,'Color', color_org);
hold on;
plot(time, measured_leg_length, '-r', 'DisplayName', 'Measured Leg Length', 'LineWidth', 2,'Color', color_fit);
ylabel('Leg Length');

% Plot leg speed (target and measured)
yyaxis right;
plot(time, target_leg_speed, '-g', 'DisplayName', 'Target Leg Speed', 'LineWidth', 2);
hold on;
plot(time, measured_leg_speed, '-k', 'DisplayName', 'Measured Leg Speed', 'LineWidth', 2);
ylabel('Leg Speed');

% Add labels and title
xlabel('Time (s)');
title('Leg Length and Speed (Target vs Measured)');
legend('show');
grid on;

xlim([7.417 13])
