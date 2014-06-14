#ifndef __Display_H
#define __Display_H

#include "devices.h"
#include "SystemState.h"

class Display {
public:
    /** create new display instance
     *
     */
    Display(void);

    /** initialize the display
     *
     */
    void init(void);

    /** display a status string
     * @param st is the status string to display
     */
    void status(const char *st);

    /** display a menu item
     * @param itemName is the itemName to display
     */
    void menu(const char *itemName);

    /** display a selected menu item
     * @param itemName is the itemName to display
     */
    void select(const char *itemName);

    /** display gauge at a given position (slot) along the bottom
     */
    void gauge(int slot);

    /** updates data on the display */
    void update(SystemState *state);
    
    /** initializes the update display */
    void redraw(void);
//private:
//    SerialTextLCD _lcd;
};        

#endif
