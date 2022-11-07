/*
 * main.c
 *
 *  Created on: Oct 18, 2022
 *      Author: Sara
 */

#include "lcd.h"
#include "Ultrasonic.h"
#include <avr/io.h>

int main(void){
	/* Enable Global Interrupt I-Bit */
		SREG |= (1<<7);
    /*initialize ultrasonic driver*/
		 Ultrasonic_init();
	/*initialize lcd drive*/
		 LCD_init();
		 LCD_displayString("Distance=");
		 LCD_moveCursor(0,9);
	/*declare variable for storing the returned and measured distance*/
		 uint16 distance;
	while (1){

		distance = Ultrasonic_readDistance();

				LCD_moveCursor(0,9);
				if (distance>=100)
				{
				 LCD_intgerToString(distance);

				}
				else if (distance>=10 && distance<100)
				{
					 LCD_intgerToString(distance);
					 LCD_displayStringRowColumn(0,12," ");
					 LCD_displayString("cm");

				}
				else if (distance<10 && distance>=0)
				{
					 LCD_intgerToString(distance);
					 LCD_displayStringRowColumn(0,11,"  ");
					 LCD_displayString("cm");


				}

			}

}
