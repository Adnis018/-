#ifndef __MPU6050_H
#define __MPU6050_H

#include "ti_msp_dl_config.h"

// MPU6050 I2C 从机地址 (AD0引脚接地)
#define MPU6050_ADDR         0x68

// MPU6050 核心寄存器地址
#define MPU_SMPLRT_DIV_REG   0x19    // 采样频率分频器
#define MPU_CFG_REG          0x1A    // 配置寄存器(用来设置低通滤波)
#define MPU_GYRO_CFG_REG     0x1B    // 陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG    0x1C    // 加速度计配置寄存器
#define MPU_ACCEL_XOUTH_REG  0x3B    // 加速度值X轴高8位
#define MPU_PWR_MGMT1_REG    0x6B    // 电源管理寄存器1
#define MPU_WHO_AM_I_REG     0x75    // 器件ID寄存器

// 函数声明
uint8_t MPU6050_WriteReg(uint8_t regAddr, uint8_t data);
uint8_t MPU6050_ReadReg(uint8_t regAddr);
uint8_t MPU6050_Init(void);
uint8_t MPU6050_Get_ID(void);

#endif