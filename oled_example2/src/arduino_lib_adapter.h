/*
 	 arduino_lib_adapter.h

	Definition of stubs to fake out Arduino stuff
	Some are functional, some are just to satisfy the compiler without a million
	preprocessor defines.	We have a few hundred million more CPU cycles per second
	to play with than the low end Atmel chips.

	---

	This program is free software: you can redistribute it and/or modify it under the terms
	of the version 3 GNU General Public License as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program.
	If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ARDUINO_LIB_ADAPTER_H_
#define ARDUINO_LIB_ADAPTER_H_

// for uint8_t etc
#include <stdint.h>

// we'll substitute std::string for String
#include <string>

// malloc is used here and there...
#include <cstdlib>
#include <cstring>

/////////////////////////////////////////////////////////////////////////////////
typedef uint8_t byte;
typedef bool boolean;
typedef std::string String;
void delay(int milliseconds);	// used in OLED reset and display enable logic

enum PinMode { INPUT, OUTPUT };
void pinMode(byte whichPin, PinMode m); 		// does nothing

const bool LOW = false;
const bool HIGH = true;

/** goes to a case statement, looks at the pin and translates that to our
 * MicroZed GPIO port and pin.
 * Inefficient as hell, but minimizes changes in original library code.
 */
void digitalWrite(byte whichPin, bool v);

/** pass these to OLED constructor (which will use them later for digitalWrite) to get desired results */
enum FakePins
{
	fake_pin_ncs,
	fake_pin_dc,
	fake_pin_reset
};



class Print { }; // we have stdlib...


class SPI_PROXY
{
public:
	SPI_PROXY();

	void begin();
	void setBitOrder(int v);
	void setClockDivider(int d);

	byte transfer(byte v);

	/** Send as many bytes as you want.
	 * Not in Arduino, but we have different SPI hardware...
	 *	@param pV	pointer to the data
	 *	@param count	size of the data
	 */
	void transfer(const byte* pV, size_t count);
};
const int MSBFIRST = 1;

extern SPI_PROXY SPI;


// there's no Harvard division between prog/data on Zynq
#define PROGMEM
inline byte pgm_read_byte(const byte* p) { return *p; }

inline _PTR memcpy_P(_PTR dst, const _PTR src, size_t len) { return memcpy(dst, src, len); }


inline int random(int limit) { return rand()*(float)limit; }

#endif /* ARDUINO_LIB_ADAPTER_H_ */
