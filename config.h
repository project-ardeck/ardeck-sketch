#ifndef CONFIG_H
#define CONFIG_H

#define VALUEOFDIGITAL 8              //Number of buttons -1 
#define VALUEOFANALOG 6               //Number of potentiometer -1 
#define BAUD 19200

extern int button_stat[VALUEOFDIGITAL + 1][4] = {
  { 2,1,0,0},		 //button ID (pin number), button use or not, button ON/OFF assignment, binary number to send
  { 3,1,0,0},        //Example.{2,1,0,0}->Use pin-2, button available, button OFF, button status preservation.
  { 4,1,0,0},
  { 5,1,0,0},
  { 6,1,0,0},
  { 7,1,0,0},
  { 8,1,0,0},
  { 9,1,0,0},
  {10,1,0,0},
}; 


extern int analog_stat[VALUEOFANALOG][4]={  
  {A1,1,0,0},         // pin number, pin use or not, pin value (voltage) assignment, binary number to send
  {A0,1,0,0},
  {A2,1,0,0},
  {A3,1,0,0},
  {A4,1,0,0},
  {A5,1,0,0},
};

extern int switchmode = 0 ;      //normal open/colse (open:1, close:0)


#endif
