#include "MPU6050.h"
#include "../System/Delay.h"

/**
 * @brief 向 MPU6050 写入一个字节的数据
 */
uint8_t MPU6050_WriteReg(uint8_t regAddr, uint8_t data) {
    uint8_t buff[2] = {regAddr, data};
    
    // 等待 I2C 总线空闲
    while (!(DL_I2C_getControllerStatus(I2C_OLED_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    // 发送寄存器地址和数据
    DL_I2C_startControllerTransfer(I2C_OLED_INST, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    DL_I2C_fillControllerTXFIFO(I2C_OLED_INST, &buff[0], 2);
    
    // 等待发送完成
    while (DL_I2C_getControllerStatus(I2C_OLED_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    return 0;
}

/**
 * @brief 从 MPU6050 读取一个字节的数据
 */
uint8_t MPU6050_ReadReg(uint8_t regAddr) {
    uint8_t data = 0;
    
    // 1. 先告诉 MPU6050 我们要读哪个寄存器 (TX 发送地址)
    DL_I2C_startControllerTransfer(I2C_OLED_INST, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    DL_I2C_fillControllerTXFIFO(I2C_OLED_INST, &regAddr, 1);
    while (DL_I2C_getControllerStatus(I2C_OLED_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

    // 2. 切换方向，从 MPU6050 读取数据 (RX 接收数据)
    DL_I2C_startControllerTransfer(I2C_OLED_INST, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 1);
    while (DL_I2C_getControllerStatus(I2C_OLED_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    
    // 提取数据
    // 提取数据 (此时总线已空闲，数据必定已到达)
    data = DL_I2C_receiveControllerData(I2C_OLED_INST);
    return data;
}

/**
 * @brief 读取 MPU6050 的身份 ID
 * @return 正常应该返回 0x68 (104)
 */
uint8_t MPU6050_Get_ID(void) {
    return MPU6050_ReadReg(MPU_WHO_AM_I_REG);
}

/**
 * @brief 初始化 MPU6050
 * @return 0:成功, 1:失败(找不到器件)
 */
uint8_t MPU6050_Init(void) {
    Delay_ms(100); // 上电延时，等待传感器稳定
    
    // 1. 唤醒传感器：电源管理寄存器1写0
    MPU6050_WriteReg(MPU_PWR_MGMT1_REG, 0x00);
    Delay_ms(10);
    
    // 如果连身份都读不对，说明硬件接线有问题，直接返回错误
    if (MPU6050_Get_ID() != 0x68) {
        return 1; 
    }
    
    // 2. 基础配置 (为以后的 PID 算法打基础)
    MPU6050_WriteReg(MPU_SMPLRT_DIV_REG, 0x07); // 采样率分频，典型值
    MPU6050_WriteReg(MPU_CFG_REG, 0x06);        // 低通滤波，滤除电机高频震动
    MPU6050_WriteReg(MPU_GYRO_CFG_REG, 0x18);   // 陀螺仪量程 ±2000°/s
    MPU6050_WriteReg(MPU_ACCEL_CFG_REG, 0x00);  // 加速度计量程 ±2g
    
    return 0; // 初始化成功
}