#include "mpu9250.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

MPU9250_Data imu;
uint8_t mpu_raw[14];
uint8_t mag_raw[7];
float mag_adjust[3];

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if(hi2c->Instance == I2C1) {
        mpu9250_parse_accel_gyro(mpu_raw);
        mpu9250_read_mag();
    }
}

void mpu9250_init(void) {
    uint8_t check;
    HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, &check, 1, HAL_MAX_DELAY);
    if(check != 0x71) { printf("MPU9250 not found!\r\n"); return; }
    printf("MPU9250 detected!\r\n");

    HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, PWR_MGMT_1_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x00}, 1, HAL_MAX_DELAY);
    HAL_Delay(100);

    HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, GYRO_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x00}, 1, HAL_MAX_DELAY);
    HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, ACCEL_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x00}, 1, HAL_MAX_DELAY);

    HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, INT_PIN_CFG, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x02}, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDR, AK8963_CNTL1, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x00}, 1, HAL_MAX_DELAY);
    HAL_Delay(10);
    HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDR, AK8963_CNTL1, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x0F}, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    uint8_t asa[3];
    HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDR, AK8963_ASAX, I2C_MEMADD_SIZE_8BIT, asa, 3, HAL_MAX_DELAY);
    for(int i=0;i<3;i++) mag_adjust[i] = ((asa[i]-128)/256.0f)+1.0f;

    HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDR, AK8963_CNTL1, I2C_MEMADD_SIZE_8BIT, (uint8_t[]){0x16}, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    printf("MPU9250 + AK8963 initialized!\r\n");
}

void mpu9250_parse_accel_gyro(uint8_t *raw) {
    int16_t ax = (raw[0]<<8)|raw[1];
    int16_t ay = (raw[2]<<8)|raw[3];
    int16_t az = (raw[4]<<8)|raw[5];
    int16_t gx = (raw[8]<<8)|raw[9];
    int16_t gy = (raw[10]<<8)|raw[11];
    int16_t gz = (raw[12]<<8)|raw[13];

    imu.ax = ax/16384.0f;
    imu.ay = ay/16384.0f;
    imu.az = az/16384.0f;
    imu.gx = gx/131.0f;
    imu.gy = gy/131.0f;
    imu.gz = gz/131.0f;

    imu.pitch = atan2f(imu.ay, sqrtf(imu.ax*imu.ax + imu.az*imu.az))*57.2958f;
    imu.roll  = atan2f(-imu.ax, imu.az)*57.2958f;
}

void mpu9250_read_mag(void) {
    uint8_t st1;
    HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDR, AK8963_ST1, I2C_MEMADD_SIZE_8BIT, &st1, 1, HAL_MAX_DELAY);
    if(st1 & 0x01) {
        HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDR, AK8963_XOUT_L, I2C_MEMADD_SIZE_8BIT, mag_raw, 7, HAL_MAX_DELAY);
        int16_t mx = (mag_raw[2]<<8)|mag_raw[1];
        int16_t my = (mag_raw[4]<<8)|mag_raw[3];
        int16_t mz = (mag_raw[6]<<8)|mag_raw[5];

        imu.mx = mx * mag_adjust[0] * 0.15f;
        imu.my = my * mag_adjust[1] * 0.15f;
        imu.mz = mz * mag_adjust[2] * 0.15f;

        float pitch_rad = imu.pitch * (M_PI/180.0f);
        float roll_rad  = imu.roll * (M_PI/180.0f);

        float mag_x_comp = imu.mx*cosf(pitch_rad) + imu.mz*sinf(pitch_rad);
        float mag_y_comp = imu.mx*sinf(roll_rad)*sinf(pitch_rad) + imu.my*cosf(roll_rad) - imu.mz*sinf(roll_rad)*cosf(pitch_rad);

        imu.yaw = atan2f(-mag_y_comp, mag_x_comp)*57.2958f;
        if(imu.yaw < 0) imu.yaw += 360.0f;
    }
}
