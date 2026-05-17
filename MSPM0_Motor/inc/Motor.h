/**
 * @file    motor.h
 * @brief   双路直流编码电机底层驱动模块 (基于 MSPM0G3507)
 * @note    适用于 TB6612 / L298N 等标准方向+PWM控制的驱动板
 * @author  Your Name / Team
 * @date    202X-XX-XX
 */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "ti_msp_dl_config.h"

/* ================= 用户配置区 (User Configuration) ================= */
// 定义 PWM 定时器的重装载值 (依据 SysConfig 中的 Period Count 配置)
#define MOTOR_MAX_PWM_VAL    800  

/* ================= 数据结构声明 (Data Structures) ================= */
/**
 * @brief 电机运行方向枚举
 */
typedef enum {
    MOTOR_FWD = 0,  // 前进
    MOTOR_REV,      // 后退
    MOTOR_STOP      // 停止/刹车
} Motor_Dir_t;

/* ================= 外部接口函数 (Public APIs) ================= */

/**
 * @brief  初始化电机底层硬件 (PWM, QEI, Capture)
 * @note   必须在 SYSCFG_DL_init() 之后调用
 */
void Motor_Init(void);

/**
 * @brief  设置双轮速度
 * @param  left_speed  左轮速度 (范围: -MOTOR_MAX_PWM_VAL 到 +MOTOR_MAX_PWM_VAL)
 * @param  right_speed 右轮速度 (范围: -MOTOR_MAX_PWM_VAL 到 +MOTOR_MAX_PWM_VAL)
 * @note   正数代表前进，负数代表后退
 */
void Motor_SetSpeed(int left_speed, int right_speed);

/**
 * @brief  获取左轮编码器脉冲数并清零
 * @retval 当前周期内的脉冲增量
 */
int Encoder_GetLeftPulse(void);

/**
 * @brief  获取右轮编码器脉冲数并清零
 * @retval 当前周期内的脉冲增量
 */
int Encoder_GetRightPulse(void);

#endif /* __MOTOR_H__ */