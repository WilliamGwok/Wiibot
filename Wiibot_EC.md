# Wiibot：平衡机器人设计与控制

## 前言

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/Robot_Design.png" width="710px">
</div>
<br>
<p align="center">本项目是机器人控制课程的课程设计，主题为平衡机器人。该项目与本人之前的 RP_Balance 项目有相似之处，但采用了不同的腿部结构。在项目开发过程中，对机器人性能进行了详细的测试并保留了相关数据。</p>
<p align="center">机器人机械部分和视觉识别部分分别由本人在 RobotPilots 实验室的同事 ZJH 和 ZZ 负责。</p>

---

## 电控硬件清单

本项目中所使用的主要硬件如下表所示：

| 名称                        | 技术参数                                                                 | 功能                                      | 数量 |
|-----------------------------|-------------------------------------------------------------------------|-----------------------------------------|------|
| Robomaster 开发板 C 型       | 输入电压：8-28 V<br>IMU+电子罗盘：1 个<br>CAN 总线接口：CAN1：2 个；CAN2：2 个<br>UART 接口：2 个<br>SPI 接口：1 个 | 提供高运算能力，保证机器人控制频率达到 1000Hz，支持电机通讯、遥控器信号接收以及视觉系统通讯。 | 1    |
| mc6mini 遥控器航模接收机     | 通道数量：6<br>频率范围：2401-2478 MHz<br>控制范围：>800 m              | 发送摇杆值，控制机器人平移、旋转及跳跃。    | 1    |
| MG6012E-i8v3 伺服电机        | 额定电压：48 V<br>空载转速：310 rpm<br>额定扭矩：6 Nm<br>峰值扭矩：16 Nm   | 控制机器人腿长和横滚角。                     | 2    |
| GM6020 电机                 | 额定电压：24 V<br>空载转速：320 rpm<br>额定扭矩：1.2 Nm<br>峰值扭矩：1.4 Nm | 控制机器人驱动轮，执行平移、旋转及平衡功能。   | 2    |
| M600 系列 TB47S 智能飞行电池 | 容量：4500 mAh<br>电压：22.2 V                                       | 提供整车电控系统电源。                     | 1    |
| DC-DC 可调电源模块           | 输入电压：7-32 V<br>输出电压：0-28 V<br>恒流范围：0.2-1.2 A            | 为视觉系统 NUC 提供适配电压与电流。          | 1    |

---

## 文件结构介绍

项目文件的结构及功能如下表：

| 文件名/文件夹名     | 功能                                                                                 | 链接                                                                                      |
|---------------------|------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|
| Code                | 包括 MATLAB 模型建模、控制器设计、增益矩阵拟合、数据绘制，以及实车硬件驱动、通讯协议与整体逻辑控制。 | [CODE](https://github.com/WilliamGwok/Wiibot/tree/main/Code)                            |
| Display_Files       | 包括项目机械、电控和视觉的课程报告及演示文件。                                        | [DISPLAY](https://github.com/WilliamGwok/Wiibot/tree/main/Disply_Files)                 |
| Mec                 | 机器人机械设计图纸。                                                                | [MEC](https://github.com/WilliamGwok/Wiibot/tree/main/Mec/balance)                      |
| Simulation          | Webots 平台的机器人仿真文件。                                                       | [SIM](https://github.com/WilliamGwok/Wiibot/tree/main/Simulation/webots)                |
| Test_Video          | 包括实车测试的图片与视频。                                                          | [VIDEO](https://github.com/WilliamGwok/Wiibot/tree/main/Test_Video)                     |

---

## 实车测试与数据分析

### 位移与偏航角静态响应分析

#### 位移阶跃响应

在静止状态下，输入幅值为 0.4 的位移阶跃信号，测得机器人位移响应及俯仰角变化如 **图 1** 所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img1.png" width="710px">
</div>
<br>
<p align="center">**图 1：** 位移阶跃响应及俯仰角变化</p>

关键性能指标如下：

- 峰值时间 \( T_P \)：  
  \[
  T_P = 1.731 \, \text{s}
  \]

- 超调量 \( \sigma\% \)：  
  \[
  \sigma\% = 17\%
  \]

- 调节时间 \( T_S \)（在 \( 5\% \) 误差范围内）：  
  \[
  T_S = 2.542 \, \text{s}
  \]

在该过程中，最大俯仰角约为 \( 0.219 \, \text{rad} \)（即 12.549°）。

#### 偏航角阶跃响应

在静止状态下，输入幅值为 1.579 rad（约 90°）的偏航角阶跃信号，测得偏航角响应及俯仰角变化如 **图 2** 所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img2.png" width="710px">
</div>
<br>
<p align="center">**图 2：** 偏航角阶跃响应及俯仰角变化</p>

关键性能指标如下：

- 峰值时间 \( T_P \)：  
  \[
  T_P = 0.477 \, \text{s}
  \]

- 超调量 \( \sigma\% \)：  
  \[
  \sigma\% = 1.6\%
  \]

- 调节时间 \( T_S \)（在 \( 5\% \) 误差范围内）：  
  \[
  T_S = 0.385 \, \text{s}
  \]

---

### 腿长与横滚角响应分析

#### 腿长控制

采用双环 PID 控制器实现机器人腿长控制。速度内环和位置外环的响应曲线如 **图 3** 所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img3.png" width="710px">
</div>
<br>
<p align="center">**图 3：** 腿长控制器内外环响应曲线</p>

关键性能指标如下：

- 峰值时间 \( T_P \)：  
  \[
  T_P = 0.29 \, \text{s}
  \]

- 超调量 \( \sigma\% \)：  
  \[
  \sigma\% = 6.07\%
  \]

---

### 离地检测与跳跃动作

在跳跃过程中，通过关节电机输出扭矩实现有效的离地检测。离地与落地过程中测得的关节扭矩如 **图 6** 所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img6.png" width="710px">
</div>
<br>
<p align="center">**图 6：** 离地与落地过程中的关节电机扭矩输出</p>

为实现跳跃，给腿长控制器输入一个大幅值脉冲信号，平均跳跃高度约为 \( 0.1 \, \text{m} \)。实车跳跃过程如 **图 7** 所示，对应控制器的输出曲线如 **图 8** 所示。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img7.jpeg" width="710px">
</div>
<p align="center">**图 7：** 机器人跳跃过程</p>

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img8.png" width="710px">
</div>
<p align="center">**图 8：** 跳跃动作中双环控制器的输出曲线</p>

---
