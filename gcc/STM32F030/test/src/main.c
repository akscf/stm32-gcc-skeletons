#include <stm32f0xx.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_tim.h>

#define TIMER_PRESCALER	720

const uint16_t LED1 = GPIO_Pin_0; // PA0

// ---------------------------------------------------------------------------------
TIM_TimeBaseInitTypeDef timer;
uint8_t  step = 0;

// ---------------------------------------------------------------------------------
void init_leds();
void led1_on();
void led1_off();

void init_timer();
void start_timer();

// ---------------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------------
int main() {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    init_timer();
    init_leds();

    start_timer();
    while (1) {
	__NOP();
    }

}

// ----------------------------------------------------------------------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------------------------------------------------------------------
void init_leds() {
    GPIO_InitTypeDef ist;
    
    GPIO_StructInit(&ist);

    ist.GPIO_Mode  = GPIO_Mode_OUT;
    ist.GPIO_OType = GPIO_OType_PP;
    ist.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    ist.GPIO_Speed = GPIO_Speed_10MHz;
    ist.GPIO_Pin   = LED1;
    
    GPIO_Init(GPIOA, &ist);
        
    GPIO_ResetBits(GPIOA, LED1);
}

void led1_on() {
    GPIO_SetBits(GPIOA, LED1);
}

void led1_off() {
    GPIO_ResetBits(GPIOA, LED1);
}

//------------------------------------------------------------------------------------------------------------
void init_timer() {
    TIM_TimeBaseStructInit(&timer);

    timer.TIM_Prescaler = TIMER_PRESCALER;
    timer.TIM_Period = 3000;
    TIM_TimeBaseInit(TIM3, &timer);
}

void start_timer() {
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if(step == 0) {	
	led1_on();
	step = 1;
    } else if (step == 1) {
	led1_off();
	step = 0;
    }
}






