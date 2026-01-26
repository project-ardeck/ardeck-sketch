#ifndef ARDECK_CONFIG_H // Do not change
#define ARDECK_CONFIG_H // Do not change

#define BAUD_RATE 9600 // Serial port baud rate (Recommend: 9600 or 19200)
#define SEND_INTERVAL 1 // Send interval (If you don't want it, set it to 0.)

extern const int d_switch_pin[][2] = // Specify digital pins to use and default values. {pin_number, default_value}
  {
    {2, 0},
    {3, 0}
  };

extern const int a_switch_pin[] = // Specify analog ping.
{
  // A0
};

#define NUMBER_OF_D_SWITCH sizeof(d_switch_pin) / sizeof(d_switch_pin[0]) // Do not change
#define NUMBER_OF_A_SWITCH sizeof(a_switch_pin) / sizeof(a_switch_pin[0]) // Do not change

#endif // Do not change