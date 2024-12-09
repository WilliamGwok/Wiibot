% Load the data from the CSV file
data = readtable('distance_and_pitch.csv', 'Delimiter', ';');

% Extract the columns from the table
time = data.Timestamp / 1000000;
displacement_measured = data.s + 0.2;
displacement_target = data.distance + 0.2;
pitch_angle = data.thetab;

% Create the plot
figure;

% Plot displacement measurement and target
subplot(3,1,1);
plot(time, displacement_measured, 'b', 'DisplayName', 'Measured Displacement(m)','LineWidth', 2);
hold on;
plot(time, displacement_target, 'r', 'DisplayName', 'Target Displacement','LineWidth', 2);
xlim([13.271,19.631]);
xlabel('Time (s)');
ylabel('Displacement');
title('Robot Displacement');
legend show;
grid on;

% Plot the pitch angle
subplot(3,1,2);
plot(time, pitch_angle, 'g', 'DisplayName', 'Pitch Angle(rad)','LineWidth', 2);
xlim([13.258,19.631]);
xlabel('Time (s)');
ylabel('Pitch Angle');
title('Robot Pitch Angle');
legend show;
grid on;

% Adjust the layout
% sgtitle('Robot Performance Over Time');
