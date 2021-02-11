/************************************************************************************************
 *   File Name : main.c																			*
 *  Created on : Dec 28, 2019																	*
 *      Author : Mohamad																		*
 ************************************************************************************************/
#include "adc.h"
#include "lcd.h"
#include "externalInterrupts.h"
#include "dcMotor.h"
#include "pwm.h"
#include "std_types.h"
#include "common_macros.h"

/************************************************************************************************
 * Variable Name : motor1																		*
 * Variable type : dcMotorData struct															*
 * Variable function : handling the configurations of a dc motor under the name of motor1		*
 ************************************************************************************************/
dcMotorData motor1 = { dcMPORTB, dcMPin3, mDiPORTB, mDiPin4 };

/************************************************************************************************
 * Variable Name : pwm1																			*
 * Variable type : pwmConfig struct																*
 * Variable function : handling the configurations of a PWM to control motor1					*
 ************************************************************************************************/
pwmConfig pwm1 = { pwmT0, pwm_64, 0 };

/************************************************************************************************
 * Function Name : motor1																		*
 * Function Return type : void																	*
 * Function Arguments/types : one argument of type 16-bit unsigned integer (uint16)				*
 * Arguments names/job : a_analogVal : it is holding the ADC value after each conversion		*
 * Function job : the function is to take the adc value to convert it to a duty cycle for pwm	*
 * 					to control the motor and display it on the LCD.								*
 ************************************************************************************************/
void displaySpeed(uint16 a_analogVal) {
	pwm1.pwmDuty = ((float) a_analogVal / 1023) * 100;
	pwmChangeDuty(&pwm1);
	lcd_goToRowColumn(0, 7);
	lcd_integerToString(a_analogVal);
}
/************************************************************************************************
 * Function Name : interruptBehaviour															*
 * Function Return type : void																	*
 * Function Arguments/types : void																*
 * Arguments names/job : nothing																*
 * Function job : It is the callback function from the ISR of external interrupt0				*
 * 				  and reverse the motion of the motor by calling function(dcMotorRev)			*
 * 				  based on user configurations given in motor1.									*
 ************************************************************************************************/
void interruptBehaviour(void) {
	dcMotorRev(&motor1);
}
/************************************************************************************************
 * Function Name : main																			*
 * Function Return type : int																	*
 * Function Arguments/types : void																*
 * Arguments names/job : nothing																*
 * Function job : It is the main function for the project program								*
 * 																								*
 ************************************************************************************************/
int main(void) {
	ADC_DATA adc1 = { AREF, Enabled, Right, ADC0, _8, &displaySpeed };
	eInterruptData eint0 = { eINT1, ActiveRising, intPupDisabled,
			&interruptBehaviour };

	/****************************************************************************
	 * initializing all drivers for the project depending on user configs		*
	 *
	 ****************************************************************************/
	lcd_init(); //initializing LCD driver
	lcd_displayString("Speed: "); // giving constant string at the beginning of the program
	adc_init(&adc1); //initializing adc on configurations of adc1
	dcMotorInit(&motor1); // setting up a motor on motor1 configurations
	eInterruptInit(&eint0); // initializing external interrupts driver based on eint0 configurations
	pwmInit(&pwm1); // initializing pwm based on pwm1 configurations
	sei();
	//Enabling global interrupts

	while (1) {
		adc_start(&adc1); // starting adc conversion on adc1 configurations
	}
}
