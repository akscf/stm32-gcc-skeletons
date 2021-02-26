#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

#define TIMER_PRESCALER	720

const uint16_t LED1 = GPIO_Pin_13;

// ---------------------------------------------------------------------------------
TIM_TimeBaseInitTypeDef timer;
uint8_t  step = 0;

// ---------------------------------------------------------------------------------
void init_leds();
void led1_on();
void led1_off();

void init_timer();
void start_timer();
void delay_ms(uint32_t delay);


// ---------------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------------
int main() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    init_timer();
    init_leds();

    start_timer();
    led1_on();

    while (1) {
	__NOP();
    }

}

// ---------------------------------------------------------------------------------
// functions
// ---------------------------------------------------------------------------------
void init_leds() {
    GPIO_InitTypeDef ist;
    
    GPIO_StructInit(&ist);

    ist.GPIO_Mode  = GPIO_Mode_Out_PP;		// GPIOMode_TypeDef
    ist.GPIO_Speed = GPIO_Speed_10MHz;		// GPIOSpeed_TypeDef
    ist.GPIO_Pin   = LED1;			// GPIO_pins_define
    
    GPIO_Init(GPIOC, &ist);
        
    GPIO_ResetBits(GPIOC, LED1);
}

void led1_on() {
    GPIO_SetBits(GPIOC, LED1);
}

void led1_off() {
    GPIO_ResetBits(GPIOC, LED1);
}

// ---------------------------------------------------------------------------------
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
	led1_off();
	step = 1;
    } else {
	led1_on();
	step = 0;
    }

}




