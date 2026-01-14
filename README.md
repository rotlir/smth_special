# Something special

## Required components
- Arduino Micro/Nano
- 8x8 WS2812b matrix
- MPU6050 IMU
- Tactile push button

### Optional
- Lithium ion battery
- DC/DC step-up converter for powering the electronics
- TP4056 charging unit with battery protection or any alternative

## Wiring
- WS2812b data pin --> D4
- Push button --> D9 (pulled up in code, the button should connect the pin to ground)
- MPU6050 SDA/SCL --> Microcontroller SDA/SCL (A4/A5 for Arduino Nano, D2/D3 for Arduino Micro)


## License
This project is licensed under the terms of the GNU General Public License v3.0. See LICENSE for details.

