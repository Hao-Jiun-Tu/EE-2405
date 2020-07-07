#include "mbed.h"

void G_Led(DigitalOut& ledpin)
{

   for(int i=0; i<4; ++i) { //blink for 4 times

       ledpin = !ledpin; // toggle led

       wait(0.5f);

   }

}