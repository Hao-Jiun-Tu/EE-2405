#include "mbed.h"
#include "mbed_rpc.h"

#define CENTER_BASE 1500
PwmOut servo_LEFT(D9);
PwmOut servo_RIGHT(D8);
PwmOut pin9(D9), pin8(D8);
parallax_ping  ping1(pin10);

DigitalIn encoder(D3);

RawSerial xbee(D12, D11);
Serial uart(D1, D0);
Serial pc(USBTX, USBRX);


Ticker encoder_ticker;
volatile int steps;
volatile int last;

int stop = 0;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
Thread t_logger;

void servo_control_LEFT(int speed);
void servo_control_RIGHT(int speed);
void encoder_control();
void _Stop();
void _Go_Straight();
void _Go_Back();
void _Turn_Left();
void _Turn_Right();
void _Bud_Turn_Left();
void _Bud_Turn_Right();
void classification();

void xbee_setting(void);
void xbee_rx_interrupt(void);
void xbee_rx(void);
void reply_messange(char *xbee_reply, char *messange);
void check_addr(char *xbee_reply, char *messenger);
void getLogger(void);

void servo_control_LEFT(int speed){
    if (speed > 200)       speed = 200;
    else if (speed < -200) speed = -200;
    servo_LEFT = (CENTER_BASE + speed)/20000.0f;
}

void servo_control_RIGHT(int speed){
    if (speed > 200)       speed = 200;
    else if (speed < -200) speed = -200;
    servo_RIGHT = (CENTER_BASE + speed)/20000.0f;
}

int dis = 0;
void encoder_control(){
    int value = encoder;
    if(!last && value){
        steps++;
        dis++;
    }
    last = value;
}
void _Stop() {
        servo_control_LEFT(0);
        servo_control_RIGHT(0);
        wait(0.5);
}

void _Go_Straight() {
        servo_control_LEFT(27);
        servo_control_RIGHT(-83);
}

void _Go_Back() {
        servo_control_LEFT(-27);
        servo_control_RIGHT(45);
}

void _Turn_Left() {
        servo_control_LEFT(-27);
        servo_control_RIGHT(-70); 
}

void _Turn_Right() {
        servo_control_LEFT(27);
        servo_control_RIGHT(65);
}

void _Bud_Turn_Left() {
        servo_control_LEFT(27);
        servo_control_RIGHT(70); 
}

void _Bud_Turn_Right() {
        servo_control_LEFT(-27);
        servo_control_RIGHT(-70); 
}

int main() {
    pc.baud(9600);
    xbee.baud(9600);
    uart.baud(9600);
    xbee_setting();

    servo_LEFT.period(.02);
    servo_RIGHT.period(.02);
    encoder_ticker.attach(&encoder_control, .001);
    steps = 0;
    last = 0;
    
    t_logger.start(getLogger);

    while(1) {
        if(dis <230){ //go straight
            _Go_Straight();
        }
        else if(dis >=230 && dis < 240){ //turn left
            if(!stop){
                _Stop(); 
                stop = !stop;
            }
            _Turn_Left();      
        }
        else if(dis >= 240 && dis <395){ //go straight to mission 1
            if(stop){
                _Stop();  
                stop = !stop;             
            }
            _Go_Straight();           
        }
        else if(dis >=395 && dis < 405){ //bud turn left
            if(!stop){
                _Stop();
                stop = !stop;
            }
            _Bud_Turn_Left();           
        } 
        else if(dis >=405 && dis < 455){ //go back
            if(stop){
                _Stop();
                stop = !stop;                
            }
            _Go_Back();           
        } 
        else if(dis >=455 && dis < 520){ //go straight
            if(!stop){
                _Stop();
                stop = !stop;
            }
            _Go_Straight();          
        }
        else if(dis >=520 && dis < 530){ //turn right
            if(stop){
                _Stop();
                stop = !stop;                
            }
            _Turn_Right();         
        }
        else if(dis >=530 && dis < 610){
            if(!stop){
                _Stop(); 
                stop = !stop;
            }
            _Go_Straight();           
        } 
        else if(dis >=610 && dis < 620){ //bud turn right
            if(stop){
                _Stop(); 
                stop = !stop;              
            }
            _Bud_Turn_Right();          
        }
        else if(dis >=620 && dis < 640){ //go back to snapshot
            if(!stop){
                _Stop();
                stop = !stop;
            }
            _Go_Back();
        }
        else if(dis >=640 && dis < 660){ //go straight
            if(stop){
                _Stop();
                wait(7);
                stop = !stop;
            }
            if(uart.readable()){
                char recv = uart.getc();
                xbee.putc(recv);
            }
            _Go_Straight();
        }
        else if(dis >=660 && dis < 670){ //turn right
            if(stop){
                _Stop();
                stop = !stop;
                wait(5);
            }
            _Turn_Right();          
        } 
        else if(dis >=670 && dis < 720){ //go straight
            if(!stop){
                _Stop(); 
                stop = !stop;               
            }
            _Go_Straight();          
        } 
        else if(dis >= 720 && dis < 730){ //turn right
            if(stop){
                _Stop(); 
                stop = !stop;
            }
            _Turn_Right(); 
        }
        else if(dis >= 730 && dis < 930){ //go straight mission 2
            if(!stop){
                _Stop();  
                stop = !stop;             
            }
            _Go_Straight(); 
        }
        else if(dis >= 930 && dis < 940){ //turn right
            if(stop){
                _Stop();
                stop = !stop;
            }
            _Turn_Right(); 
        }
        else if(dis >= 940 && dis < 1085) { //go straight
            if(!stop){
                _Stop();
                stop = !stop;               
            }
            _Go_Straight();  
        }
        else if(dis >=1085 && dis < 1095){ //turn right
            if(stop){
                _Stop();
                stop = !stop;
            }
            classification();
            _Turn_Right();          
        }
        else if(dis >=1095 && dis < 1400){ //go straight
            if(!stop){
                _Stop();  
                stop = !stop;             
            }
            _Go_Straight();   
        }
        else{
            servo_control_LEFT(0);
            servo_control_RIGHT(0);
        }
        wait(0.01);

    }
    
}

void getLogger(){
    while(1) {
        if(dis < 230)  xbee.printf("Go Straight to Start Mission 1\r\n");          
        else if(dis >= 230 && dis < 240) xbee.printf("Turn Left\r\n");     
        else if(dis >= 240 && dis < 395)  xbee.printf("Go Straight to Mission 1\r\n");    
        else if(dis >= 395 && dis < 405)  xbee.printf("Adjusting the Car Position\r\n");         
        else if(dis >= 405 && dis < 455)  xbee.printf("Parking!\r\n");    
        else if(dis >= 455 && dis < 520)  xbee.printf("Go Straight\r\n");  
        else if(dis >= 520 && dis < 530)  xbee.printf("Turn right\r\n"); 
        else if(dis >= 530 && dis < 610)  xbee.printf("Go Straight\r\n");    
        else if(dis >= 610 && dis < 620)  xbee.printf("Turn Left\r\n");
        else if(dis >= 620 && dis < 640)  xbee.printf("Go Back to Snapshot!\r\n");                 
        else if(dis >= 640 && dis < 730)  xbee.printf("Leave Mission 1\r\n"); 
        else if(dis >= 730 && dis < 930)  xbee.printf("Go Straight to Start Mission 2\r\n");    
        else if(dis >= 930 && dis < 940)  xbee.printf("Turn Right\r\n");      
        else if(dis >= 940 && dis < 1000)  xbee.printf("Go Straight to Mission 2\r\n");
        else if(dis >= 1000 && dis < 1070)  xbee.printf("Adjusting the Car Position\r\n");
        else if(dis >= 1070 && dis < 1082)  xbee.printf("Start Distinguishing the Object!\r\n");
        else if(dis >= 1082 && dis < 1500)  xbee.printf("Leave Mission 2\r\n");     
        else {
            xbee.printf("No Action\r\n");
            break;
        }
        wait(1);
    }
}

void classification()
{
    state = 2;
    float ping_dis[4];

    for(int i = 0; i < 4; i ++){
      ping_dis[i] = float(ping1);
      wait(0.25);
    }

    if(ping_dis[0] > ping_dis[1] && ping_dis[2] < ping_dis[3]) { //triangle
      xbee.printf("Triangle!!\r\n");
    }else if(ping_dis[0] == ping_dis[1] && ping_dis[1] == ping_dis[2] && ping_dis[2] == ping_dis[3]) { //rectangle
      xbee.printf("Rectangle!!\r\n");
    }else if(ping_dis[0] > ping_dis[1] && ping_dis[1] > ping_dis[2] && ping_dis[2] > ping_dis[3]) { //right triangle
      xbee.printf("Right Triangle!!\r\n");
    }else {
      xbee.printf("Tooth Pattern!!\r\n");
    }
    wait(2);
}

void xbee_setting(){
  // XBee setting
  char xbee_reply[4];
  xbee.baud(9600);
//   xbee.printf("+++");
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){
    pc.printf("enter AT mode.\r\n");
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
  }

//   xbee.printf("ATMY 0x223\r\n");
//   reply_messange(xbee_reply, "setting MY : 0x223");

//   xbee.printf("ATDL 0x123\r\n");
//   reply_messange(xbee_reply, "setting DL : 0x123");

//   xbee.printf("ATID 0x1\r\n");
//   reply_messange(xbee_reply, "setting PAN ID : 0x1");

//   xbee.printf("ATWR\r\n");
//   reply_messange(xbee_reply, "write config");

//   xbee.printf("ATMY\r\n");
//   check_addr(xbee_reply, "MY");

//   xbee.printf("ATDL\r\n");
//   check_addr(xbee_reply, "DL");

//   xbee.printf("ATCN\r\n");
//   reply_messange(xbee_reply, "exit AT mode");
  xbee.getc();

  // start
  pc.printf("start\r\n");
  t.start(callback(&queue, &EventQueue::dispatch_forever));

  // Setup a serial interrupt function to receive data from xbee
  xbee.attach(xbee_rx_interrupt, Serial::RxIrq);
}

void xbee_rx_interrupt(void)
{
  xbee.attach(NULL, Serial::RxIrq); // detach interrupt
  queue.call(&xbee_rx);
}

void xbee_rx(void)
{
  char buf[100] = {0};
  char outbuf[100] = {0};
  while(xbee.readable()){
    for (int i=0; ; i++) {
      char recv = xbee.getc();
      if (recv == '\r') {
        break;
      }
      buf[i] = pc.putc(recv);
    }
    RPC::call(buf, outbuf);
    pc.printf("%s\r\n", outbuf);
    wait(0.1);
  }
  xbee.attach(xbee_rx_interrupt, Serial::RxIrq); // reattach interrupt
}

void reply_messange(char *xbee_reply, char *messange){
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();
    if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
    pc.printf("%s\r\n", messange);
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
    xbee_reply[2] = '\0';
    }
}

void check_addr(char *xbee_reply, char *messenger){
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();
    xbee_reply[3] = xbee.getc();
    pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
    xbee_reply[2] = '\0';
    xbee_reply[3] = '\0';
}
