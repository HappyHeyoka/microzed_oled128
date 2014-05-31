/*
	Example to drive FTOLED128 from MicroZed.

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
#include <string>
#include <sstream>
#include <algorithm>

//#include "xparameters.h"

//#include "ps7_init.h" -- if compiled outside of SDK

// project stuff
#include "user_led.h"

#include "microzed_oled_io.h"

#include "FTOLED/FTOLED.h"
#include "FTOLED/fonts/SystemFont5x7.h"
#include "FTOLED/fonts/Arial14.h"
#include "FTOLED/fonts/Arial_Black_16.h"
#include "FTOLED/fonts/Droid_Sans_36.h"

using namespace std;



//////////////////////////////////////////////////////////////////////////////////////////////
// interrupt driven timer	-	not fitted this version


//clock_t last = 0;
//
//void init_timing()
//{
//	last = clock();
//}
//
//void test_timing()
//{
//	clock_t now = clock();
//	double d = now - last;
//	d = d / double(CLOCKS_PER_SEC);
//
//	cout << "elapsed time = " << d << " seconds"<< endl;
//	last = now;
//}

//////////////////////////////////////////////////////////////////////
void test_oled1(OLED& oled)
{
  //oled.begin();

  // Fill the background with dark red (the values in the curly braces are {RED,GREEN,BLUE} )
  Colour background = {10,0,0};
  oled.fillScreen(background);

  // Horizontal blue line from (30,30) to (90,30)
  oled.drawLine(30,30,90,30,ROYALBLUE);

  // Diagonal black line across the screen
  oled.drawLine(0,127,127,0,BLACK);

  // Draw a filled circle and then an outline of a circle around it
  oled.drawFilledCircle(64,64,12,CYAN);
  oled.drawCircle(64,64,18,WHITE);

  // Draw a yellow box in the top-right
  oled.drawFilledBox(90,90,120,120,YELLOW);

  // With an outline of a violet box underneath it
  oled.drawBox(90,50,120,80,5,VIOLET);
}

#undef str	// from 'xpseudo_asm_gcc.h'

/// basically original 'fonts demo' example
void test_oled2(OLED& oled)
{
	const uint64_t tStart = 0; //getMicroseconds();

	  // Fill the background with dark red (the values in the curly braces are {RED,GREEN,BLUE} )
	  Colour background = {0,10,0};
	  oled.fillScreen(background);

	  const char *ARIAL_BLACK_16 = "Arial Black 16";
	const char *ARIAL14 = "Arial 14";
	const char *SYSTEM_5_7 = "System 5x7\nOn Two Lines";
	const char *DS_36 = "DS 36";

	oled.selectFont(Arial14);
	oled.drawString(6,20,ARIAL14,VIOLET,BLACK);

	oled.selectFont(Arial_Black_16);
	oled.drawString(6,38,ARIAL_BLACK_16,WHITE,BLACK);

	oled.selectFont(SystemFont5x7);
	oled.drawString(6,70,SYSTEM_5_7,RED,BLACK);

	oled.selectFont(Droid_Sans_36);
	oled.drawString(6,78,DS_36,GREEN,BLACK);

	const uint64_t tEnd = 1; //getMicroseconds();

	// Finally, use the String object to build a dynamic string
	// instead of the previous strings which were all constant
	// "character arrays" (always the same value)
	ostringstream ss;

	ss << "Sketch ran in" << endl << (tEnd-tStart) << "microseconds"; // <<getMicroseconds()/1000 << " ms";
	oled.selectFont(SystemFont5x7);
	oled.drawString(6,8,ss.str(),WHITE,BLACK);
}

void sprite_loop(OLED& oled);
void moodlight_loop(OLED& oled);

//////////////////////////////////////////////////////////////////////

OLED oled128(fake_pin_ncs, fake_pin_dc, fake_pin_reset);

const uint64_t ledPeriod = 1000; //1000000;
uint64_t ledCounter = ledPeriod;

void loop()
{
//	if (ledCounter != 0)	-- write to the display as fast as possible - LED toggles each loop
//		ledCounter -= 1;
//	else
	{
		ledCounter = ledPeriod;

		// toggle MicroZed led
		toggleUsrLED();
		test_oled2(oled128);
		//sprite_loop(oled128);
		//moodlight_loop(oled128);
	}
}

void setup()
{
	initUsrLED();

	ftoled_init();

	oled128.begin();
}

int main()
{
    // ps7_init(); -- outside SDK, use this function from sources from generated hw platform

	cout << "starting FTOLED128 example" << endl;

	setup();

	while (1) loop();

	return 0;
}
