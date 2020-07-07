#include "mbed.h"

DigitalOut redLED(LED_RED);

DigitalOut greenLED(LED_GREEN);


int G_Led(DigitalOut& ledpin);
int R_Led(DigitalOut& ledpin);

int main()
{

   redLED = 1;
   greenLED = 1;

   while (true) {

       R_Led(redLED);
       G_Led(greenLED);

   }

}