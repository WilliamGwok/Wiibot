# Wiibot

## 前言
<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/Robot_Design.png" width="710px">
</div>
<br>
<p align="center">本项目为本人选修机器人控制课程的课程设计。由于期末大作业的主题便是平衡车，故该项目与本人的RP_Balance项目相类似。与其不同的是，该项目平衡机器人选用了不同的腿部构型，在完成项目的过程中也保留了大部分测试数据。</p>
<p align="center">负责机器人机械部分和视觉识别部分的分别是本人在RobotPilots实验室的同事ZJH和ZZ</p>

## 电控硬件清单
| 名称                        | 部分技术参数                                                                 | 功能                                    | 数量 |
|-----------------------------|-----------------------------------------------------------------------------|---------------------------------------|------|
| Robomaster 开发板 C 型       | 输入电压: 8-28 V<br>IMU+E-compass: 1 个<br>CAN 总线接口: CAN1: 2 个; CAN2: 2 个<br>UART 接口: 2 个<br>SPI 接口: 1 个 | 搭载的芯片提供足够高的运算能力，让机器人控制频率保持 1000Hz。电机通讯、遥控器信息接收以及视觉电脑通讯 | 1    |
| 迈克 mc6mini 遥控器航模接收机 | 通道数量: 6<br>频率范围: 2401-2478 MHz<br>控制范围: >800 m                  | 发送摇杆值，发送机器人平移、旋转、跳跃等指令 | 1    |
| 领控 MG6012E-i8v3 伺服电机   | 额定电压: 48 V<br>空载转速: 310 rpm<br>额定扭矩: 6 Nm<br>峰值扭矩: 16 Nm      | 机器人关节电机，控制机器人的腿长以及机体横滚角   | 2    |
| Robomater GM6020 电机        | 额定电压: 24 V<br>空载转速: 320 rpm<br>额定扭矩: 1.2 Nm<br>峰值扭矩: 1.4 Nm  | 机器人驱动轮电机，执行平移、旋转、维持平衡等动作   | 2    |
| 经纬 M600 系列 TB47S 智能飞行电池 | 容量: 4500 mAh<br>电压: 22.2 V                                           | 整套机器人的电控系统的供电                 | 1    |
| DC-DC 可调电源模块            | 输入电压: 7-32 V<br>输出电压: 0-28 V<br>恒流范围: 0.2-1.2 A               | 为视觉NUC提供适配的工作电压及电流            | 1    |

## 文件介绍

| 文件名 | 功能 | 文件链接 |
|---|---|---|
| Code | 分别由MATBLAB和实车代码，前者包括建模求解，控制器设计，增益矩阵K拟合以及数据绘制等，后者包括硬件驱动，通讯协议，模块控制和整体逻辑控制等 | [CODE](https://github.com/WilliamGwok/Wiibot/tree/main/Code) |
| Display_Files | 项目的机械、电控和视觉的课程报告以及演示文件 | [DISPLAY](https://github.com/WilliamGwok/Wiibot/tree/main/Disply_Files) |
| Mec | 机器人机械图纸 | [MEC](https://github.com/WilliamGwok/Wiibot/tree/main/Mec/balance) |
| Simulation | Webots平台的机器人仿真* | [SIM](https://github.com/WilliamGwok/Wiibot/tree/main/Simulation/webots) |
| Test_Video | 机器人实车测试图片和视频 | [VIDEO](https://github.com/WilliamGwok/Wiibot/tree/main/Test_Video) |

*在机械负责同事帮助下，本人尝试将机械提供的STL文件直接导入到Webots平台中进行仿真验证，发现导入模型较为复杂，在进行模型简化后仿真世界仍难以正常运行，鉴于该机器人模型较为简单，本人只在仿真平台验证了机器人维持平衡的功能

## 实车测试
对整车多个控制器的控制效果进行分析，除了腿长控制器和横滚角控制器的测试，所有数据测量均在固定腿长的实车状态下测得，对应的增益矩阵K 如下所示。数据均在J-Scope 中测得，在MATLAB 中绘制。  

$$
K={\left[\begin{array}{l l l l l l}{-3.693}&{-3.287}&{-8.559}&{-0.591}&{3.693}&{0.369; }\\ {-3.693}&{-3.287}&{-8.559}&{-0.591}&{-3.693}&{-0.369}\end{array}\right]}
$$

### 平衡车位移、偏航角静态响应及俯仰角变化  

在静止状态下，给机器人位移期望输入幅值为0.4的阶跃信号，其位移响应及俯仰角变化如图1 所示。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img1.png" width="710px">
</div>
<br>
<p align="center">图1 机器人位移阶跃响应及其过程俯仰角变化</p>

经测量，位移响应的峰值时间 $T_{P}$ 为：  

$$
T_{P}=1.731s
$$

超调量 $\upsigma\%$ 为：  

$$
$\upsigma\%$ = 17%
$$

调节时间 $T_{S}$ 为（ $5\%$ 误差下）：  

$$
T_{S}=2.542s
$$

在位移状态量的轨迹跟踪过程中，测得机器人机体最大俯仰角为0.219 𝑟𝑎𝑑，即约12.549度。  

在静止状态下，给机器人偏航角期望输入幅值为1.579（约90 度）的阶跃信号，其偏航角响应及俯仰角变化如图2 所示。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img2.png" width="710px">
</div>
<br>
<p align="center">图2 机器人偏航角阶跃响应及其过程俯仰角变化</p>


经测量，位移响应的峰值时间 $T_{P}$ 为：  

$$
T_{P}=0.477s
$$

超调量 $\upsigma\%$ 为：  

$$
\upsigma\%=1.6%
$$

调节时间 $T_{S}$ 为（ $5\%$ 误差下）：  

$$
T_{S}=0.385s
$$

在偏航角状态量的轨迹跟踪过程中，测得机器人机体俯仰角在转动过程中会在0.104 𝑟𝑎𝑑内产生小幅度震荡。  

综上，机器人位移响应速度较慢，考虑到测量过程中没有加入机器人速度状态的轨迹跟踪，且响应过程中机体倾角较小，实际响应效果可以满足项目需求。偏航角的响应十分迅速，且超调量小，对机体的俯仰角影响十分有限，也能满足项目需求。  

### 平衡车腿长及机体横滚角响应  

对于机器人腿长的控制，我们采用了双环PID 控制器。对腿长目标值输入幅值为0.3 的阶跃信号。其位置外环及速度内环的响应曲线如图3 所示。  

其中，该PID 控制器的内外环参数如下所示：  

$$
\begin{array}{l}{{K p=100,\qquad K i=0.5,\qquad I n t e r g r a l_{m a x}=10,\qquad O u t_{m a x}=30;}}\\ {{\mathrm{}}}\\ {{\mathrm{}\kappa p=0.4,\qquad K i=0.05,\qquad I n t e r g r a l_{m a x}=5,\qquad O u t_{m a x}=8;}}\end{array}
$$

基于关节电机的扭矩参数，额定扭矩为 $6N\cdot m$ ，峰值扭矩为 $16N\cdot m$ ，设置常态下速度环的最大速出为8，即 $8N\cdot m$ 。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img3.png" width="710px">
</div>
<br>
<p align="center">图3 机器人腿长双环控制器的内外环响应曲线</p>

由图3 可以看到，速度响应在初始阶段有一段锯齿波形，这是由于机械的腿部涉及有一些干涉导致的。再次输入阶跃信号，其幅值为机器人的最高腿长，其响应曲线如图4 所示。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img4.png" width="710px">
</div>
<br>
<p align="center">图4 机器人腿长最大阶跃响应曲线</p>


经测量，腿长响应的峰值时间 $T_{P}$ 为：  

$$
T_{P}=0.29s
$$

超调量 $\upsigma\%$ 为：  

$$
\upsigma\%=6.07%
$$

调节时间 $T_{S}$ 为（ $\langle2\%$ 误差下）：  

关于机器人横滚角的响应，我们通过让机器人完成单侧上坡的动作来体现。其上坡过程中的横滚角变化曲线及左右腿腿长变化曲线如图5 所示。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img5.png" width="710px">
</div>
<br>
<p align="center">图5 机器人单侧上坡时测得的机体横滚角和两腿长度</p>
  

由图5 可以得到，在单侧上坡过程中，通过横滚角PID 控制器控制双边腿长发生改变，来使得机体始终保持横滚角为0。由测得数据可知，该过程中机器人横滚角的变化幅值为0到0.07 rad，该横滚角响应性能满足项目需求。  

### 离地检测与跳跃动作  

在机器人跳跃过程中，机器人整体会完全离开地面，此时其模型与在地面时有差异，这将会导致系统发散。故高效的离地检测与离地处理十分重要。理论上，严谨的离地处理需要通过测得的关节输出扭矩来解算机器人实时受到的支持力，这将需要一定的推导计算。为了项目进度，我们将该流程简化，直接用关节输出扭矩来判断机器人是否离地。图6 为机器人两次离地落地的关节扭矩输出，我们选取了离地阈值为 $0.5\mathrm{Nm}$ ，经多次测试，该方式检测离地较为可靠与高效。关于离地后的处理，我们将驱动轮的输出置零，并将位移和偏航角两个状态变量置零，以使得机器人在落地后保持初始状态。  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img6.png" width="710px">
</div>
<br>
<p align="center">图6 机器人两次离地落地时关节电机输出扭矩曲线</p>
  

对于机器人跳跃动作的设计，我们通过给腿长期望输入一个幅值较大的脉冲信号实现。实车跳跃过程如图7 所示，多次测试测得的平均跳跃高度约为 $0.1\mathrm{m}$ 。受限于机器人较大的机体重量，以目前使用的关节电机输出性能难以实现较高的跳跃高度，其跳跃性能有待提高。图8 为机器人在执行两次跳跃动作时腿长控制器双环的输出曲线  

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img7.jpeg" width="710px">
</div>
<br>
<p align="center">图7 机器人跳跃过程</p>

<div align=center>
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img8.png" width="710px">
</div>
<br>
<p align="center">图8 机器人两次跳跃动作过程内外环控制器的输出曲线</p>
  




