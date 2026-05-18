#include "pid.h"

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float out_max, float int_max) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->current_error = 0.0f;
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->out_max = out_max;
    pid->int_max = int_max;
}

float PID_Calc(PID_TypeDef *pid, float current_error) {
    float p_out, i_out, d_out, total_out;
    
    pid->current_error = current_error;
    
    // P 比例项
    p_out = pid->Kp * pid->current_error;
    
    // I 积分项 (带限幅)
    pid->integral += pid->current_error;
    if (pid->integral > pid->int_max) pid->integral = pid->int_max;
    if (pid->integral < -pid->int_max) pid->integral = -pid->int_max;
    i_out = pid->Ki * pid->integral;
    
    // D 微分项
    d_out = pid->Kd * (pid->current_error - pid->last_error);
    pid->last_error = pid->current_error;
    
    // 总输出计算与限幅
    total_out = p_out + i_out + d_out;
    if (total_out > pid->out_max) total_out = pid->out_max;
    if (total_out < -pid->out_max) total_out = -pid->out_max;
    
    return total_out;
}