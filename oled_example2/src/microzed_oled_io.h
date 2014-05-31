/*
	microzed_oled_io.h

	---

	This program is free software: you can redistribute it and/or modify it under the terms
	of the version 3 GNU General Public License as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program.
	If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MICROZED_OLED_IO_H_
#define MICROZED_OLED_IO_H_

#include <stdint.h>

/** call once to initialize SPI and GPIO hardware for driving the display.
 * @return true if success
 */
bool ftoled_init();


/**	transfer a single byte each way
 *	@param data		byte to sent
 *	@return received byte
 */

uint8_t ftoled_spi_transfer(uint8_t data);

/** Send as many bytes as you want.
 *	@param pV	pointer to the data
 *	@param count	size of the data
 */
void ftoled_spi_transfer(const uint8_t* pV, size_t count);

void set_ftoled_RST(bool v);
void set_ftoled_DC(bool v);


#endif /* MICROZED_OLED_IO_H_ */
