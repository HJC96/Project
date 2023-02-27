#include "WS2812.h" 

WS2812::WS2812(){}

WS2812::~WS2812(){} 

void WS2812::WS2812_Setup(uint16_t* Arr, int x){ 
    idx = 0;
    for (int j = 0; j < 7; j++) {
        if ((x >> j) % 2) { // &1
            for (int i = 0; i < 24; i++) {
                Arr[idx] = Red[i];
                idx++;
            }
        }
        else {
            for (int i = 0; i < 24; i++) {
                Arr[idx] = Blue[i];
                idx++;
            }
        }
    }
    if ((x >> 7) % 2) {
        for (int i = 0; i < 22; i++) {
            Arr[idx] = Red[i];
            idx++;
        }
        for (int i = 0; i < 6; i++) {
            Arr[idx] = Lo;
            idx++;
        }
  }
    else {
        for (int i = 0; i < 22; i++) {
            Arr[idx] = Blue[i];
            idx++;
        }
        for (int i = 0; i < 6; i++) {
            Arr[idx] = Hi;
            idx++;
        }
    }
        int _idx = idx;
        for (int i = _idx; i < _idx+300; i++) {
        Arr[i] = 0;
        idx++;
    }
} 

void WS2812::GPIO_init(void){  
    /* RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) */
    RCC->AHB1ENR |= 1<<2; // Bit(2) GPIOCEN: IO port clock enable 
    // PC_7: D9 PWM
    /* GPIO port mode register */
    GPIOC->MODER &= ~(0x3 << 14); // Clear Bit(5,4) for setting PC7
    GPIOC->MODER |= (0x2 << 14); // PC7->Alternate function mode 
    /* GPIO alternate function low register (GPIOC_AFR[0]) */
    GPIOC->AFR[0] &= ~(0xF << 28);
    GPIOC->AFR[0] |= (0x2 << 28);   
} 

void WS2812::Timer3_init(void){ 
    RCC->APB1ENR |= (1 << 1); // Bit 1 TIM3EN 
    TIM3->CR1 |= (0x1 << 7);
    TIM3->CR2 = 0;
    TIM3->PSC = 0;    // 100MHz -> 10ns
    TIM3->ARR = 150 - 1;  // 10ns -> 1500ns
    TIM3->CCMR1 |= (0x6 << 12);  // ch2: OC Mode = PWM mode1 ( 110 )
    TIM3->CCR2 = 0;
    TIM3->DIER |= (0x1 << 10); // Bit 10 CC2DE: Capture/Compare 2 DMA request enable  
    TIM3->CR1 |= (0x1);
    TIM3->CCER |= (0x1 << 4);  // ch2: output enable    
}

void WS2812::WS2812_DMA_Setup(unsigned int src, unsigned int dst, int len){ 
    // channel5, stream5 -> TIM3 CH2, 190Page
    DMA1_Stream5->CR &= ~1;
    while (DMA1_Stream5->CR & 1) {}
    DMA1->LIFCR = 0x0F7D0F7D; DMA2->HIFCR = 0x0F7D0F7D;
    DMA1_Stream5->PAR = dst;
    DMA1_Stream5->M0AR = src;
    DMA1_Stream5->NDTR = len;
    DMA1_Stream5->CR = 0x5 << 25;  /* 101: channel 5 selected  */
    DMA1_Stream5->CR |= 0x00000440; /* data size byte, mem incr, mem-to-peripheral */
    DMA1_Stream5->CR |= 1 << 13; // sending datasize = 16bit
    DMA1_Stream5->CR |= 1 << 11;
    DMA1_Stream5->CR |= DMA_SxCR_TCIE;
    DMA1_Stream5->FCR = 0;         /* direct mode, no FIFO */
    DMA1_Stream5->CR |= 1;          /* enable DMA1 Stream 5 */
    TIM3->CR1 |= 1;         // Запускаем таймер 
}