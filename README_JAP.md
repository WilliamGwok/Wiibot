# Wiibot

## 前書き
<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/Robot_Design.png" width="710px">
</div>
<br>
<p align="center">このプロジェクトは、ロボット制御コースの課題設計の一環であり、バランスロボットの設計と制御を研究することを目的としています。本プロジェクトは以前のRP_Balanceプロジェクトに似ていますが、異なる脚構成を採用し、研究と分析をサポートするためにテストデータを保持しています。</p>
<p align="center">ロボットの機械部分はRobotPilots研究所のZJHが担当し、視覚認識部分はZZが担当しました。</p>

## 電子制御ハードウェア一覧
| 名称                           | 技術仕様                                                                                      | 機能                                         | 数量   |
|--------------------------------|-----------------------------------------------------------------------------------------------|--------------------------------------------|--------|
| Robomaster 開発ボード C型        | 入力電圧: 8-28 V<br>IMU+Eコンパス: 1個<br>CANバス: CAN1: 2ポート; CAN2: 2ポート<br>UARTポート: 2個<br>SPIポート: 1個 | 高性能チップを搭載し、制御周波数は最大1000Hz。モーター通信、リモート信号受信、視覚モジュール通信を担当。 | 1      |
| Mike MC6mini リモートコントローラー受信機 | チャンネル数: 6<br>周波数範囲: 2401-2478 MHz<br>制御範囲: >800 m                          | ロボットの移動、回転、ジャンプなどの動作指令を送信。 | 1      |
| Lingkong MG6012E-i8v3 サーボモーター   | 定格電圧: 48 V<br>無負荷回転速度: 310 rpm<br>定格トルク: 6 Nm<br>ピークトルク: 16 Nm         | 脚の長さとロボット本体のロール角を制御。        | 2      |
| Robomaster GM6020 モーター       | 定格電圧: 24 V<br>無負荷回転速度: 320 rpm<br>定格トルク: 1.2 Nm<br>ピークトルク: 1.4 Nm       | ロボットの車輪を駆動し、移動、回転、バランスを実現。 | 2      |
| DJI M600 TB47S インテリジェントバッテリー | 容量: 4500 mAh<br>電圧: 22.2 V                                                            | ロボットの全体の電力供給を提供。                | 1      |
| DC-DC 調整可能電源モジュール       | 入力電圧: 7-32 V<br>出力電圧: 0-28 V<br>定電流範囲: 0.2-1.2 A                            | 視覚認識用NUCに安定した電圧と電流を供給。        | 1      |

## ファイル概要

| ファイル名       | 機能                                                                 | ファイルリンク                                                                                 |
|------------------|----------------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| Code             | MATLABコード（モデリング、制御器設計、ゲインフィッティング、データプロット）および実機コード（ハードウェアドライバ、通信プロトコル、モジュール制御、ロジック制御）。 | [CODE](https://github.com/WilliamGwok/Wiibot/tree/main/Code)                                 |
| Display_Files    | 機械、電気制御、視覚認識に関するレポートとプレゼンテーションファイル。                                            | [DISPLAY](https://github.com/WilliamGwok/Wiibot/tree/main/Disply_Files)                      |
| Mec              | ロボットの機械設計図面を含む。                                                             | [MEC](https://github.com/WilliamGwok/Wiibot/tree/main/Mec/balance)                           |
| Simulation       | Webotsプラットフォーム用のロボットシミュレーションモデル*。                                      | [SIM](https://github.com/WilliamGwok/Wiibot/tree/main/Simulation/webots)                     |
| Test_Video       | 実機テスト画像とビデオを含む。                                                               | [VIDEO](https://github.com/WilliamGwok/Wiibot/tree/main/Test_Video)                          |

*STLファイルをWebotsプラットフォームにインポートするのは複雑で、簡略化されたモデルでもシミュレーションが正常に動作しない場合がありました。最終的にバランス維持の機能のみが検証されました。

## 実機テスト

全体の制御器の性能がテストおよび分析されました。脚の長さ制御器とロール角制御器のテストを除き、その他のテストは固定脚の長さの条件下で実施されました。対応するゲイン行列Kは以下の通りです。全てのデータはJ-Scopeで測定され、MATLABでプロットされています：

$$
K={\left[\begin{array}{l l l l l l}{-3.693}&{-3.287}&{-8.559}&{-0.591}&{3.693}&{0.369; }\\ {-3.693}&{-3.287}&{-8.559}&{-0.591}&{-3.693}&{-0.369}\end{array}\right]}
$$

### バランスロボットの変位、ヨー角の静的応答、およびピッチ角の変化

静止状態で、振幅0.4のステップ信号を入力し、ロボットの変位応答とピッチ角の変化をテストしました。結果は図1の通りです。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img1.png" width="710px">
</div>
<br>
<p align="center">図1 変位ステップ応答およびピッチ角の変化</p>

測定された変位応答の特性は以下の通りです：

- ピーク時間 $T_{P}$：1.731s
- オーバーシュート $\upsigma\%$：17%
- 調整時間 $T_{S}$（誤差5%以内）：2.542s

変位応答の過程で、最大ピッチ角は0.219rad（約12.549度）でした。

静止状態で、振幅1.579（約90度）のヨーステップ信号を入力し、ヨー応答とピッチ角の変化をテストしました。結果は図2の通りです。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img2.png" width="710px">
</div>
<br>
<p align="center">図2 ヨーステップ応答およびピッチ角の変化</p>

ヨー応答の特性は以下の通りです：

- ピーク時間 $T_{P}$：0.477s

- オーバーシュート $\upsigma\%$：1.6%

- 調整時間 $T_{S}$（誤差5%以内）：0.385s

ヨー角の軌道追従中、ピッチ角は0.104rad以内で小さな振動を発生しました。

### 脚長およびロボット本体のロール角応答

脚長制御にはデュアルループPID制御器を使用しました。振幅0.3のステップ信号を入力した場合、外部位置ループと内部速度ループの応答は図3のようになりました。

制御器のパラメータ：

$$
\begin{array}{l}{{K p=100,\qquad K i=0.5,\qquad I n t e r g r a l_{m a x}=10,\qquad O u t_{m a x}=30;}}\\ {{\mathrm{}}}\\ {{\mathrm{}\kappa p=0.4,\qquad K i=0.05,\qquad I n t e r g r a l_{m a x}=5,\qquad O u t_{m a x}=8;}}\end{array}
$$

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img3.png" width="710px">
</div>
<br>
<p align="center">図3 脚長デュアルループ制御器の応答</p>

最大脚長ステップ信号を入力した場合、応答は図4のようになりました。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img4.png" width="710px">
</div>
<br>
<p align="center">図4 最大脚長ステップ応答</p>

測定された脚長応答の特性は以下の通りです：

- ピーク時間 $T_{P}$：0.29s

- オーバーシュート $\upsigma\%$：6.07%

- 調整時間 $T_{S}$（誤差2%以内）：0.46s

ロール角応答は片側の坂を上る動作*で示され、そのロール角と左右の脚長の変化は図5の通りです。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img5.png" width="710px">
</div>
<br>
<p align="center">図5 坂を上る際のロール角と脚長の変化</p>

*坂を上るプロセスは2秒間続き、坂の長さは約0.8m、傾斜角は9°。

### 地面検知とジャンプ動作

関節トルクを監視することで地面検知を行いました。図6は2回の地面検知における関節トルクの出力曲線を示しています。地面検知の閾値は0.5Nmに設定され、テスト結果は高い信頼性を示しました。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img6.png" width="710px">
</div>
<br>
<p align="center">図6 地面検知におけるトルク曲線</p>

脚長に大幅なパルス信号を入力することでジャンプを実現しました。平均ジャンプ高さは約0.1mでした。モーター性能に制限されているため、ジャンプ高さには改良の余地があります。ジャンプ中の制御器出力は図7と図8の通りです。

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img7.jpeg" width="710px">
</div>
<br>
<p align="center">図7 ジャンププロセス</p>

<div align="center">
<img src="https://github.com/WilliamGwok/Wiibot/blob/main/Disply_Files/Figure/img8.png" width="710px">
</div>
<br>
<p align="center">図8 ジャンプ動作の制御器出力曲線</p>
