// ardeck-sketch.ino  This sketch makes the arduino like a stream deck. By using button/potentiometer etc... you can control about your pc(example:Volume control,Keyboard shortcut).
// Copyright (C) 2024 project-ardeck 

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "config.h"



char identifier[4]={'A', 'D', 'E', 'C'};                     //Identifier. Sent at each Serial.write.


void setup() {
  for(int i=0;i<=VALUEOFDIGITAL;i++){                         //PinMode switching of //digital pins
    pinMode(button_stat[i][0], INPUT_PULLUP);               
  }

   for(int i=0;i<=VALUEOFANALOG;i++){                            // pinMode switching of analog pins
     pinMode(analog_stat[i][0],INPUT_PULLUP);
   }

    Serial.begin(BAUD);
}

void loop() {

  for(int i=0;i<=VALUEOFDIGITAL;i++){
    button_stat[i][2] = digitalRead(button_stat[i][0]);
  }

  for(int i=0;i<=VALUEOFANALOG;i++){
    analog_stat[i][2] = analogRead(analog_stat[i][0]);
  }



  for(int i=0;i<=VALUEOFDIGITAL;i++){
    button_stat[i][3]=B00000010 + (i << 1);
    if(button_stat[i][2] == switchmode){                              //Normal open/close switching.
      button_stat[i][3] = button_stat[i][3] + 1 ;
    }
    if(button_stat[i][1] == 1){ 
      func_d(button_stat[i][3]);
    }
  }
  for(int i=0;i<VALUEOFANALOG;i++){
    func_a(analog_stat[i][2],i);
  }
}

//デジタル値をアプリに送信
int func_d(byte array_d){             // Functionalize digital transmission
  Serial.print(identifier[0]);
  Serial.print(identifier[1]);
  Serial.write(array_d);
  Serial.print(identifier[2]);
  Serial.print(identifier[3]);
}

//アナログ値をアプリに送信
int func_a(int stat_res,int num_pin){
  byte array_res[2]={0,0};
  array_res[1] = stat_res;         //Lower 8-bit input
  array_res[0] = stat_res >> 8;    //Higher 2 bits input
  array_res[0] = array_res[0] | (B10000000) ;          // Assign 1 for AD determination
  array_res[0] = array_res[0] | (num_pin << 2);
  Serial.print(identifier[0]);
  Serial.print(identifier[1]);
  Serial.write(array_res,2);     // Combine 8 bits and 2 bits and output
  Serial.print(identifier[2]);
  Serial.print(identifier[3]);
}
