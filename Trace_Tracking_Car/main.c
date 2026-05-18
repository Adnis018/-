#include "ti_msp_dl_config.h"
#include "motor.h"
#include "tracker.h"
#include "pid.h"

PID_TypeDef pid_track;
#define BASE_SPEED 400

int main(void) {
    // 1. 硬件外设初始化
    SYSCFG_DL_init();
    Motor_Init();
    Tracker_Init();
    
    // 2. 初始化巡线 PID 
    // 参数: 结构体, Kp, Ki, Kd, 最大转向PWM限幅, 积分限幅
    PID_Init(&pid_track, 40.0f, 0.0f, 15.0f, 400.0f, 100.0f);
    
    // 3. 开启全局中断与系统滴答定时器
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();
    DL_SYSTICK_enable();

    while (1) {
        // 主循环留空，控制逻辑全部在 10ms 中断内极速执行
    }
}

// ---------------------------------------------------------
// 控制心跳：10ms 定时器中断服务函数
// ---------------------------------------------------------
void SysTick_Handler(void) {
    static float last_valid_error = 0.0f;
    
    // 1. 读取误差
    float current_error = Tracker_GetWeightedError();
    
    // 2. 状态机：防脱线保护
    if (current_error == 999.0f) {
        current_error = last_valid_error; // 强行沿用脱线前最后一刻的误差
    } else {
        last_valid_error = current_error;
    }
    
    // 3. PID 计算转向力矩
    float turn_out = PID_Calc(&pid_track, current_error);
    
    // 4. 差速叠加并执行
    int left_speed  = BASE_SPEED + (int)turn_out;
    int right_speed = BASE_SPEED - (int)turn_out;
    Motor_SetSpeed(left_speed, right_speed);
}