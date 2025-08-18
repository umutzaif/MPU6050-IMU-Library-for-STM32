#ifndef MPU6050_H

#define MPU6050_H

#include "main.h"

#define MPU6050_ADDR 		  0x68<<1
#define PWM_MGMT_1_REG 		0x6B
#define SMPLRT_DIV_REG 		0x19
#define GYRO_CNFG_REG 		0x1B
#define ACC_CNFG_REG 		  0x1C

typedef enum{
  ACC_X,
  ACC_Y,
  ACC_Z,
  GYRO_X,
  GYRO_Y,
  GYRO_Z,
  TEMP,
}MPU6050GetDataType;

typedef struct{
  uint8_t data_1 = 0x00;
  uint8_t data_2 = 0x08;
  uint8_t data_3 = 0x10;
}MPU6050_Init_Variables;

typedef struct{
  uint8_t cuffer[6];
  float prevtime2, time2, elapsedtime2;
  int16_t gyro_raw[3];
  uint8_t buffer[2],tuffer[6];
  float gyro_cal[3];
}MPU6050_Calibration_Variables;

typedef struct{

  int16_t acc_total_vector, acc_raw[3];
  float angle_pitch_gyro;
  float angle_roll_gyro;
  float angle_pitch_acc;
  float angle_roll_acc;
  float angle_pitch;
  float angle_roll;
  int16_t raw_temp;
  float temp;
  float prevtime,prevtime1,time1, elapsedtime1;
}MPU6050_GetDataVariables;

MPU6050_InitStatus MPU6050_Init(I2C_HandleTypeDef *hi2cx);
MPU6050_Calibration_Variables MPU6050_Calibration()






#endif // MPU6050_H
