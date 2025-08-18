# MPU6050-IMU-Library-for-STM32
A library and sample project file to use the MPU6050 IMU sensor on STM32 development boards.
This library allows you to quickly and structurally integrate the MPU6050 sensor into your project by keeping your main.c file clean.
## **Features**
#### - Clean Code Architecture: Separated into mpu6050.c and mpu6050.h for low dependency and high readability.

#### - STM32 HAL Compatible: Works seamlessly with projects created with STM32CubeIDE.

#### - Struct-based: All sensor data and settings are managed through a single MPU6050_t struct.

#### - Unit Conversion: Automatically converts raw sensor data to physical units:

#### + Accelerometer: g (gravity)

#### + Gyroscope: °/s (degrees/second)

#### + Temperature: °C (Celsius)

#### - Gyroscope Calibration: Calculates gyroscope offsets at startup for more precise measurements.

#### - Easy to Use: Start, calibrate, and read sensor data with just a few function calls.

## **Hardware Requirements**
#### Any STM32 series microcontroller (Examples are for the F1 series).

#### MPU6050 sensor module.

#### I2C connection (SCL, SDA).

## **Integration and Usage**
Adding this driver to your own project is very easy:

### **Copy the Files:**

Copy the mpu6050.c file to your project's Core/Src folder.

Copy the mpu6050.h file to your project's Core/Inc folder.

### **Add to IDE:**

In STM32CubeIDE, right-click on the project tree and "Refresh" (F5). Make sure the files are added to your project.
**See main.c file for usage example**
