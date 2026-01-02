#ifndef TIM_H_
#define TIM_H_
void init_tim4_1hz(void);
void init_output_compare(void);
void init_input_compare(void);

#define SR_UIF (1U<<0)
#define SR_C1IF (1U<<1)
#endif /* TIM_H_ */
