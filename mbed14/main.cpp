#include"mbed.h"
Serial pc(USBTX,USBRX); //tx,rx
Serial uart(D12,D11); //tx,rx
int main(){
   uart.baud(9600);
   while(1){
      if(uart.readable()){
            char recv = uart.getc();
            pc.putc(recv);
      }
   }
}
