/*
 * homework4.h
 *
 *  Created on:
 *      Author:
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
typedef enum {STATE0,STATE1, STATE2, STATE3, STATE4} states;
bool charFSM(char rChar, states currentState);

// TODO: Define any constants that are local to homework.c using #define


#endif /* HOMEWORK4_H_ */
