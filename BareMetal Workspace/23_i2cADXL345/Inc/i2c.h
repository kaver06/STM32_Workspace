/*
 * i2c.h
 *
 *  Created on: Aug 1, 2025
 *      Author: kaver
 */

#ifndef I2C_H_
#define I2C_H_

void i2c_init(void);
void I2C1_Burst_Write(char saddress,char maddress,int n,char *data);
void I2C1_Burst_Read(char saddress,char maddress,int n,char *data);
void I2C1_ByteRead(char saddress,char maddress,char* data);
#endif /* I2C_H_ */
