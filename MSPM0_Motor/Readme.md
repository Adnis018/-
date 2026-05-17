# MSPM0G3507 Dual Motor Driver Module 🚗

专为全国大学生电子设计竞赛（NUEDC）打造的 **MSPM0G3507** 编码电机通用驱动库。采用完全模块化设计，支持硬件 QEI 测速与通用捕获测速混搭，自带死区与越界保护。

## ✨ 特性 (Features)
- **底层解耦**：一套 `Motor_SetSpeed()` API 搞定所有调速与正反转。
- **自动防呆限幅**：代码内置 PWM 绝对值转换与溢出限幅，告别占空比越界导致的跑飞。
- **跨端口容错机制**：彻底解决 SysConfig 在 `Port A` 和 `Port B` 跨区配置 GPIO 时引发的未定义报错。
- **智能脉冲读取**：针对 MSPM0 仅支持单硬件 QEI 的痛点，实现了 `QEI + 定时器组合捕获` 的不对称测速方案。

## 🛠️ SysConfig 移植指南 (Porting Guide)

当在全新的工程中调用此模块时，请在 `.syscfg` 文件中严格按以下名称配置外设：

| 模块类型 | 实例名称 (Name) | 作用 |
| :--- | :--- | :--- |
| **TIMER - PWM** | `PWM_MOTOR` | 两路 PWM 发生器 (开启 Channel 0 & 1) |
| **TIMER - QEI** | `QEI_LEFT` | 左轮硬件正交解码 |
| **TIMER - CAPTURE** | `CAPTURE_RIGHT` | 右轮脉冲捕获 (开启 Multi-capture 两路通道) |
| **GPIO** | `MOTOR_DIR` | 电机正反转控制 |

> **⚠️ 注意**：GPIO 引脚的 `Pin Name` 必须与 `motor.c` 中的宏定义完全一致（如 `PIN_PB13` 等）。如遇端口报错，请查阅 `ti_msp_dl_config.h` 获取真实硬件宏。

## 🚀 极速调用示例 (Quick Start)

```c
#include "ti_msp_dl_config.h"
#include "motor.h"

int main(void) {
    // 1. 系统初始化
    SYSCFG_DL_init();
    
    // 2. 电机模块初始化
    Motor_Init();
    
    // 3. 测试：左轮全速前进，右轮半速后退
    Motor_SetSpeed(800, -400);
    
    while(1) {
        // 在定时器中断中读取脉冲用于 PID 闭环
        // int pulse_L = Encoder_GetLeftPulse();
        // int pulse_R = Encoder_GetRightPulse();
    }
}