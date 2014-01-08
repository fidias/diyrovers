#include "mbed.h"
#include "Display.h"

// This is for the Sparkfun module
#define DISPLAY_CLEAR     0x01
#define DISPLAY_SET_POS   0x08

Display::Display(void): 
        lcd(p17, p18, SD_FW),
        v(1, 20, 35, 20, 'V'),
        //a(11, 40, 15, 'A'),
        g1(22, 20, 35, 20, 'G'),
        g2(43, 20, 35, 20, 'H')
{
  // nothin to do
}

void Display::init()
{
    lcd.puts("test\n");
	lcd.clear();
    wait(0.3);
    
    // Initialize LCD graphics
    Bargraph::lcd = &lcd;   
    v.calibrate(6.3, 8.4);
    a.calibrate(0, 15.0);
    g1.calibrate(0, 10.0);
    g2.calibrate(4.0, 0.8);
    //GPSStatus g2(21, 12);
    //GPSStatus::lcd = &lcd;

}

void Display::status(const char *newstatus)
{
    int pad = 20 - strlen(newstatus);
    lcd.pos(0,1);
    lcd.puts(newstatus);
    while (pad--) lcd.putc(' ');
}

void Display::menu(const char *itemName)
{
    int pad = 14 - strlen(itemName);
    lcd.pos(0,0);
    lcd.puts("< ");
    lcd.puts(itemName);
    lcd.puts(" >");
    while (pad--) lcd.putc(' ');
}

void Display::select(const char *itemName)
{
    int pad = 14 - strlen(itemName);
    lcd.pos(0,0);
    lcd.puts(">> ");
    lcd.puts(itemName);
    while (pad--) lcd.putc(' ');
}

// display gauge at a given position (slot) along the bottom
void Display::gauge(const int slot)
{
}


void Display::update(SystemState *state) {
	if (state) {
		v.update(state->voltage);
		//a.update(state->current);
		g1.update((float) state->gpsSats);
		g2.update(state->gpsHDOP);

		lcd.rectFill(68,16,122,64, 0x00);
		lcd.circle(90, 40, 22, true);
		lcd.circle(90, 40, 14, true);
		lcd.posXY(90-9,40-(8/2)); // 3 * 6 / 2, char width=6, 5 chars, half that size
	    lcd.printInt((int) state->estHeading);
		int nx = 90 - 18 * sin(3.141529 * state->estHeading / 180.0);
		int ny = 40 - 18 * cos(-3.141529 * state->estHeading / 180.0);
		lcd.posXY(nx - 2, ny - 3);
	    lcd.putc('N');
		//lcd.circle(nx, ny, 5, true);
		int bx = 90 - 18 * sin(-3.141529 * (state->bearing-state->estHeading) / 180.0);
		int by = 40 - 18 * cos(-3.141529 * (state->bearing-state->estHeading) / 180.0);
		lcd.circle(bx, by, 2, true);
	}
}

 void Display::redecorate() {
    v.init();
    //a.init();
    g1.init();
    g2.init();
}


            
