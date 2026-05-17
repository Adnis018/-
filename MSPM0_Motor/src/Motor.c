/**
 * @file    motor.c
 * @brief   双路直流编码电机底层驱动实现
 */

#include "motor.h"
#include <stdlib.h> 

/* ================= 硬件绑定区 (Hardware Abstraction) ================= */
/* 注意：移植到新项目时，请根据 SysConfig 重新核对以下的 Port 和 Pin 宏定义 */

/**
 * @brief 设置左轮物理方向引脚
 */
static void Motor_SetLeftDir(Motor_Dir_t dir) {
    if (dir == MOTOR_FWD) {
        DL_GPIO_setPins(GPIOB, MOTOR_DIR_PIN_PB13_PIN);
        DL_GPIO_clearPins(GPIOB, MOTOR_DIR_PIN_PB6_PIN);
    } else if (dir == MOTOR_REV) {
        DL_GPIO_clearPins(GPIOB, MOTOR_DIR_PIN_PB13_PIN);
        DL_GPIO_setPins(GPIOB, MOTOR_DIR_PIN_PB6_PIN);
    } else {
        DL_GPIO_clearPins(GPIOB, MOTOR_DIR_PIN_PB13_PIN | MOTOR_DIR_PIN_PB6_PIN);
    }
}

/**
 * @brief 设置右轮物理方向引脚 (注意 PA 和 PB 跨区隔离)
 */
static void Motor_SetRightDir(Motor_Dir_t dir) {
    if (dir == MOTOR_FWD) {
        DL_GPIO_setPins(GPIOA, MOTOR_DIR_PIN_PA28_PIN);
        DL_GPIO_clearPins(GPIOB, MOTOR_DIR_PIN_PB20_PIN);
    } else if (dir == MOTOR_REV) {
        DL_GPIO_clearPins(GPIOA, MOTOR_DIR_PIN_PA28_PIN);
        DL_GPIO_setPins(GPIOB, MOTOR_DIR_PIN_PB20_PIN);
    } else {
        DL_GPIO_clearPins(GPIOA, MOTOR_DIR_PIN_PA28_PIN);
        DL_GPIO_clearPins(GPIOB, MOTOR_DIR_PIN_PB20_PIN);
    }
}

/* ================= 核心逻辑区 (Core Logic) ================= */

void Motor_Init(void) {
    // 启动 PWM 调速定时器
    DL_TimerG_startCounter(PWM_MOTOR_INST);
    // 启动左轮纯硬件 QEI 定时器
    DL_Timer_startCounter(QEI_LEFT_INST);
    // 启动右轮输入捕获定时器
    DL_TimerG_startCounter(CAPTURE_RIGHT_INST);
}

void Motor_SetSpeed(int left_speed, int right_speed) {
    // 1. 处理左轮方向与速度限幅
    if (left_speed > 0) {
        Motor_SetLeftDir(MOTOR_FWD);
    } else if (left_speed < 0) {
        Motor_SetLeftDir(MOTOR_REV);
    } else {
        Motor_SetLeftDir(MOTOR_STOP);
    }
    
    int left_pwm = abs(left_speed);
    if(left_pwm > MOTOR_MAX_PWM_VAL) left_pwm = MOTOR_MAX_PWM_VAL;
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, left_pwm, DL_TIMER_CC_0_INDEX);

    // 2. 处理右轮方向与速度限幅
    if (right_speed > 0) {
        Motor_SetRightDir(MOTOR_FWD);
    } else if (right_speed < 0) {
        Motor_SetRightDir(MOTOR_REV);
    } else {
        Motor_SetRightDir(MOTOR_STOP);
    }
    
    int right_pwm = abs(right_speed);
    if(right_pwm > MOTOR_MAX_PWM_VAL) right_pwm = MOTOR_MAX_PWM_VAL;
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, right_pwm, DL_TIMER_CC_1_INDEX);
}

/* ================= 传感器反馈区 (Sensor Feedback) ================= */

int Encoder_GetLeftPulse(void) {
    // 纯硬件 QEI 直接读取
    uint32_t count = DL_Timer_getTimerCount(QEI_LEFT_INST);
    DL_Timer_setTimerCount(QEI_LEFT_INST, 0); 
    return (int)count;
}

int Encoder_GetRightPulse(void) {
    // 捕获模式读取并结合方向状态判断正反转
    uint32_t count = DL_TimerG_getTimerCount(CAPTURE_RIGHT_INST);
    DL_TimerG_setTimerCount(CAPTURE_RIGHT_INST, 0); 
    
    // 如果方向指示引脚显示后退状态，则反转脉冲符号
    if (DL_GPIO_readPins(GPIOA, MOTOR_DIR_PIN_PA28_PIN) == 0) {
        return -(int)count; 
    }
    return (int)count;
}