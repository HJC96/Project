#ifndef DMA_ADC_H 
#define DMA_ADC_H  

#include "mbed.h"   

#define ADC_bufferSIZE 2048 

class DMA_ADC{
    
    private: 
            
    public:   
        DMA_ADC();
        ~DMA_ADC();
        uint8_t ADC_buffer[ADC_bufferSIZE];    
        void TIM2_Setup(void); 
        void ADC1_Setup(void);        
        void DMA_ADC_Setup(int size); 
}; 
#endif