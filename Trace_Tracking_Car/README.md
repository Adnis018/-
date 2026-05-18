# MSPM0G3507 PID Line Follower 🏎️

基于 TI MSPM0G3507 微控制器的纯血统 8 路加权巡线小车工程。
采用经典的 **裸机前后台架构** (主循环空转，SysTick 10ms 中断执行核心算法)，确保闭环控制周期的绝对精确。

## ⚙️ Core Architecture (核心架构)
* **感知层 (Tracker)**: 8路红外对管，通过加权平均算法提取出连续线性的偏移误差，自带防脱线记忆功能。
* **决策层 (PID)**: 手写标准位置式 PID 纯数学黑盒，包含完整的积分限幅（抗饱和）与输出限幅。
* **执行层 (Motor)**: 配合 SysConfig 产生的 10kHz 高频 PWM，驱动 TB6612 模块，丝滑静音。

## ⚠️ 硬件踩坑排雷指南 (Blood & Tears)
如果您在烧录此工程时遇到 `Texas Instruments XDS110 USB Debug Probe/CS_DAP_0 Error connecting to the target: DAP Connection Error (-614)`，请立刻检查以下两点（本作者实战血泪史）：
1. **SysConfig 引脚冲突**：绝对、千万不要将 `PA19` (SWCLK) 或 `PA20` (SWDIO) 配置为普通 GPIO。单片机一旦执行锁死引脚的代码，将直接变成“植物人”。(补救措施：拔电 -> 按住 BSL 键插 USB -> CCS 强行 Mass Erase)。
2. **“薛定谔的跳线帽”**：XDS110 仿真器与下半板之间的 **SWDIO、SWCLK、NRST、3V3、GND** 5 根跳线帽极易发生金属疲劳导致虚接。如果你确信代码无误且单片机没烧，**拔掉跳线帽，直接换上杜邦线大力出奇迹！**

## 🚀 Tuning (调参笔记)
* 纯高速巡线建议采用 **PD 控制**，关闭 I（I 会拖慢过弯响应）。
* **先调 P**：增大 P 直到小车勉强能跟线，但左右剧烈画龙。
* **后调 D**：增大 D（阻尼），预测未来误差，瞬间抹平 P 带来的震荡。