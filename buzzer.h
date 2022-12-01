#ifndef __STM32L476R_BUZZER_H
#define __STM32L476R_BUZZER_H

void SysTick_Initialize(unsigned int ticks);
void SysTick_Load(unsigned int ticks);
void hertz_to_load(double freq);
void toggle_buzzer_pin(void);
void buzzer_init(void);
void synthesize(void);

extern unsigned char direction;
extern unsigned int stepCounter; 


#endif
