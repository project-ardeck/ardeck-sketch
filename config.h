#ifndef CONFIG_H
#define CONFIG_H

#define VALUEOFDIGITAL 8              //Number of buttons -1 
#define VALUEOFANALOG 6               //Number of potentiometer -1 
#define BAUD 19200

//ボタンの設定を格納
extern int button_stat[VALUEOFDIGITAL + 1][4] = {
  { 2,1,0,0},		 
  { 3,1,0,0},        
  { 4,1,0,0},
  { 5,1,0,0},
  { 6,1,0,0},
  { 7,1,0,0},
  { 8,1,0,0},
  { 9,1,0,0},
  {10,1,0,0},
}; 

//VRの設定を格納(ピン番号、ピンの使用（1:y 0:n）電圧格納、送信する二進数を格納)
extern int analog_stat[VALUEOFANALOG][4]={  
  {A1,1,0,0},         
  {A0,1,0,0},
  {A2,1,0,0},
  {A3,1,0,0},
  {A4,1,0,0},
  {A5,1,0,0},
};

extern int switchmode = 0 ;      //ノーマリーオープン　(オープン:1, クローズ:0)


#endif
