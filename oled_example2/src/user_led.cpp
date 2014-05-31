/*
	user_led.cpp

	initialise IO for User LED on MicroZed board.
	---

	This program is free software: you can redistribute it and/or modify it under the terms
	of the version 3 GNU General Public License as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program.
	If not, see <http://www.gnu.org/licenses/>.
*/


#include "xgpiops.h" //
#include "user_led.h"





// Define the output pin we want to toggle. In this case it is pin 8 within the bank of 10, MIO 47:
#define pin_usrLED 47
#define pin_usrButton 51

// Declare the driver instance for MicroZed LED i/o
XGpioPs Gpio_usrled;

void setUsrLED(bool v)
{
	// Write the desired output value to the GPIO pin:
	XGpioPs_WritePin(&Gpio_usrled, pin_usrLED, v ? 1 : 0);
}


void toggleUsrLED()
{
	int v = XGpioPs_ReadPin(&Gpio_usrled, pin_usrLED);
	XGpioPs_WritePin(&Gpio_usrled, pin_usrLED, v ^ 1);
}

bool getUsrButton()
{
	return XGpioPs_ReadPin(&Gpio_usrled, pin_usrButton);
}

bool initUsrLED()
{
	bool success = false;

	// Configure the GPIO and define the status and pointer variables required for initialization within the function you wish to use:
	XGpioPs_Config *GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);

	XStatus status = XGpioPs_CfgInitialize(&Gpio_usrled, GPIOConfigPtr, GPIOConfigPtr->BaseAddr);

	if (status == XST_SUCCESS)
	{
		// Set the direction of the GPIO pin and enable the output:

		XGpioPs_SetDirectionPin(&Gpio_usrled, pin_usrLED, 1);
		XGpioPs_SetOutputEnablePin(&Gpio_usrled, pin_usrLED, 1);

		XGpioPs_SetDirectionPin(&Gpio_usrled, pin_usrButton, 0x0);

		toggleUsrLED();

		success = true;
	}

	return success;
}
