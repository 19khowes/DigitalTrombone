#ifndef __STM32L476R_NUCLEO_KEYPAD_H
#define __STM32L476R_NUCLEO_KEYPAD_H

void delay_ms(unsigned int ms);
void delay_10us(unsigned int us);
void keypad_init(void);
unsigned char keypad_listen(void);

#endif
