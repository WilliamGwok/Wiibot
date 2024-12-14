# Wiibot

## 前言
<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/Robot_Design.png" width="710px">
</div>
<br>
<p align="center">本项目是选修机器人控制课程的课程设计，主要研究平衡车的控制与设计。该项目与先前的RP_Balance项目相似，但采用了不同的腿部构型，并保留了更多测试数据以支持研究分析。</p>
<p align="center">机器人机械部分由RobotPilots实验室的同事ZJH负责，视觉识别部分由ZZ负责。</p>

## 电控硬件清单
| 名称                        | 部分技术参数                                                                 | 功能                                    | 数量 |
|-----------------------------|-----------------------------------------------------------------------------|---------------------------------------|------|
| Robomaster 开发板 C 型       | 输入电压: 8-28 V<br>IMU+E-compass: 1 个<br>CAN 总线接口: CAN1: 2 个; CAN2: 2 个<br>UART 接口: 2 个<br>SPI 接口: 1 个 | 搭载高性能芯片，支持机器人控制频率达 1000Hz，同时负责电机通讯、遥控信息接收及视觉模块通信。 | 1    |
| 迈克 mc6mini 遥控器航模接收机 | 通道数量: 6<br>频率范围: 2401-2478 MHz<br>控制范围: >800 m                  | 发送机器人平移、旋转、跳跃等动作指令。 | 1    |
| 领控 MG6012E-i8v3 伺服电机   | 额定电压: 48 V<br>空载转速: 310 rpm<br>额定扭矩: 6 Nm<br>峰值扭矩: 16 Nm      | 控制机器人腿长及机体横滚角。             | 2    |
| Robomater GM6020 电机        | 额定电压: 24 V<br>空载转速: 320 rpm<br>额定扭矩: 1.2 Nm<br>峰值扭矩: 1.4 Nm  | 控制机器人驱动轮，实现平移、旋转及平衡。 | 2    |
| 经纬 M600 系列 TB47S 智能飞行电池 | 容量: 4500 mAh<br>电压: 22.2 V                                           | 提供整套机器人的电控系统供电。         | 1    |
| DC-DC 可调电源模块            | 输入电压: 7-32 V<br>输出电压: 0-28 V<br>恒流范围: 0.2-1.2 A               | 为视觉NUC提供稳定的工作电压与电流。     | 1    |

## 文件介绍

| 文件名 | 功能 | 文件链接 |
|---|---|---|
| Code | 包括MATLAB代码（建模求解、控制器设计、增益矩阵拟合、数据绘制）和实车代码（硬件驱动、通讯协议、模块控制与逻辑控制）。 | [CODE](https://github.com/WilliamGwok/Wiibot/tree/main/Code) |
| Display_Files | 机械、电控、视觉课程报告及演示文件。 | [DISPLAY](https://github.com/WilliamGwok/Wiibot/tree/main/Disply_Files) |
| Mec | 包含机器人机械设计图纸。 | [MEC](https://github.com/WilliamGwok/Wiibot/tree/main/Mec/balance) |
| Simulation | Webots平台的机器人仿真模型*。 | [SIM](https://github.com/WilliamGwok/Wiibot/tree/main/Simulation/webots) |
| Test_Video | 包含实车测试图片及视频文件。 | [VIDEO](https://github.com/WilliamGwok/Wiibot/tree/main/Test_Video) |

*由于导入STL文件到Webots平台较为复杂，简化模型后仿真世界仍难以正常运行。最终仅验证了机器人维持平衡的功能。

## 实车测试

对整车控制器性能进行了测试与分析。除腿长控制器和横滚角控制器的测试外，其他测试均在固定腿长的条件下进行。对应的增益矩阵K如下，所有数据通过J-Scope测量，并使用MATLAB绘制：

$$
K={\left[\begin{array}{l l l l l l}{-3.693}&{-3.287}&{-8.559}&{-0.591}&{3.693}&{0.369; }\\ {-3.693}&{-3.287}&{-8.559}&{-0.591}&{-3.693}&{-0.369}\end{array}\right]}
$$

### 平衡车位移、偏航角静态响应及俯仰角变化

在静止状态下，输入幅值为0.4的阶跃信号，测试机器人位移响应及俯仰角变化，结果如图1所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img1.png" width="710px">
</div>
<br>
<p align="center">图1 机器人位移阶跃响应及其俯仰角变化</p>

经测量，位移响应特性如下：

- 峰值时间 $T_{P}$：1.731s
- 超调量 $\upsigma\%$：17\%
- 调节时间 $T_{S}$（5%误差下）：2.542s


位移响应过程中，机器人最大俯仰角为0.219 rad（约12.549度）。

在静止状态下，输入幅值为1.579（约90度）的偏航角阶跃信号，测试偏航角响应及俯仰角变化，结果如图2所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img2.png" width="710px">
</div>
<br>
<p align="center">图2 机器人偏航角阶跃响应及其俯仰角变化</p>

偏航角响应特性如下：

- 峰值时间 $T_{P}$：0.477s

- 超调量 $\upsigma\%$：1.6\%

- 调节时间 $T_{S}$（5%误差下）：0.385s


偏航角轨迹跟踪过程中，俯仰角在0.104 rad内产生小幅度震荡。

### 平衡车腿长及机体横滚角响应

采用双环PID控制器对腿长进行控制。输入幅值为0.3的阶跃信号，位置外环与速度内环响应如图3所示。

控制器参数如下：

$$
\begin{array}{l}{{K p=100,\qquad K i=0.5,\qquad I n t e r g r a l_{m a x}=10,\qquad O u t_{m a x}=30;}}\\ {{\mathrm{}}}\\ {{\mathrm{}\kappa p=0.4,\qquad K i=0.05,\qquad I n t e r g r a l_{m a x}=5,\qquad O u t_{m a x}=8;}}\end{array}
$$

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img3.png" width="710px">
</div>
<br>
<p align="center">图3 机器人腿长双环控制器响应</p>

再次输入最大腿长阶跃信号，响应如图4所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img4.png" width="710px">
</div>
<br>
<p align="center">图4 机器人腿长最大阶跃响应</p>

经测量，腿长响应特性如下：

- 峰值时间 $T_{P}$：0.29s

- 超调量 $\upsigma\%$：6.07\%

- 调节时间 $T_{S}$（2%误差下）：0.46s


横滚角响应通过单侧上坡动作*体现，其横滚角及左右腿腿长变化如图5所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img5.png" width="710px">
</div>
<br>
<p align="center">图5 单侧上坡时横滚角与腿长变化</p>

*上坡过程持续2s，坡长约0.8m，坡度9 °

### 离地检测与跳跃动作

通过监测关节扭矩检测离地状态。图6展示了两次离地的关节扭矩输出曲线，设置离地阈值为0.5Nm，经测试可靠性较高。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img6.png" width="710px">
</div>
<br>
<p align="center">图6 离地检测扭矩曲线</p>

通过给腿长输入大幅脉冲信号实现跳跃。平均跳跃高度约为0.1m，受限于电机性能，跳跃高度尚有改进空间。跳跃时的控制器输出如图7和图8所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img7.jpeg" width="710px">
</div>
<br>
<p align="center">图7 跳跃过程</p>

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img8.png" width="710px">
</div>
<br>
<p align="center">图8 跳跃动作控制器输出曲线</p>
