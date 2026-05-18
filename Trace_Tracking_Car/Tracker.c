#include "tracker.h"
#include "ti_msp_dl_config.h" // 依赖 SysConfig 生成的底层宏定义

void Tracker_Init(void) {
    // 基础引脚初始化通常已由 SysConfig 中 DL_GPIO_init() 完成
    // 若有额外的传感器使能引脚，在此处开启
}

uint8_t Tracker_GetRawData(void) {
    uint8_t sensor_val = 0;
    // 假设使用 GPIOA 的 0-7 号引脚作为输入 (具体请根据 SysConfig 修改宏)
    // 1 表示压到黑线，0 表示白色
    if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_0)) sensor_val |= (1 << 0);
    if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_1)) sensor_val |= (1 << 1);
    // ... 依次读取 8 个引脚 ...
    if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_7)) sensor_val |= (1 << 7);
    
    return sensor_val;
}

float Tracker_GetWeightedError(void) {
    uint8_t raw_data = Tracker_GetRawData();
    
    if (raw_data == 0x00) return 999.0f; // 彻底脱线，返回特征值
    
    // 传感器权重定义 (中心为0，左右对称扩展)
    float weights[8] = {-4.0f, -3.0f, -2.0f, -1.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    float sum_weights = 0.0f;
    int active_sensors = 0;
    
    for (int i = 0; i < 8; i++) {
        if ((raw_data >> i) & 0x01) {
            sum_weights += weights[i];
            active_sensors++;
        }
    }
    
    return (sum_weights / active_sensors); // 返回加权中心误差
}