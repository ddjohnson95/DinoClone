/*
 * game.c
 *
 *  Created on: Nov 23, 2019
 *  Updated: 8:00 11/27/2019
 *      Author: dave
 */

#ifndef GAME_H_
#define GAME_H_

void printCharacter(int CharacterLabel, uint8_t LeftTopX, uint8_t LeftTopY);
void TIM3_IRQHandler (void);
void TIM6_DAC_IRQHandler(void);
void game_over(void);
void gameCode(void);


#endif /* GAME_H_ */
