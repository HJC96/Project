# STM32F4x

ARM Cortex-M4 계열의 프로세서를 포함한 뉴클레오보드를 이용
C++ 기반의 RTOS인 Mbed-OS 이용

### 프로젝트 시나리오
1.	터미널을 이용하여 Digital potentiometer의 값을 변화시킨다.
2.	Digital potentiometer로부터 생성된 전압을 이용하여, 한번에 측정 할 횟수 N을 입력 받는다.
3.	ADC를 이용하여 신호를 1us 간격으로 N번 측정한 다음, 측정 결과를 terminal에 출력한다.
4.	마지막 줄에 평균 값을 표시하고, 그 값을 WS2812 LED bar에 8-bit binary 형태로 표시한다.
5.	3 – 5 의 과정을 1초 간격으로 반복한다.
6.	User button을 누르면, 현재 진행 중인 측정을 완료한 다음 다시 1번부터 수행한다.

### 업무 수행 목록
- ADC를 이용해서 값을 측정하고, DMA를 이용하여 메모리로 전송하는 라이브러리 작성
- WS2812 제어를 위해 Timer의 PWM기능과 DMA를 이용하여 구현하는 라이브러리 작성
- Sinusoidal signal을 입력시켜, Fast Fourier transform 한 결과를 표시하는 기능 구현
