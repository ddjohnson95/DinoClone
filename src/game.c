/*
 * game.c
 *
 *  Created on: Nov 23, 2019
 *  Updated: 8:00 11/27/2019
 *      Author: dave
 */

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "glcd.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

//Initialize structs for the game sprites
typedef struct{int SizeA; int SizeB; int PositionX; int PositionY;} enemy;

typedef struct{int mode; int SizeA; int SizeB; int PositionX; int PositionY;} dino;

//initialize sizes and the positions of the trees and crows.
enemy Tree1 = {.SizeA = 15, .SizeB = 10, .PositionX = 128, .PositionY = 49};
enemy Tree2 = {.SizeA = 15, .SizeB = 10, .PositionX = 220, .PositionY = 49};

enemy Crow = {.SizeA = 10, .SizeB = 15, .PositionX = 220, .PositionY = 33};

//Initialize the player sprite
dino Trex = {.mode = 0, .SizeA = 20, .SizeB = 15, .PositionX = 5, .PositionY = 36};

//Initialize game parameters
int horizon = 36;
int restart = 0;
int jump = 0;
int dodge = 0;
int randNum = 0;
int rollTheDice = 1;
float Speed = 5000;

int changeFoot = 0;
int changeFootCount = 0;

//Generate sprite arrays

uint8_t dino_laugh[28][25] = {
		//[20][15] to [20][25] is used to cover the head when the dino stop dodging
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
uint8_t dino_right[28][25] = {
		//[20][15] to [20][25] is used to cover the head when the dino stop dodging
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
uint8_t dino_left[28][25] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
uint8_t cactii[15][17] = {
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1},
		{0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,0,1},
		{0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,0,1},
		{0,0,1,1,0,1,1,1,1,0,1,0,0,0,1,0,1},
		{1,0,1,1,0,0,0,1,1,0,1,0,0,0,1,0,1},
		{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1},
		{1,0,1,1,0,1,0,1,1,1,1,0,1,0,1,0,1},
		{1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1},
		{1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,0,1},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0},
		{0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0}};

//Special sprite to erase the cactii
uint8_t cactiiclear[15][17] = {{0},{0}};

uint8_t cactus[15][10] = {
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,1,1},
		{0,0,0,0,1,1,1,0,1,1},
		{1,1,1,0,1,1,1,0,1,1},
		{1,1,1,0,1,1,1,0,1,1},
		{1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0}};

uint8_t bird[10][15] = {
		{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,1,1,0,0,0,1,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0}};

uint8_t dino_dodging_left[25][25] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0},
		{0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

uint8_t dino_dodging_right[25][25] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0},
		{0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}};


//Function to display a sprite on screen
void printCharacter(int CharacterLabel, uint8_t LeftTopX, uint8_t LeftTopY){

	switch (CharacterLabel)
	{
	case 0: // dinasour right
		image(dino_right, 28, 25, LeftTopX, LeftTopY);
		break;
	case 1: // dinasour left
		image(dino_left, 28, 25, LeftTopX, LeftTopY);
		break;
	case 2: // cactii
		image(cactii, 15, 17, LeftTopX, LeftTopY);
		break;
	case 3: // cactus
		image(cactus, 15, 10, LeftTopX, LeftTopY);
		break;
	case 4: // bird
		image(bird, 10, 15, LeftTopX, LeftTopY);
		break;
	case 5: //dodging dino left
		image(dino_dodging_left, 25, 25, LeftTopX, LeftTopY);
		break;
	case 6: //dodging dino right
		image(dino_dodging_right, 25, 25, LeftTopX, LeftTopY);
		break;
	case 7: //dodging dino laugh
		image(dino_laugh, 25, 25, LeftTopX, LeftTopY);
		break;
	case 8: // cactiiclear
		image(cactiiclear, 15, 17, LeftTopX, LeftTopY);
		break;
	default: // code to be executed if n doesn't match any cases
		break;
	}
}

void setupMusic()
{
	/* Student code goes here */
	//Enable system clock for Timer 1
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->BDTR |= TIM_BDTR_MOE;

	//Set the counting direction (clear/(0) for count up)
	TIM1->CR1 &= ~TIM_CR1_DIR;

	TIM1->CR1 &= 0x0;

	//Configure CH1-3 for PWM mode 1
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_0;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; //| TIM_CCMR1_OC2PE;


	TIM1->CCER |= TIM_CCER_CC1E;
	TIM1->CCER |= TIM_CCER_CC2E;

	//Set prescaler for 2 MHz output
	TIM1->PSC = 480 - 1;

	//Set ARR for 1000 Hz output
	TIM1->ARR = 100 - 1;

	//Enable timer
	TIM1->CR1 |= TIM_CR1_CEN;

	//Enable port A
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	//Set PA8 to alternate function
	GPIOA->MODER &= ~(15 << 16);
	GPIOA->MODER |= (10 << 16);

	GPIOA->MODER |= GPIO_MODER_MODER0_0;

	//Set the alternate function for PA8
	GPIOA->AFR[1] &= ~0xff;
	GPIOA->AFR[1] |= 0x22;

	return;
}

void beep(void){
	DAC->CR |= DAC_CR_EN1;
	nano_wait(50000000);
	DAC->CR &= ~DAC_CR_EN1;
}

//The game ticks Timer 3
void TIM3_IRQHandler (void)
{
    //ack interrupt
    TIM3->SR &= ~TIM_SR_UIF;
    gameCode();
}

//Stop the game and blackout the screen
void game_over(){
	TIM3->DIER &= ~TIM_DIER_UIE;
	TIM6->DIER &= ~TIM_DIER_UIE;
	GLCD_fillLine(2);
	GLCD_fillLine(3);
	GLCD_fillLine(4);
	GLCD_fillLine(5);
	GLCD_fillLine(6);
	GLCD_fillLine(7);
	}

void gameCode() {
	if(changeFootCount > 5){
		if(changeFoot == 1){
			if(dodge == 0 && jump == 0){
				printCharacter(0, Trex.PositionX, Trex.PositionY);
			}
			if(dodge == 1){
				printCharacter(5, Trex.PositionX, Trex.PositionY-10);
			}
			changeFoot = 0;
		}else{
			if(dodge == 0 && jump == 0){
				printCharacter(1, Trex.PositionX, Trex.PositionY);
			}
			if(dodge == 1){
				printCharacter(6, Trex.PositionX, Trex.PositionY-10);
			}
			changeFoot = 1;
		}
		changeFootCount = 0;
	}else{
		changeFootCount++;
	}

	if((((GPIOA->IDR & 0x80)>>7) == 0) && (jump == 0) ){
		dodge = 1;//dodge = 1 dodge. dodge = 0 nothing
		jump = 0;
	}else{
		dodge = 0;
	}

	if((((GPIOA->IDR & 0x40)>>6) == 0) && (jump == 0) ){
		jump = 1;//jump = 1 up. jump = 2 down
		dodge = 0;
	}

	if(jump == 1){
		rectangle(Trex.PositionX, Trex.PositionY - 2 + Trex.SizeA, 1, Trex.SizeB, 0);
		Trex.PositionY = Trex.PositionY - 1.5;
		printCharacter(1, Trex.PositionX, Trex.PositionY);
		if(Trex.PositionY <= 5){
			jump = 2;
		}
	}else if(jump == 2){
		rectangle(Trex.PositionX, Trex.PositionY - 1, 2, Trex.SizeB, 0);
		Trex.PositionY = Trex.PositionY + 1.5;
		printCharacter(1, Trex.PositionX, Trex.PositionY);
		if(Trex.PositionY >= 39){
			jump = 0;
		}
	}

	if(dodge == 1 && (jump == 0)){//change the edge of Trex
		rectangle(Trex.PositionX, Trex.PositionY-5, 5, Trex.SizeB, 0);
		Trex.SizeA = 16;
		Trex.SizeB = 26;
		Trex.PositionX = 5;
		Trex.PositionY = 47;
		if(changeFootCount > 5){
			if(changeFoot == 1){
				printCharacter(5, Trex.PositionX, Trex.PositionY-10);
				changeFoot = 0;
			}else{
				printCharacter(6, Trex.PositionX, Trex.PositionY-10);
				changeFoot = 1;
			}
				changeFootCount = 0;
		}else{
			changeFootCount++;
		}

	}else if(dodge == 0 && (jump == 0)){
		Trex.SizeA = 20;
		Trex.SizeB = 15;
		Trex.PositionX = 5;
		Trex.PositionY = 39;

		if(changeFootCount > 5){
			if(changeFoot == 1){
				printCharacter(0, Trex.PositionX, Trex.PositionY);
				changeFoot = 0;
			}else{
				printCharacter(1, Trex.PositionX, Trex.PositionY);
				changeFoot = 1;
			}
			changeFootCount = 0;
		}else{
			changeFootCount++;
		}
	}

	//Move Tree 1
	printCharacter(8, Tree1.PositionX, Tree1.PositionY);
	Tree1.PositionX = Tree1.PositionX - 1;
	if(Tree1.PositionX + Tree1.SizeB <= 0){
		if(randNum == 0){
			if(128 - Tree2.PositionX > 50){
				Tree1.PositionX = 128 + rand() % 50;
			}else{
				Tree1.PositionX = 128  + rand() % 50 + (50 - (128 - Tree2.PositionX));
			}
		}else{
			if(128 - Crow.PositionX > 50){
				Tree1.PositionX = 128 + rand() % 50;
			}else{
				Tree1.PositionX = 128  + rand() % 50 + (50 - (128 - Crow.PositionX));
			}
		}
		//Tree1.PositionX = 180 + rand() % 50;//Never be larger than 230
	}

	printCharacter(2, Tree1.PositionX, Tree1.PositionY);

	if(rollTheDice == 1){
		randNum = rand() % 2;
		rollTheDice = 0;
	}
	if(randNum == 0){//Move Tree 2
		rectangle(Tree2.PositionX, Tree2.PositionY, Tree2.SizeA, Tree2.SizeB, 0);
		Tree2.PositionX = Tree2.PositionX -1;
		if(Tree2.PositionX + Tree2.SizeB == 0){
			if(128 - Tree1.PositionX > 50){
				Tree2.PositionX = 128 + rand() % 50;
			}else{
				Tree2.PositionX = 128  + rand() % 50 + (50 - (128 - Tree1.PositionX));
			}
			//Tree2.PositionX = 180 + rand()// % 50;//Never be larger than 230
			rollTheDice = 1;
		}
		printCharacter(3, Tree2.PositionX, Tree2.PositionY);
	}else{
		//Move Crow
		rectangle(Crow.PositionX, Crow.PositionY, Crow.SizeA, Crow.SizeB, 0);
		Crow.PositionX = Crow.PositionX - 1;
		if(Crow.PositionX + Crow.SizeB == 0){
			if(128 - Tree1.PositionX > 50){
				Crow.PositionX = 128 + rand() % 50;
			}else{
				Crow.PositionX = 128  + rand() % 50 + (50 - (128 - Tree1.PositionX));
			}
			//Crow.PositionX = 180 + rand() % 50;//Never be larger than 230
			rollTheDice = 1;
		}
		printCharacter(4, Crow.PositionX, Crow.PositionY);
	}

		if((Trex.PositionY + Trex.SizeA > Tree1.PositionY &&
			Trex.PositionX + Trex.SizeB > Tree1.PositionX &&
			Trex.PositionX + Trex.SizeB < Tree1.PositionX + Tree1.SizeB)
			||(Trex.PositionY + Trex.SizeA > Tree1.PositionY &&
			Trex.PositionX > Tree1.PositionX &&
			Trex.PositionX < Tree1.PositionX + Tree1.SizeB)){
			game_over();

		}
		if((Trex.PositionY + Trex.SizeA > Tree2.PositionY &&
			Trex.PositionX + Trex.SizeB > Tree2.PositionX &&
			Trex.PositionX + Trex.SizeB < Tree2.PositionX + Tree2.SizeB)
				||(Trex.PositionY + Trex.SizeA > Tree2.PositionY &&
				Trex.PositionX > Tree2.PositionX &&
				Trex.PositionX < Tree2.PositionX + Tree2.SizeB)){
			game_over();

		}
		if((dodge == 0 && Trex.PositionY + Trex.SizeA > Crow.PositionY &&
			Trex.PositionX + Trex.SizeB > Crow.PositionX &&
			Trex.PositionX + Trex.SizeB < Crow.PositionX + Crow.SizeB)
			||(dodge == 0 && Trex.PositionY + Trex.SizeA > Crow.PositionY &&
			Trex.PositionX > Crow.PositionX &&
			Trex.PositionX < Crow.PositionX + Crow.SizeB)
			||(dodge == 1 && Trex.PositionY < Crow.PositionY + Crow.SizeA &&
			Trex.PositionX + Trex.SizeB > Crow.PositionX &&
			Trex.PositionX + Trex.SizeB < Crow.PositionX + Crow.SizeB)
			){

			game_over();
		}
	GLCD_refreshFrame();
	}
