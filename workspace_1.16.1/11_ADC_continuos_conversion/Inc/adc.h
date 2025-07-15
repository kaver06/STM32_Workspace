

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

void init_adc_PA1(void);
void start_conversion(void);
uint32_t read_adc(void);


#endif /* ADC_H_ */
