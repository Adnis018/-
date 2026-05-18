#ifndef __PID_H__
#define __PID_H__

typedef struct {
    float Kp;            // 比例系数
    float Ki;            // 积分系数
    float Kd;            // 微分系数
    
    float current_error; // 当前误差
    float last_error;    // 上次误差
    float integral;      // 误差积分累计值
    
    float out_max;       // 输出限幅
    float int_max;       // 积分限幅 (抗积分饱和)
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float out_max, float int_max);
float PID_Calc(PID_TypeDef *pid, float current_error);

#endif /* __PID_H__ */