/*
	microzed_oled_io.cpp

 	 initialize IO and support 'shim' for Arduino library stuff.

	 ---

	 This program is free software: you can redistribute it and/or modify it under the terms
	 of the version 3 GNU General Public License as published by the Free Software Foundation.

	 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	 See the GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License along with this program.
	 If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <iomanip>

#include "xparameters.h"// BSP stuff

#include "xgpio.h"	// PL AXI GPIO support
#include "xspips.h"	// PS SPI as opposed to PL

#include "microzed_oled_io.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////

/** chip select pin out from Zynq to OLED128 display - we're using SPI_0_SS_O */
const uint8_t OLED128_OLED_SPI_CS = 0;

/** chip select pin out from Zynq to OLED128 SD Card - we're using SPI_0_SS1_O */
const uint8_t OLED128_SD_SPI_CS = 1;

/*
 * The following constants define the AXI GPIO Channels
 */
const uint32_t OLED_CONTROL_CHANNEL = 1;


/// OLED128 Card Detect status bit on GPIO 1, OLED_STATUS_CHANNEL
const uint32_t OLED_SD_CD = 1;

/// OLED128 Data/Command control bit on GPIO 1, OLED_CONTROL_CHANNEL
const uint32_t OLED_DC = 1;
/// OLED128 Reset bit on GPIO 1, OLED_CONTROL_CHANNEL
const uint32_t OLED_RST = 2;
/// OLED128 Chip Select control bit
const uint32_t OLED_CS = 4;

XGpio gpio_oled_control; // test GPIO for r/m/w output


bool init_ftoled_spi();
bool initPL_GPIO();


bool ftoled_init()
{
	bool success = initPL_GPIO();
	if (success)  success = init_ftoled_spi();
	return success;
}


///////////////////////////////////////////////////////////////////////////
//	AXI GPIO setup for handshaking signals
///////////////////////////////////////////////////////////////////////////

bool init_AXI_GPIO(XGpio& which, uint16_t deviceID)
{
	bool sucess = true;
	/*
	 * Initialize the GPIO driver(s)
	 */
	XStatus status = XGpio_Initialize(&which, deviceID);
	if (status != XST_SUCCESS)
	{
		cout << "couldn't initialise GPIO 0 (" << status << ')' << endl;
		sucess = false;
	}

	return sucess;
}

bool initPL_GPIO()
{
	bool success = init_AXI_GPIO(gpio_oled_control, XPAR_AXI_GPIO_OLED_CONTROL_DEVICE_ID);

	// bits set to '0' are outputs, '1' for inputs
	XGpio_SetDataDirection(&gpio_oled_control, OLED_CONTROL_CHANNEL, 0x00000000);

	return success;
}

uint32_t getOledControl()
{
	return XGpio_DiscreteRead(&gpio_oled_control, OLED_CONTROL_CHANNEL);
}

template <uint32_t MASK> void set_oled_control(bool f)
{
	if (f)
		XGpio_DiscreteSet(&gpio_oled_control, OLED_CONTROL_CHANNEL, MASK);
	else
		XGpio_DiscreteClear(&gpio_oled_control, OLED_CONTROL_CHANNEL, MASK);
}


void set_ftoled_CS(bool f)
{
//	set_oled_control<OLED_CS>(f);
//	don't set manually - spi hardware does it
}

void set_ftoled_DC(bool f)
{
	set_oled_control<OLED_DC>(f);
}

void set_ftoled_RST(bool f)
{
	set_oled_control<OLED_RST>(f);
}



///////////////////////////////////////////////////////////////////////////
//	PS SPI setup for sending data to the OLED128 display
///////////////////////////////////////////////////////////////////////////

/**	SPI instance for FTOLED
 */
XSpiPs spi_oled;

uint8_t ReadBuffer[4];
uint8_t WriteBuffer[4];

/** initialise the SPI for driving the FTOLED128 display
 * @return true if succeeded
 */
bool init_ftoled_spi()
{
	bool success = false;

	cout << "initialising SPI" << endl;
	//
	// Initialize the SPI driver so that it is  ready to use.
	//
	XSpiPs_Config *pConfig = XSpiPs_LookupConfig(XPAR_PS7_SPI_0_DEVICE_ID);// device id from 'xparameters.h'

	if (pConfig == NULL)
	{
		cout << "XSpiPs_LookupConfig failed " << endl;
	}
	else
	{
		XStatus status = XSpiPs_CfgInitialize(&spi_oled, pConfig, pConfig->BaseAddress);

		if (status != XST_SUCCESS)
		{
			cout << "XSpiPs_CfgInitialize failed - status=" << status << endl;
		}
		else
		{
			//
			// Perform a self-test to ensure that the hardware was built correctly.
			//
			status = XSpiPs_SelfTest(&spi_oled);

			if (status != XST_SUCCESS)
			{
				cout << "XSpiPs_SelfTest failed - status=" << status << endl;
			}
			else
			{
				//
				// Set the SPI device as a master with manual start and manual
				// chip select mode options
				//
				XSpiPs_SetOptions(&spi_oled, XSPIPS_MANUAL_START_OPTION | XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION);

				//
				// Set the SPI device pre-scaler to divide by 8
				//

				XSpiPs_SetClkPrescaler(&spi_oled, XSPIPS_CLK_PRESCALE_8);	// ~20MHz
//				XSpiPs_SetClkPrescaler(&spi_oled, XSPIPS_CLK_PRESCALE_16);	// ~10MHz

				cout << "SPI init completed" << endl;

				success = true;
			}
		}
	}

	return success;
}


uint8_t ftoled_spi_transfer(uint8_t v)
{
	WriteBuffer[0] = v;
	ReadBuffer[0] = 0;

	// set the address of our slave
	XSpiPs_SetSlaveSelect(&spi_oled, OLED128_OLED_SPI_CS);

	// blocking transfer
	XSpiPs_PolledTransfer(&spi_oled, WriteBuffer, ReadBuffer, 1);

	return ReadBuffer[0];
}

void ftoled_spi_transfer(const uint8_t* pV, size_t count)
{
	// set the address of our slave
	XSpiPs_SetSlaveSelect(&spi_oled, OLED128_OLED_SPI_CS);

	// blocking transfer
	XSpiPs_PolledTransfer(&spi_oled, const_cast<uint8_t*>(pV), NULL, count); // @TODO post a bug on tx pointer being non-const
}



