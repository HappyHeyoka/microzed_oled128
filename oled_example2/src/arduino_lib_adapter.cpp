/*
  arduino_lib_adapter.cpp

 	implementation of a very lightweight shim for Arduino run-time routines
 ---

 This program is free software: you can redistribute it and/or modify it under the terms
 of the version 3 GNU General Public License as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program.
 If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <sleep.h>

#include "microzed_oled_io.h"

#include "arduino_lib_adapter.h"

SPI_PROXY SPI = SPI_PROXY();

SPI_PROXY::SPI_PROXY()
{
}

void SPI_PROXY::begin()
{
	// do nothing; initialize the SPI stuff elsewhere
}
void SPI_PROXY::setBitOrder(int v)
{
	// do nothing; initialize the SPI stuff elsewhere
}
void SPI_PROXY::setClockDivider(int d)
{
	// do nothing; initialize the SPI stuff elsewhere
}

byte SPI_PROXY::transfer(byte v)
{
	return ftoled_spi_transfer(v);
}

void SPI_PROXY::transfer(const byte* pV, size_t count)
{
	ftoled_spi_transfer(pV, count);
}

void pinMode(byte whichPin, PinMode m)
{

}


void digitalWrite(byte whichPin, bool v)
{
	switch (whichPin)
	{
		case fake_pin_ncs:		// ignore - SPI hardware on Zynq handles CS for us
		case fake_pin_dc:		set_ftoled_DC(v);  break;
		case fake_pin_reset:	set_ftoled_RST(v);  break;
	}
}

void delay(int milliseconds)
{
	usleep(milliseconds*1000);
}
