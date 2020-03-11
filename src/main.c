/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "game.h"
#include "glcd.h"
			

int main(void)
{
		//Initialize gpioA for the button inputs.
		//The game runs on Timer 3 with updates on a 60 Hz frequency
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		GPIOA->MODER &= ~(0xf << 6*2);

		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

		TIM3->ARR = 200 - 1;
		TIM3->PSC = 10000 - 1;

		TIM3->CR1 |= TIM_CR1_CEN;

		NVIC_SetPriority(TIM6_DAC_IRQn,2);
		NVIC_SetPriority(TIM3_IRQn,1);

		gpiob_init();

		GLCD_init();
		GLCD_on();
		set_line(0);
		GLCD_fill();
		GLCD_clear();
		TIM3->DIER |= TIM_DIER_UIE;
		NVIC->ISER[0] |= (1 << TIM3_IRQn);

		for(;;)
			;
		for(int k = 0; k < 10; k += 1){
			GLCD_fill();
			micro_wait(1000);
			GLCD_clear();
			micro_wait(1000);
	}
}
