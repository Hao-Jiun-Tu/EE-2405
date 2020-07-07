#include "mbed.h"

#define CENTER_BASE 1500

Serial pc(USBTX, USBRX);
DigitalIn encoder(D10);
PwmOut servo(D11);

Timer T1;
Timer T2;
Timer t;
Ticker encoder_ticker;

volatile int steps;
volatile int last;

void servo_control(int speed){
    if (speed > 200)       speed = 200;
    else if (speed < -200) speed = -200;

    servo=(CENTER_BASE + speed)/20000.0f;
}

void encoder_control(){
    int value = encoder;
    if(!last && value) steps++;
    last = value;
}


int main() {

    pc.baud(9600);

    encoder_ticker.attach(&encoder_control, .01);

    servo.period(.02);

    T1.start();
    float t1 = T1.read();
    while(t1 < 8.000) {
    //TODO: revise this value according to your result
    servo_control(-22.273);

    steps = 0;
    t.reset();
    t.start();

    wait(1);
    t1 = T1.read();
    float time = t.read();

    pc.printf("T1:%1.3f  %1.3f\r\n", t1-2, (float)steps*6.5*3.14/32/time);
    }
    T2.start();
    float t2 = T2.read();
    while(t2 < 5.000) {
    //TODO: revise this value according to your result
    servo_control(44.426);

    steps = 0;
    t.reset();
    t.start();

    wait(1);
    t2 = T2.read();
    float time = t.read();

    pc.printf("T2:%1.3f  %1.3f\r\n", t2, (float)steps*6.5*3.14/32/time);
    
    }
    servo_control(0.000);
}