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

void cobs_encode(byte *input, int length, byte *output)
{
  output[1] = input[0];
}

byte check_sum(byte *data, int length)
{
  byte sum = 0;
  for (int i = 0; i < length; i++)
  {
    sum += data[i];
  }
  return sum;
}

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
    state = 1023 - analogRead(a_switch_pin[i]); // if you delete "1023", analog input will invert
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

  byte body[4] = {0x03, 0xFF, 0xFF, 0}; // 0:COBS_HEAD, 1:DATA, 2:CHECKSUM, 3:COBS_END
  body[1] =
      (((pin & 0b00111111) << 1) |
       (state & 1));
  body[2] = check_sum(&body[1], 1);

  // COBS encode
  if (body[1] == 0 && body[2] == 0)
  {
    body[0] = 0x01;
    body[1] = 0x01;
    body[2] = 0x01;
  }
  // DATAが1バイトであるので、どちらかのみが0になるという状態はありえないため、記述を省略

  Serial.write(body, 4);

  return 0;
}

int send_a(int pin, int state)
{
  if (pin > 32)
  {
    return -1;
  }

  byte body[3] = {0, 0, 0};
  body[0] =
      (1 << 7) |
      ((pin & 0b00011111) << 2) |
      (state & 0b1100000000) >> 8;

  body[1] = (byte)state;

  body[2] = check_sum(body, 2);

  Serial.write(IDENTIFIER, 2);
  Serial.write(body, 3);
  Serial.write(&IDENTIFIER[2], 2);

  return 0;
}
