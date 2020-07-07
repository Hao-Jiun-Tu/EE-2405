#include "mbed.h"

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x0};
//0x0 stands for all the displaying are dark 
Serial pc( USBTX, USBRX );

AnalogIn Ain(A0);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
DigitalOut dot(D8); //Decimal point

DigitalIn  Switch(SW3);
AnalogOut Aout(DAC0_OUT);

float ADCdata;

Timer t;
float cur_t;
float interval = 1/*1(s)*/;
//if there are 20 times of waves in 1(s), that stands for 20HZ. 

int wave_state = 0; //state = 1 means sampling sine wave in the "upper half"
                    // state = 0 means sampling sine wave in the "lower half"
int counter = 0; //is to count times of the state transitions 

int frequency = 0;
//frequency splits into three corresponding digits
int freq0 = 0;
int freq1 = 0;
int freq2 = 0;

int main(){

    redLED = 1; //redLED is dark
    greenLED = 0; //greenLED is bright

    t.start();
    cur_t = t.read();

    while(t.read() - cur_t <= interval){

        ADCdata = Ain;

        if((ADCdata >= 0.33) && (wave_state == 0)){
            counter++;
            wave_state = 1;
        }else if((ADCdata <= 0.33) && (wave_state == 1)){
            counter++;
            wave_state = 0;
        }
    }

    frequency = counter / 2;
    freq2 = frequency / 100;
    freq1 = (frequency % 100) / 10;
    freq0 = frequency % 10;

    int sample = 512;
    float ADC_data[512];
    for (int i = 0; i < sample; i++){
        ADC_data[i] = Ain;
        wait(1./sample);
    }

    for (int i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADC_data[i]);
        //wait(0.1);
    }


    while(1){
        //The following is displaying the frequency on the seven_segment.
        if(Switch == 0){ //When switch is pushed...
            redLED = 0; //redLED is bright
            greenLED = 1; //greenLED is dark

            display = table[freq2];
            wait(1);

            display = table[freq1];
            wait(1);

            display = table[freq0];
            dot = 1; //Decimal point is bright
            wait(1);

            display = table[11]; //all dark
            wait(1);

        }else{
            redLED = 1; //redLED is dark
            greenLED = 0; //greenLED is bright
        }
        
        redLED = 1; //redLED is dark
        greenLED = 0; //greenLED is bright
      
        double pi = 3.141592653589;
        double w = 2*pi*frequency; //w = 2*pi*f
        
        for(float k = 0; k < 2; k += 0.0001){
            Aout =  0.5+0.5*sin(w*k);
            wait(0.0001);
        }
        
    }

}
