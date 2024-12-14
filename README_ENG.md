# Wiibot

## Preface
<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/Robot_Design.png" width="710px">
</div>
<br>
<p align="center">This project is part of the coursework for the Robotics Control course, focusing on the control and design of a balance robot. It is similar to the previous RP_Balance project but adopts a different leg configuration and retains more test data to support research and analysis.</p>
<p align="center">The mechanical components of the robot were designed by ZJH from the RobotPilots lab, while the visual recognition module was handled by ZZ.</p>

## Electronic Hardware List
| Name                        | Technical Specifications                                                       | Function                                     | Quantity |
|-----------------------------|--------------------------------------------------------------------------------|---------------------------------------------|----------|
| Robomaster Development Board Type C | Input Voltage: 8-28 V<br>IMU+E-compass: 1<br>CAN Bus: CAN1: 2 ports; CAN2: 2 ports<br>UART Ports: 2<br>SPI Port: 1 | Supports high-performance chip with control frequency up to 1000Hz; handles motor communication, remote control signal reception, and vision module communication. | 1        |
| Mike MC6mini Remote Control Receiver | Channels: 6<br>Frequency Range: 2401-2478 MHz<br>Control Range: >800 m   | Sends commands for robot translation, rotation, and jumping actions. | 1        |
| Lingkong MG6012E-i8v3 Servo Motor   | Rated Voltage: 48 V<br>No-load Speed: 310 rpm<br>Rated Torque: 6 Nm<br>Peak Torque: 16 Nm | Controls leg length and robot body roll angle.             | 2        |
| Robomater GM6020 Motor        | Rated Voltage: 24 V<br>No-load Speed: 320 rpm<br>Rated Torque: 1.2 Nm<br>Peak Torque: 1.4 Nm | Drives the robot wheels for translation, rotation, and balance. | 2        |
| DJI M600 TB47S Intelligent Flight Battery | Capacity: 4500 mAh<br>Voltage: 22.2 V                                       | Provides power for the entire robot control system.         | 1        |
| DC-DC Adjustable Power Module  | Input Voltage: 7-32 V<br>Output Voltage: 0-28 V<br>Constant Current Range: 0.2-1.2 A | Supplies stable voltage and current to the vision NUC.     | 1        |

## File Overview

| Filename       | Function                                                         | File Link                                                                                      |
|----------------|------------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| Code           | Includes MATLAB code (modeling, controller design, gain fitting, data plotting) and real robot code (hardware drivers, communication protocols, module control, and logic control). | [CODE](https://github.com/WilliamGwok/Wiibot/tree/main/Code)                                 |
| Display_Files  | Reports and presentation files on mechanics, electronics, and vision. | [DISPLAY](https://github.com/WilliamGwok/Wiibot/tree/main/Disply_Files)                      |
| Mec            | Contains mechanical design drawings of the robot.                | [MEC](https://github.com/WilliamGwok/Wiibot/tree/main/Mec/balance)                           |
| Simulation     | Robot simulation models for the Webots platform.*                | [SIM](https://github.com/WilliamGwok/Wiibot/tree/main/Simulation/webots)                     |
| Test_Video     | Includes real robot test images and videos.                      | [VIDEO](https://github.com/WilliamGwok/Wiibot/tree/main/Test_Video)                          |

*Due to the complexity of importing STL files into the Webots platform, the simplified model still struggled to function properly in the simulation environment. Only balance maintenance was verified.

## Real Robot Tests

The performance of the overall controller was tested and analyzed. Except for the leg length controller and roll angle controller tests, other tests were conducted under fixed leg length conditions. The corresponding gain matrix K is shown below. All data was measured with J-Scope and plotted using MATLAB:

$$
K={\left[\begin{array}{l l l l l l}{-3.693}&{-3.287}&{-8.559}&{-0.591}&{3.693}&{0.369; }\\ {-3.693}&{-3.287}&{-8.559}&{-0.591}&{-3.693}&{-0.369}\end{array}\right]}
$$

### Balance Robot Displacement, Yaw Angle Static Response, and Pitch Angle Variation

In a stationary state, an input step signal with an amplitude of 0.4 was applied to test the robot's displacement response and pitch angle variation. The results are shown in Figure 1.

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img1.png" width="710px">
</div>
<br>
<p align="center">Figure 1 Displacement Step Response and Pitch Angle Variation</p>

The measured displacement response characteristics are as follows:

- Peak Time $T_{P}$: 1.731s
- Overshoot $\upsigma\%$: 17%
- Settling Time $T_{S}$ (within 5% error): 2.542s

During the displacement response, the maximum pitch angle was 0.219 rad (approximately 12.549 degrees).

Under stationary conditions, an input yaw step signal with an amplitude of 1.579 (approximately 90 degrees) was applied to test the yaw response and pitch angle variation. The results are shown in Figure 2.

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img2.png" width="710px">
</div>
<br>
<p align="center">Figure 2 Yaw Step Response and Pitch Angle Variation</p>

Yaw response characteristics are as follows:

- Peak Time $T_{P}$: 0.477s

- Overshoot $\upsigma\%$: 1.6%

- Settling Time $T_{S}$ (within 5% error): 0.385s

During yaw trajectory tracking, the pitch angle oscillated slightly within 0.104 rad.

### Balance Robot Leg Length and Body Roll Angle Response

A dual-loop PID controller was used for leg length control. When a step signal with an amplitude of 0.3 was applied, the outer position loop and inner velocity loop responses are shown in Figure 3.

Controller parameters:

$$
\begin{array}{l}{{K p=100,\qquad K i=0.5,\qquad I n t e r g r a l_{m a x}=10,\qquad O u t_{m a x}=30;}}\\ {{\mathrm{}}}\\ {{\mathrm{}\kappa p=0.4,\qquad K i=0.05,\qquad I n t e r g r a l_{m a x}=5,\qquad O u t_{m a x}=8;}}\end{array}
$$

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img3.png" width="710px">
</div>
<br>
<p align="center">Figure 3 Leg Length Dual-Loop Controller Response</p>

When the maximum leg length step signal was applied, the response is shown in Figure 4.

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img4.png" width="710px">
</div>
<br>
<p align="center">Figure 4 Maximum Leg Length Step Response</p>

The measured leg length response characteristics are as follows:

- Peak Time $T_{P}$: 0.29s

- Overshoot $\upsigma\%$: 6.07%

- Settling Time $T_{S}$ (within 2% error): 0.46s

The roll angle response was demonstrated through a single-side uphill motion*, with the roll angle and left/right leg length variations shown in Figure 5.

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img5.png" width="710px">
</div>
<br>
<p align="center">Figure 5 Roll Angle and Leg Length Variation During Single-Side Uphill</p>

*The uphill process lasted 2 seconds, with a slope length of approximately 0.8m and a slope angle of 9°.

### Ground Detection and Jumping Motion

Ground detection was achieved by monitoring joint torque. Figure 6 shows the torque curves for two ground detections. The threshold was set at 0.5 Nm, with high reliability observed during tests.

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img6.png" width="710px">
</div>
<br>
<p align="center">Figure 6 Torque Curves for Ground Detection</p>

Jumping was implemented by inputting a large pulse signal to the leg length. The average jump height was approximately 0.1m. Limited by motor performance, the jump height has room for improvement. The controller outputs during jumping are shown in Figures 7 and 8.

<
