#include "stm32l476xx.h"
#ifndef __STM32L476R_NUCLEO_USART_H
#define __STM32L476R_NUCLEO_USART_H

void USART_Init(USART_TypeDef * USARTx);
void GPIO_USART_Init(void);
void USART_Read(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
void USARTPrint(uint8_t * stringToPrint);

#endif
