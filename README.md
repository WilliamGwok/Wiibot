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



