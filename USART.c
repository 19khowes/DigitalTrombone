#include "USART.h"
#include "stm32l476xx.h"


/******************************************************************************
 * Authors: Kade Howes, Forrest Nichols, Justin Wellington
 * 
 * Summary: 
 * 
 * ***************************************************************************/


/**
Psuedocode 
Begin
	Enable HSI clock
	Disable USART 
	Set data length to 8 bits (00)
	Select 1 stop bit (00)
	Set parity control as no parity (0)
	Oversample by 16
	Baud rate 9600
	Enable transmision and reception
	Enable USART
	Check USART is ready for transmission
End
*/
void USART_Init(USART_TypeDef * USARTx){
	
	
	// Disable USART 
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// Set data length to 8 bits (00)
	USARTx->CR1 &= ~USART_CR1_M;
	
	// Select 1 stop bit (00)
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	// Set parity control as no parity (0)
	USARTx->CR1 &= ~USART_CR1_PCE;
	
	// Oversample by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	// Baud rate 9600 (USARTDIV = 1667 for 16MHz clock
	USARTx->BRR = 0x0683;
	
	// Enable transmision and reception
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);

	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	// Check USART is ready for transmission
	while ((USARTx->ISR & USART_ISR_TEACK) == 0);
	
	while ((USARTx->ISR & USART_ISR_REACK) == 0);

}

/**
Psuedocode 
Begin
	Enable clock for GPIOA
	Select alternate function mode for PA2 and PA3
	Set PA2 and PA3 to AF7
	Set PA2 and PA3 to high speed (11)
	Set PA2 and PA3 to pull up 
	Set PA2 and PA3 to push-pull
	Enable clock for USART2 
	Select system clock for USART2
End
*/
void GPIO_USART_Init(void){
	// turn on HSI16 MHz clock
	RCC->CR |= RCC_CR_HSION;

	// wait for HSI clock to be ready
	while((RCC->CR & RCC_CR_HSIRDY) == 0)
		;
	
	// select HSI 
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	
	// Enable clock for GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Enable clock for GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Select alternate function mode for PA2, PA3(10)
	GPIOA->MODER &= ~(0xFU << (2*2));
	GPIOA->MODER |= 0xA << (2*2);
	//GPIOA->MODER &= ~(0xFU << (9*2));
	//GPIOA->MODER |= 0xA << (9*2);
	
	// select alternate funiton mode for PB6 and 7 (10)
	GPIOB->MODER &= ~(0xFU << (2*6));
	GPIOB->MODER |= (0xA << (2*6));
	
	
	// Set PA2 and PA3 to AF7
	GPIOA->AFR[0] |= 0x77 << (2*4);
	// Set PA9 and PA10 to AF7
	//GPIOA->AFR[1] |= 0x77;
	
	// set PB7 and 7 to AF7
	GPIOB->AFR[0] |= 0x77 << (4*6);
	
	// Set PA2 and PA3 to high speed (11)
	GPIOA->OSPEEDR |= 0xF << (2*2);
	// Set PA9 and PA10 to high speed (11)
	// GPIOA->OSPEEDR |= 0xF << (9*2);
	
	// set PB6 and 7 to high speed (11)
	GPIOB->OSPEEDR |= 0xF << (2*6);
	
	// Set PA2 and PA3 to pull up 
	GPIOA->PUPDR &= ~(0xFU << (2*2));
	GPIOA->PUPDR |= 0x5 << (2*2);
	// Set PA9 and PA10 to pull up 
	// GPIOA->PUPDR &= ~(0xFU << (9*2));
	// GPIOA->PUPDR |= 0x5 << (9*2);
	
	// set PB6 and 7 to pull up
	GPIOB->PUPDR &= ~(0xFU << (2*6));
	GPIOB->PUPDR |= (0x5 << (2*6));
	
	// Set PA2 and PA3 to push-pull
	GPIOA->OTYPER &= ~(0x3U << 2);
	// Set PA9 and PA10 to push-pull
	// GPIOA->OTYPER &= ~(0x3U << 9);
	GPIOB->OTYPER &= ~(0x3U << 6);
	
	// Enable clock for USART 2 
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// Select system clock for USART2 (01)
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	
	// Enable clock for USART 1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Select system clock for USART1 (01)
	RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL);
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;

}

/**
Psuedocode 
Begin
	
End
*/
void USART_Read(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes){
	unsigned int i;
	
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_RXNE));
		buffer[i] = USARTx->RDR;
	}
}

/**
Psuedocode 
Begin
	
End
*/
void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes){
	unsigned int i;
	
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));
		USARTx->TDR = buffer[i] & 0xFF;
	}
	
	while(!(USARTx->ISR & USART_ISR_TC));
	
	USARTx->ICR |= USART_ICR_TCCF;
}

void USARTPrint(uint8_t * stringToPrint) {
		uint8_t msgSend[100];
		unsigned int i;
		// "clear" out msgSend before sending new message
		for (i = 0; i < 100; i++) {
			msgSend[i] = 0x00;
		}
		// loop through string (char *) until null
    unsigned char * newptr = stringToPrint;
		i = 0;
    while (*newptr != 0x00) {
			// insert the stringToPrint into msgSend array
			msgSend[i] = stringToPrint[i];

			// change pointer to go to next character in string
			newptr++;
			i++;
    }
		
		
		USART_Write(USART2, msgSend, 100);
}


