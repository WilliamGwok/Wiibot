% Load the data from the CSV file
data = readtable('yaw_and_Pitch.csv', 'Delimiter', ';');

% Extract the columns from the table
time = data.Timestamp / 1000000;
yaw_angle_measured = data.phi;
pitch_angle = data.thetab;
yaw_angle_target = data.spin_rad;

% Create the plot
figure;

% Plot yaw angle measurement and target
subplot(3,1,1);
plot(time, yaw_angle_measured, 'b', 'DisplayName', 'Measured Yaw Angle','LineWidth', 2);
hold on;
plot(time, yaw_angle_target, 'r', 'DisplayName', 'Target Yaw Angle','LineWidth', 2);
xlim([16.85,22])
xlabel('Time (s)');
ylabel('Yaw Angle (rad)');
title('Robot Yaw Angle');
legend show;
grid on;

% Plot pitch angle
subplot(3,1,2);
plot(time, pitch_angle, 'g', 'DisplayName', 'Pitch Angle','LineWidth', 2);
xlim([16.85,24])
xlabel('Time (s)');
ylabel('Pitch Angle (rad)');
title('Robot Pitch Angle');
legend show;
grid on;

% Adjust the layout
% sgtitle('Robot Angles Over Time');
