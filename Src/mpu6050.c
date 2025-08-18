#include "mpu6050.h"

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data;

    // Uyandırma
    data = 0x00;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    // Gyro config ±500 dps
    data = 0x08;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    // Acc config ±8g
    data = 0x10;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

void MPU6050_CalibrateGyro(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu)
{
    uint8_t buf[6];
    int32_t gyro_sum[3] = {0};

    for (int i = 0; i < 2000; i++) {
        HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_REG_GYRO_XOUT_H, 1, buf, 6, HAL_MAX_DELAY);

        int16_t gx = (buf[0] << 8) | buf[1];
        int16_t gy = (buf[2] << 8) | buf[3];
        int16_t gz = (buf[4] << 8) | buf[5];

        gyro_sum[0] += gx;
        gyro_sum[1] += gy;
        gyro_sum[2] += gz;

        HAL_Delay(3);
    }

    mpu->gyro_cal[0] = gyro_sum[0] / 2000.0f;
    mpu->gyro_cal[1] = gyro_sum[1] / 2000.0f;
    mpu->gyro_cal[2] = gyro_sum[2] / 2000.0f;
}

void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu)
{
    uint8_t buf[6];
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, 1, buf, 6, HAL_MAX_DELAY);

    mpu->acc_raw[0] = (buf[0] << 8) | buf[1];
    mpu->acc_raw[1] = (buf[2] << 8) | buf[3];
    mpu->acc_raw[2] = (buf[4] << 8) | buf[5];
}

void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu)
{
    uint8_t buf[6];
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_REG_GYRO_XOUT_H, 1, buf, 6, HAL_MAX_DELAY);

    mpu->gyro_raw[0] = ((buf[0] << 8) | buf[1]) - mpu->gyro_cal[0];
    mpu->gyro_raw[1] = ((buf[2] << 8) | buf[3]) - mpu->gyro_cal[1];
    mpu->gyro_raw[2] = ((buf[4] << 8) | buf[5]) - mpu->gyro_cal[2];
}

void MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_t *mpu)
{
    uint8_t buf[2];
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_REG_TEMP_OUT_H, 1, buf, 2, HAL_MAX_DELAY);

    int16_t raw_temp = (buf[0] << 8) | buf[1];
    mpu->temperature = (raw_temp / 340.0f) + 36.53f;
}

void MPU6050_ComputeAngles(MPU6050_t *mpu)
{
    int32_t acc_total_vector = sqrt(
        (mpu->acc_raw[0] * mpu->acc_raw[0]) +
        (mpu->acc_raw[1] * mpu->acc_raw[1]) +
        (mpu->acc_raw[2] * mpu->acc_raw[2])
    );

    mpu->angle_pitch_acc = asin((float)mpu->acc_raw[1] / acc_total_vector) * 57.296f;
    mpu->angle_roll_acc  = asin((float)mpu->acc_raw[0] / acc_total_vector) * -57.296f;

    // Basit complementary filter (gyro+acc)
    mpu->angle_pitch = mpu->angle_pitch_gyro * 0.9996f + mpu->angle_pitch_acc * 0.0004f;
    mpu->angle_roll  = mpu->angle_roll_gyro * 0.9996f + mpu->angle_roll_acc * 0.0004f;
}
