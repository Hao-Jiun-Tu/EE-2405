// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library

//

#include "mbed.h"

#include "uLCD_4DGL.h"

PwmOut PWM1(D6);


uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;

int x = 0;
int flag;

int main()

{

    // basic printf demo = 16 by 18 characters on screen

    uLCD.printf("\n\n\n107060011\n"); //Default Green on black text

    //uLCD.printf("\n  Starting Demo...");
    //uLCD.line(int x1, int y1 , int x2, int y2, int color)
    uLCD.line(0,  0 ,  0,  16, RED);
    uLCD.line(0,  0 ,  16,  0, RED);
    uLCD.line(16,  16 ,  0,  16, RED);
    uLCD.line(16,  16 ,  16,  0, RED);


    // uLCD.text_width(4); //4X size text

    // uLCD.text_height(4);

    // uLCD.color(RED);

    // for (int i=10; i>=0; --i) {

    //     uLCD.locate(1,2);

    //     uLCD.printf("%2D",i);

    //     wait(.5);

    // }
    //     if(x == 0)
    //     flag = 1;
    // else if(x == 1)
    //     flag = 0;
    
    // if(flag = 1)
    //     x += 0.1;
    // else
    //     x -= 0.1;
        
    // PWM1.period(x);

    // PWM1 = 2;
      PWM1.period(0.001);

    PWM1 = 0.5;

}
