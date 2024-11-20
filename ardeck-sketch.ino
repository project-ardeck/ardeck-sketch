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

const char IDENTIFIER[4] = {'A', 'D', 'E', 'C'};

void setup()
{
  // Init of digital pin
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    pinMode(d_switch_pin[i][0], INPUT_PULLUP);
  }

  Serial.begin(BAUD_RATE);
}

void loop()
{
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    int state = 0;
    state = digitalRead(d_switch_pin[i][0]);
    if (d_switch_pin[i][1] == 0)
    {
      state ^= 1;
    }
    send_d(d_switch_pin[i][0], state);

    delay(SEND_INTERVAL);
  }

  for (int i = 0; i < NUMBER_OF_A_SWITCH; i++)
  {
    int state = 0;
    state = analogRead(a_switch_pin[i]);
    send_a(a_switch_pin[i], state);
    
    delay(SEND_INTERVAL);
  }
}

int send_d(int pin, int state)
{
  if (pin > 64)
  {
    return -1;
  }

  byte body = 0;
  body =
    ((pin & 0b00111111) << 1) |
    (state & 1);

  Serial.write(IDENTIFIER, 2);
  Serial.write(body);
  Serial.write(&IDENTIFIER[2], 2);

  return 0;
}

int send_a(int pin, int state)
{
  if (pin > 32)
  {
    return -1;
  }

  byte body[2] = {0, 0};
  body[0] =
    (1 << 7) |
    ((pin & 0b00011111) << 2) |
    (state & 0b1100000000) >> 8;

  body[1] = state & 0xFF;

  Serial.write(IDENTIFIER, 2);
  Serial.write(body, 2);
  Serial.write(&IDENTIFIER[2], 2);

  return 0;
}
