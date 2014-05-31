/*
	user_led.h

	setup PS GPIO for blinking LED and reading button on MicroZed

	---

	This program is free software: you can redistribute it and/or modify it under the terms
	of the version 3 GNU General Public License as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program.
	If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef USER_LED_H_
#define USER_LED_H_

/// initialise PS GPIO peripheral
bool initUsrLED();

/// set the LED on or off
void setUsrLED(bool v);

/// toggle the LED state
void toggleUsrLED();

/// read the button on the MicroZed
bool getUsrButton();


#endif /* USER_LED_H_ */
