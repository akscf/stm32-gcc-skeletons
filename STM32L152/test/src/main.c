#include <stm32l1xx.h>
#include <stm32l1xx_rcc.h>
#include <stm32l1xx_gpio.h>
#include <stm32l1xx_tim.h>

#define TIMER_PRESCALER	720

//------------------------------------------------------------------------------------------------------------
const uint16_t LED1 = GPIO_Pin_6;
const uint16_t LED2 = GPIO_Pin_7;

TIM_TimeBaseInitTypeDef timer;
uint8_t  step = 0;

void init_leds();
void led1_on();
void led1_off();
void led2_on();
void led2_off();

void init_timer();
void start_timer();
void delay_ms(uint32_t delay);

//------------------------------------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------------------------------------
int main() {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    init_timer();
    init_leds();

    start_timer();

    led2_on();

    while (1) {
	__NOP();
    }

}

//------------------------------------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------------------------------------
void init_leds() {
    GPIO_InitTypeDef ist;
    
    GPIO_StructInit(&ist);

    ist.GPIO_Mode  = GPIO_Mode_OUT;
    ist.GPIO_OType = GPIO_OType_PP;
    ist.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    ist.GPIO_Speed = GPIO_Speed_10MHz;
    ist.GPIO_Pin   = LED1 | LED2;
    
    GPIO_Init(GPIOB, &ist);
        
    GPIO_ResetBits(GPIOB, LED1);
    GPIO_ResetBits(GPIOB, LED2);
}

void led1_on() {
    GPIO_SetBits(GPIOB, LED1);
}

void led1_off() {
    GPIO_ResetBits(GPIOB, LED1);
}

void led2_on() {
    GPIO_SetBits(GPIOB, LED2);
}

void led2_off() {
    GPIO_ResetBits(GPIOB, LED2);
}

//------------------------------------------------------------------------------------------------------------
void init_timer() {
    TIM_TimeBaseStructInit(&timer);

    timer.TIM_Prescaler = TIMER_PRESCALER;

    timer.TIM_Period = 30000;
    TIM_TimeBaseInit(TIM4, &timer);	

}

void start_timer() {
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    //
    if(step == 0) {	
	led1_on();
	step = 1;
    } else {
	led1_off();
	step = 0;
    }
}






