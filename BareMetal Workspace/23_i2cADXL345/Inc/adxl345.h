/*
 * adxl345.h
 *
 *  Created on: Sep 18, 2025
 *      Author: kaver
 */

#ifndef ADXL345_H_
#define ADXL345_H_
#include <stdint.h>
#include "i2c.h"
#define DIVID_R			(0x00)
#define DEVICE_ADDR		(0x53)
#define DATA_FORMAT_R	(0x31)
#define POWER_CTRL_R	(0x2D)
#define DATA_START_ADDR	(0x32)

#define FOUR_G			(0x01)
#define RESET			(0x00)
#define SET_MEASURE_B	(0x08)

void adxl_init();
void adxl_read_values(uint8_t reg);
#endif /* ADXL345_H_ */
