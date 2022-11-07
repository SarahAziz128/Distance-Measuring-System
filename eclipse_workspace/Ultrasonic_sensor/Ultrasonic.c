/*
 * Ultrasonic.c
 *
 *  Created on: Oct 18, 2022
 *      Author: Sara
 */


#include "gpio.h"
#include "icu.h"
#include "Ultrasonic.h"
#include "common_macros.h"
#include <util/delay.h>

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;


void  Ultrasonic_init(void){

	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};
	Icu_init(&Icu_Config);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_setupPinDirection(ULTRASONIC_PORT_ID,ULTRASONIC_PIN_NUM,PIN_OUTPUT);
	GPIO_writePin(ULTRASONIC_PORT_ID,ULTRASONIC_PIN_NUM , LOGIC_LOW);
}




void Ultrasonic_Trigger(void)
{
	GPIO_writePin(ULTRASONIC_PORT_ID,ULTRASONIC_PIN_NUM , LOGIC_HIGH);
	_delay_us(15);
	GPIO_writePin(ULTRASONIC_PORT_ID,ULTRASONIC_PIN_NUM , LOGIC_LOW);

}

uint16 Ultrasonic_readDistance(void){
	uint16 measure_distance;
	uint16 period;
	Ultrasonic_Trigger();
	Ultrasonic_edgeProcessing();


	/* calculate the period */
	period = ( g_timeHigh);
	/*calculate the distance*/
	measure_distance = period/58.5; /*speed of sound is 340m/sec therefore through this conversion the output measured distance is in cm*/

	return measure_distance;
}

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
		//Icu_DeInit();
		g_edgeCount = 0;

	}
}


