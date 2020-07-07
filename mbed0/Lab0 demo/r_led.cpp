#include "mbed.h"

void R_Led(DigitalOut& ledpin)
{

   for(int i=0; i<6; ++i) { //blink for 6 times

       ledpin = !ledpin; // toggle led

       wait(0.5f);

   }

}