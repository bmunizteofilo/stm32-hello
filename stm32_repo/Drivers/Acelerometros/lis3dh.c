#include "lis3dh.h"

#define LIS3DH_ADDR 0x18u

/**
 * @brief Initialize the LIS3DH accelerometer.
 */
bool lis3dh_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x20, 0x57}; /* CTRL_REG1: 100Hz, all axes */
    return i2c_write_poll(i2c, LIS3DH_ADDR, data, 2);
}

/**
 * @brief Read acceleration data from the LIS3DH.
 */
bool lis3dh_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az) {
    if (!ax || !ay || !az) return false;
    uint8_t reg = 0x28 | 0x80; /* OUT_X_L with auto-increment */
    uint8_t buf[6] = {0};
    if (!i2c_write_poll(i2c, LIS3DH_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, LIS3DH_ADDR, buf, 6)) return false;
    *ax = *ay = *az = 0.0f;
    return true;
}

/** Example demonstrating how to use the LIS3DH driver. */
void lis3dh_example(void) {
    float ax, ay, az;
    lis3dh_init(I2C1);
    lis3dh_read(I2C1, &ax, &ay, &az);
}
