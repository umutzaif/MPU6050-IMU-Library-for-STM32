# MPU6050 IMU Library for STM32 MCUs

This repository provides an example project using the **MPU6050 accelerometer + gyroscope sensor** with the **STM32F103C8T6 (Blue Pill)** board.  
The main goal is to create a **reusable MPU6050 library** and demonstrate its usage.

## 🚀 Features
- MPU6050 driver (`mpu6050.c/.h`) for easy integration via I2C
- Read raw accelerometer, gyroscope, and temperature data
- Gyroscope calibration
- Pitch and roll angle calculation (complementary filter)
- Example of displaying sensor data on a 16x2 LCD

## 📂 Project Structure
```
/Core/Inc/
main.h
mpu6050.h -> MPU6050 function prototypes
/Core/Src/
main.c -> Example usage
mpu6050.c -> MPU6050 driver implementation
```


## 🔧 Hardware Requirements
- STM32F103C8T6 (Blue Pill)
- MPU6050 sensor module
- 16x2 LCD (I2C or parallel)
- Jumper wires

## 📌 Connections
| MPU6050 | STM32F103C8T6 |
|---------|---------------|
| VCC     | 3.3V          |
| GND     | GND           |
| SCL     | PB6 (I2C1_SCL)|
| SDA     | PB7 (I2C1_SDA)|

> ⚠️ The MPU6050 is **not 5V tolerant**, always power it with **3.3V**.

## 🛠️ How to Use
1. Clone the repository:
   ```bash
   git clone https://github.com/umutzaif/MPU6050-IMU-Library-for-STM32.git
2. Open the project in STM32CubeIDE.

3. Make sure MX_I2C1_Init() is configured for the correct pins.

3. Use the provided example in main.c.

## 🗒️ Note
In this project, sensor data is displayed on a 16x2 LCD screen. I used the LCD.h library for this. For more information about this library, visit [github.com.](https://github.com/BahadirAydinoglu/STM32F1-LCD-Tutorial)

## 📝 License

This project is licensed under the MIT License. See the LICENSE file for details.
