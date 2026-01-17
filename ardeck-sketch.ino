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

/// @brief ボディーデータの開始インデックスと長さを渡すと、その合計値を1バイトで返します。
/// @param data 配列のポインタ
/// @param length 長さ
/// @return １バイトの合計値
byte check_sum(byte *data, int length)
{
  byte sum = 0;
  for (int i = 0; i < length; i++)
  {
    sum += data[i];
  }
  return sum;
}

/// @brief スイッチの状態を保持する構造体
struct SwitchState {
  /// @brief ピン番号
  int pin;
  /// @brief スイッチの状態を表す数値
  int state;
};

// 1ループ前のデジタルスイッチの状態
SwitchState prev_d_state[NUMBER_OF_D_SWITCH];
SwitchState prev_a_state[NUMBER_OF_A_SWITCH];

void setup()
{
  // Init of digital pin
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    prev_d_state[i] = SwitchState {d_switch_pin[i][0], 0};
    pinMode(d_switch_pin[i][0], INPUT_PULLUP);
  }

  // Init of analog pin
  for (int i = 0; i < NUMBER_OF_A_SWITCH; i++)
  {
    prev_a_state[i] = SwitchState {a_switch_pin[i], 0};
  }

  Serial.begin(BAUD_RATE);
}

void loop()
{
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    int pin = d_switch_pin[i][0];
    int state = 0;
    state = digitalRead(pin);
    if (d_switch_pin[i][1] == 0)
    {
      state ^= 1;
    }

    // 前回の情報と比較して、スイッチ状態が変わっていれば送信する。
    for (int j = 0; j < NUMBER_OF_D_SWITCH; i++)
    {
      if (prev_d_state->pin == pin)
      {
        if (prev_d_state->state != state)
        {
          send_d(pin, state);
        }
        prev_d_state->state = state;
      }
    }

    delay(SEND_INTERVAL);
  }

  for (int i = 0; i < NUMBER_OF_A_SWITCH; i++)
  {
    int pin = a_switch_pin[i];
    int state = 0;
    state = 1023 - analogRead(a_switch_pin[i]); // if you delete "1023", analog input will invert

    // 前回の情報と比較して、スイッチ状態が変わっていれば送信する。
    for (int j = 0; j < NUMBER_OF_A_SWITCH; i++)
    {
      if (prev_a_state->pin == pin)
      {
        if (prev_a_state->state != state)
        {
          send_a(pin, state);
        }
        prev_a_state->state = state;
      }
    }

    delay(SEND_INTERVAL);
  }
}

int send_d(int pin, int state)
{
  if (pin > 64)
  {
    return -1;
  }

  byte body[3] = {0x02, 0xFF, 0x00}; // 0:COBS_HEAD, 1:DATA, 2:COBS_END
  body[1] =
      (((pin & 0b00111111) << 1) |
       (state & 1));

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

  byte body[5] = {0x04, 0xFF, 0xFF, 0xFF, 0}; // 0:COBS_HEAD, 1:DATA, 2:DATA, 3:CHECKSUM, 4:COBS_END
  body[1] =
      (1 << 7) |
      ((pin & 0b00011111) << 2) |
      (state & 0b1100000000) >> 8;

  body[2] = (byte)state;

  body[3] = check_sum(&body[1], 2);

  // COBS encode
  int prev_zero_index = 4;
  for (int i = 3; i >= 0; i--)
  {
    if (body[i] == 0)
    {
      body[i] = prev_zero_index - i + 1;
      prev_zero_index = i;
    }
  }

  Serial.write(body, 5);

  return 0;
}
