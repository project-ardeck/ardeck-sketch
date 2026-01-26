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
/// @param length 配列の長さ
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

/// @brief byteの配列データをcobsエンコードします。
/// @param data 配列のポインタ
/// @param length 配列の長さ
/// @param res cobsエンコードされた配列のポインタ 長さは`data` + 2 になります
void cobs(byte *data, int length, byte *res)
{
  int res_len = length + 2;

  for (int i = 0; i < 4; i++)
  {
    printf("%d ", data[i]);
  }
  printf("\n");

  for (int i = 0; i < length; i++)
  {
    res[i + 1] = data[i];
  }

  for (int i = 0; i < 6; i++)
  {
    printf("%d ", res[i]);
  }
  printf("\n");

  // 一番新しい、今のところ見つけた0の場所
  int prev_zero_index = 0;

  for (int i = 0; i < res_len; i++)
  {
    if (res[i] == 0)
    {
      res[prev_zero_index] = i - prev_zero_index;
      prev_zero_index = i;
    }
  }
}

// 1ループ前のデジタルスイッチの状態
int prev_d_state[NUMBER_OF_D_SWITCH][2];
int prev_a_state[NUMBER_OF_A_SWITCH][2];

void setup()
{
  // Init of digital pin
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    prev_d_state[i][0] = d_switch_pin[i][0]; // [0] ピン番号
    prev_d_state[i][1] = 0;                  // [1] スイッチの状態

    pinMode(d_switch_pin[i][0], INPUT_PULLUP);
  }

  // Init of analog pin
  for (int i = 0; i < NUMBER_OF_A_SWITCH; i++)
  {
    prev_a_state[i][0] = a_switch_pin[i]; // [0] ピン番号
    prev_a_state[i][1] = 0;               // [1] スイッチの状態
  }

  Serial.begin(BAUD_RATE);
}

void loop()
{
  // 各スイッチのステータスを取得して処理する
  for (int i = 0; i < NUMBER_OF_D_SWITCH; i++)
  {
    int pin = d_switch_pin[i][0]; // 対象のピン番号
    int state = digitalRead(pin); // 対象のステータス

    // デフォルト値のユーザー設定に従って変換する
    if (d_switch_pin[i][1] == 0)
    {
      state ^= 1;
    }

    // 前回の情報と比較して、ステータスが変わっていれば送信する。
    // if (prev_d_state[i][1] != state)
    {
      send_d(pin, state);
    }

    // ステータスを保存する
    prev_d_state[i][1] = state;

    delay(SEND_INTERVAL);
  }

  for (int i = 0; i < NUMBER_OF_A_SWITCH; i++)
  {
    int pin = a_switch_pin[i];
    int state = 0;
    state = 1023 - analogRead(a_switch_pin[i]); // if you delete "1023", analog input will invert

    // 前回の情報と比較して、ステータスが変わっていれば送信する。
    // if (prev_a_state[i][1] != state)
    {
      send_a(pin, state);
    }

    // ステータスを保存する
    prev_a_state[i][1] = state;

    delay(SEND_INTERVAL);
  }
}

int send_d(int pin, int state)
{
  if (pin > 64)
  {
    return -1;
  }

  byte body[2] = {0, 0};
  body[0] =
      (((pin & 0b00111111) << 1) |
       (state & 1));

  body[1] = check_sum(&body[2], 1);

  byte cobsed[4] = {0, 0, 0, 0};
  cobs(body, 2, cobsed);

  Serial.write(cobsed, 4);

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

  body[2] = check_sum(&body[1], 2);

  byte cobsed[5] = {0, 0, 0, 0, 0};
  cobs(body, 3, cobsed);

  Serial.write(cobsed, 5);

  return 0;
}
