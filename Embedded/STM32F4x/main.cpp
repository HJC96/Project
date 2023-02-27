/*
    2020-1 Embedded Processor Application
    Section 2, Group 11  
*/
#include "mbed.h" 
#include "dma_adc.h" 
#include "WS2812.h"
#include "fourierTransform.h"  

/* UART */ 
#define UART_bufferSIZE 24
RawSerial pc(PA_2, PA_3, 115200); 
void rx_cb(void); 
char rx_buffer[UART_bufferSIZE];  
volatile int rx_index=0;  
volatile int rx_done=0;  
volatile int UART_tick=0; 
volatile int potentiometer_value=0;
volatile int Input_N=0;  

/* SPI */ 
SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); 
DigitalOut cs(SPI_CS); 
void set_MCP41010(void); 

/* DMA ADC*/  
DMA_ADC dma_adc; 
volatile bool tick_DMA2IRQ=false; 
void DMA2_Setup(void);  
void DMA2_Stream0_IRQHandler(void);

/* WS2812 */ 
WS2812 ws2812; 
volatile bool tick_DMA1IRQ=false; 
void DMA1_Setup(void);     
void DMA1_Stream5_IRQHandler(void); 

/* Button and Timeout*/
InterruptIn button(PC_13);
volatile bool tick_button= false;
void button_pressed(void);
Timeout flipper;
volatile bool Flag_flipper = false;
void flip(void); 

/* FFT */  
#define FFT_MAX_SIZE 1024
void test_fft(int q, float* fft_input); 

/* main */ 
int main(void) {
    
    // Setup UART
    pc.printf("Final Project!!\r\n"); 
    pc.attach(callback(rx_cb)); 
    
    // Setup SPI 
    cs=1; 
    spi.format(8,0); // 8 bits, mode=0 
    spi.frequency(1000000); // clock 1MHz   
    
    // Button 
    button.fall(&button_pressed); 
    
    // Setup TIM3, DMA1 for WS2812 
    ws2812.GPIO_init();
    ws2812.Timer3_init();
    DMA1_Setup();     
    
    // Variables
    float meas=0;  
    float sum=0;    
    int save_N = 0;  
    float fft_input[FFT_MAX_SIZE]; 
        
    while(1) {   
        pc.printf("Enter the value for the potentiometer[0, 255]:");   
        rx_done=0; while(rx_done==0){} set_MCP41010(); 
        
        pc.printf("Enter the How many measurements(N):");   
        rx_done=0; while(rx_done==0){}          
        save_N = Input_N;  
        
        while(true){ // start a DMA of ADC data transfer         
            flipper.attach(callback(flip), 1.0); // setup flipper to call flip after 1 seconds       
            pc.printf("Start measurements!\r\n");          
            dma_adc.TIM2_Setup(); dma_adc.ADC1_Setup(); DMA2_Setup();          
            dma_adc.DMA_ADC_Setup(save_N);  
            while(!tick_DMA2IRQ){} tick_DMA2IRQ=false;
            pc.printf("DMA TC Interrupt -> Finish measurements!\r\n");   
            
            // Print the ADC data   
            pc.printf("Input_N=%d\r\n",Input_N);  
            for(int i=1; i<=save_N; i++){  
                meas = (float)dma_adc.ADC_buffer[i-1]/255 * 3300;       
                fft_input[i-1] = meas; 
                sum = sum+meas;  
                if(i%5==0){ 
                    pc.printf("%0.f ",meas); 
                    pc.printf("\r\n");    
                }
                else
                    if(i==save_N){ 
                        pc.printf("%0.f ",meas);
                        pc.printf("\r\n");   
                    }
                    else 
                        pc.printf("%0.f ",meas);
            } 
            pc.printf("Average=%0.f[mV], WS2812:[%d]\r\n", sum/save_N, (int)(sum*255)/(save_N*3300));  
            
            // WS2812   
            ws2812.WS2812_Setup(ws2812.DMA_buf,0);  
            ws2812.WS2812_DMA_Setup((unsigned int)ws2812.DMA_buf, (unsigned int)0x40000438, ws2812.idx-1); 
            while(!tick_DMA1IRQ){} tick_DMA1IRQ=false;
            
            ws2812.WS2812_Setup(ws2812.DMA_buf,(int)(sum*255)/(save_N*3300)); 
            ws2812.WS2812_DMA_Setup((unsigned int)ws2812.DMA_buf, (unsigned int)0x40000438, ws2812.idx-1);  
            while(!tick_DMA1IRQ){} tick_DMA1IRQ=false; 
            pc.printf("DMA TC Interrupt -> Trun on WS2812!\r\n");       
			
            while(true){ 
                if(Flag_flipper==true){  
                    Flag_flipper=false;
                    break; 
                }
            }      
			
            pc.printf("Timeout Interrupt!!\r\n"); 
            meas=0; sum=0;
             
            if(tick_button==true){  
                tick_button=false;
                break;
            }
        }
        
        if(save_N==(save_N&-save_N)){    
            pc.printf("Start Fast Fourier Transform!!\r\n"); 
            test_fft(save_N, fft_input);  
        }
        Flag_flipper=false;
        tick_button=false; 
        save_N=0; Input_N=0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
/* UART */ 
void rx_cb(void){ 
    char ch; 
    ch = pc.getc(); 
    pc.putc(ch); 
    rx_buffer[rx_index++]=ch; 
    if(ch==0x0D){ // CR
        pc.putc(0x0A); //LF 
        rx_buffer[--rx_index]='\0'; // change CR to 0   
        if(UART_tick==0){  
            potentiometer_value = atoi((const char*)rx_buffer);  
            UART_tick++;
        }
        else if(UART_tick==1){  
            Input_N = atoi((const char*)rx_buffer);  
            UART_tick=0; 
        } 
        rx_index=0;
        rx_done=1;
    }
}

/* SPI */  
void set_MCP41010(void){ 
    cs=0; 
    spi.write(0x11); 
    spi.write((char)potentiometer_value); 
    cs=1;
} 

/* DMA ADC*/  
void DMA2_Setup(void){   
    RCC->AHB1ENR |= 0x00400000;     /* DMA2 controller clock enable */               
    DMA2->LIFCR = 0x0F7D0F7D; DMA2->HIFCR = 0x0F7D0F7D;  
    NVIC_SetPriorityGrouping(6); 
    NVIC_SetPriority(DMA2_Stream0_IRQn,0x40>>4);         
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);  /* DMA interrupt enable at NVIC */        
    NVIC_SetVector(DMA2_Stream0_IRQn,(uint32_t)DMA2_Stream0_IRQHandler); 
}

void DMA2_Stream0_IRQHandler(void){ 
    tick_DMA2IRQ=true; 
    DMA2->LIFCR = 0x0F7D0F7D; DMA2->HIFCR = 0x0F7D0F7D;  
    NVIC_ClearPendingIRQ(DMA2_Stream0_IRQn); 
    ADC1->CR2 &= ~0x0100;           /* disable ADC conversion complete DMA */ 
    DMA2_Stream0->CR = 0;           /* disable DMA2 Stream 0 */
    TIM2->CR1 &= ~1;                /* disable timer2 */ 
} 

/* WS2812 */ 
void DMA1_Setup(void){ 
    RCC->AHB1ENR |= 0x00200000;     /* DMA controller clock enable */
    DMA1->LIFCR = 0x0F7D0F7D; DMA1->HIFCR = 0x0F7D0F7D;   
    NVIC_SetPriorityGrouping(6);
    NVIC_SetPriority(DMA1_Stream5_IRQn,0x40>>4);       
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);  /* DMA interrupt enable at NVIC */
    NVIC_SetVector(DMA1_Stream5_IRQn, (uint32_t)DMA1_Stream5_IRQHandler);       
}

void DMA1_Stream5_IRQHandler(void){  
    tick_DMA1IRQ=true; 
    DMA1_Stream5->CR = 0;           /* disable DMA2 Stream 0 */
    DMA1->LIFCR = 0x0F7D0F7D; DMA1->HIFCR = 0x0F7D0F7D;
    NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
    TIM3->CR1 &= ~1;                /* disable timer2 */    
}

/* button and timer*/ 
void button_pressed(void){
    tick_button= true;    
} 

void flip(void){ 
    Flag_flipper = true;  
}

/* FFT */ 
void test_fft(int q, float* fft_input){  
    
    pc.printf("-------------------------------------------------------\r\n");
    vecRef input = vecAlloc(q);
    vecRef max_temp = vecAlloc(1);
    for (int i = 0; i < q; i++)
    { 
        input->arr[i] = ComplexMake(fft_input[i], 0);
    }
    vecRef output1 = NULL;
    fft(input, &output1);
    
    for(int i=0; i<(output1->length); i++){
        pc.printf("%.2f\r\n",cAbsolute(output1->arr[i]));
    }
    max_temp->arr[0]=output1->arr[1]; 
    int index=0;  
    
    // Find Second Largest Magnitude
    for(int i=1; i<(output1->length)/2; i++){ 
        if(cAbsolute(output1->arr[i])>=cAbsolute(max_temp->arr[0])){   
            max_temp->arr[0]=output1->arr[i]; 
            index=i;    
        }
    } 
    
    float div_factor = 1000/(float)q; 
    float sinusoidal_freq= (index*div_factor);
    pc.printf("----------------------------------------\r\n");
    pc.printf("DC , %0.2f\r\n", cAbsolute(output1->arr[0]));  
    pc.printf("%0.2f(KHz) , %0.2f\r\n", sinusoidal_freq ,cAbsolute(output1->arr[index]));   
    
    vecRelease(&input); 
    vecRelease(&max_temp); 
    vecRelease(&output1); 
}