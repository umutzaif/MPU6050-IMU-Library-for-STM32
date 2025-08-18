#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <math.h>

// I2C adresi (7-bit adres sol shift ile HAL uyumlu yapılır)
#define MPU6050_ADDR       (0x68 << 1)

// Register adresleri
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_SMPLRT_DIV   0x19
#define MPU6050_REG_GYRO_CONFIG  0x1B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_TEMP_OUT_H   0x41
#define MPU6050_REG_GYRO_XOUT_H  0x43

typedef struct {
    int16_t acc_raw[3];
    int16_t gyro_raw[3];
    float gyro_cal[3];
    float temperature;
    float angle_pitch, angle_roll;
    float angle_pitch_acc, angle_roll_acc;
    float angle_pitch_gyro, angle_roll_gyro;
} MPU6050_t;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_CalibrateGyro(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu);
void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu);
void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu);
void MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu);
void MPU6050_ComputeAngles(MPU6050_t *mpu);

#endif // MPU6050_H
