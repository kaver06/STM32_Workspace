#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include <stdio.h>

#define MPU9250_ADDR        (0x68 << 1)
#define AK8963_ADDR         (0x0C << 1)

#define ACCEL_XOUT_H_REG    0x3B
#define INT_PIN_CFG         0x37
#define PWR_MGMT_1_REG      0x6B
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C
#define WHO_AM_I_REG        0x75

#define AK8963_CNTL1        0x0A
#define AK8963_ASAX         0x10
#define AK8963_ST1          0x02
#define AK8963_XOUT_L       0x03

typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    float pitch, roll, yaw;
} MPU9250_Data;

extern MPU9250_Data imu;
extern uint8_t mpu_raw[14];
void mpu9250_init(void);
void mpu9250_parse_accel_gyro(uint8_t *raw);
void mpu9250_read_mag(void);

#endif /* INC_MPU9250_H_ */
