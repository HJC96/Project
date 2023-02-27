#include "dma_adc.h" 

DMA_ADC::DMA_ADC(){}

DMA_ADC::~DMA_ADC(){}

void DMA_ADC::TIM2_Setup(){  
    RCC->APB1ENR |= 1;              /* enable TIM2 clock */
    TIM2->PSC = 10 - 1;             /* 100MHz -> 10MHz */
    TIM2->ARR = 10 - 1;             /* 10MHz ->  1MHz */
    TIM2->CNT = 0;
    TIM2->CCMR1 = 0x6800;           /* pwm1 mode,  preload enable */
    TIM2->CCER = 0x0010;            /* High polarity, ch2 enable */
    TIM2->CCR2 = 5-1; 				/* Duty = 40% */
	TIM2->CR2 = 5<<4;            /* OC2REF Signal -> Trigger output */
} 

void DMA_ADC::ADC1_Setup(){ 
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    GPIOA->MODER |=  3;             /* PA0 analog */
    RCC->APB2ENR |= 0x0100;         /* enable ADC1 clock */
    ADC1->CR1 = 0x2000000;          /* 8-bit conversion */
    ADC1->CR2 = 0x13000000;         /* exten rising edge, extsel 3 = tim2.2 */
    ADC1->CR2 |= 0x400;             /* enable setting EOC bit after each conversion */
    ADC1->CR2 |= 1;                 /* enable ADC1 */    
}

void DMA_ADC::DMA_ADC_Setup(int size){ 
    DMA2_Stream0->CR &= ~1; 
    while (DMA2_Stream0->CR & 1) {} 
    DMA2->LIFCR = 0x0F7D0F7D; DMA2->HIFCR = 0x0F7D0F7D;
    DMA2_Stream0->PAR |= (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR |= (uint32_t)&ADC_buffer; 
    DMA2_Stream0->NDTR = size+1; 
    DMA2_Stream0->CR = 0x00000000;  /* ADC1_0 on DMA2 Stream0 Channel 0 */
    DMA2_Stream0->CR |= 0x00000400; /* data size byte, mem incr, peripheral-to-mem */ 
    DMA2_Stream0->CR |= DMA_SxCR_TCIE;
    DMA2_Stream0->FCR = 0;          /* direct mode, no FIFO */
    
    DMA2_Stream0->CR |= 1;          /* enable DMA2 Stream 0 */         
    ADC1->CR2 |= 0x0100;            /* enable ADC conversion complete DMA data transfer */ 
    TIM2->CR1 = 1;                  /* enable timer2 */
}


