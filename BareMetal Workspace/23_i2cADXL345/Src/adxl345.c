#include "adxl345.h"

char data;
uint8_t data_rec[6];
void read_adxl_addr(uint8_t reg)
{
	I2C1_ByteRead(DEVICE_ADDR,reg, &data);

}

void adxl_write(uint8_t reg, char val)
{
	char data[1];
	data[0]=val;

	I2C1_Burst_Write(DEVICE_ADDR,reg,1,data);

}

void adxl_read_values(uint8_t reg)
{
	I2C1_Burst_Read(DEVICE_ADDR,reg,6,(char*)data_rec);
}

void adxl_init()
{
	//Enable the i2c module
	i2c_init();
	//Read the device id, this should return 0xE5
	read_adxl_addr(DIVID_R);
	//set data format range to +_4g
	adxl_write(DATA_FORMAT_R,FOUR_G);
	//Reset all bits
	adxl_write(POWER_CTRL_R,RESET);
	//configure power control measure bit
	adxl_write(POWER_CTRL_R,SET_MEASURE_B);
}
