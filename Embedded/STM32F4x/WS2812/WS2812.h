#ifndef WS2812_H 
#define WS2812_H  

#include "mbed.h"   

#define LEDS_NUM 8
#define RESET_NUM 300+4
#define Lo 40
#define Hi 110

class WS2812{
    private:  
        uint16_t Blue[24] = { Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Lo,Lo,Lo,Lo };
        uint16_t Red[24] =  { Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo };  
        int x = 0;
        
    public:  
        uint16_t DMA_buf[LEDS_NUM * 24 + RESET_NUM] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Lo,Lo,Hi,Hi,Hi,Hi,Lo,Lo,
            Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo, Hi,Hi,Hi,Hi,Lo,Lo,Lo,Lo, Lo,Lo,Lo,Lo,Lo,Lo,Lo,Lo
        };  
        int idx = 0;
        
        WS2812();
        ~WS2812();
        void WS2812_Setup(uint16_t* Arr, int x);
        void GPIO_init(void);
        void Timer3_init(void);         
        void WS2812_DMA_Setup(unsigned int src, unsigned int dst, int len);   
}; 
#endif