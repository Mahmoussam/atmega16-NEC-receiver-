# ATmega16A NEC IR Receiver and Decoder

This project demonstrates how to interface an **ATmega16A microcontroller** with an **NEC IR receiver module (TSOP38)** to decode IR remote control signals. The decoded signals can be used to control various devices or applications.

my main goal is to use the 8-bit timer2 so that the 16-bit timer is available for integrating USB support as HID device (next step).

note that my receiver signal is inverted.
![Logic analyzer output for my actual IR signal](https://github.com/Mahmoussam/atmega16-NEC-receiver-/blob/master/logic_NEC.png)
## Specs
- **NEC Protocol Decoding**: Handles decoding of NEC IR remote control signals , note that my receiver signal is inverted.
- **LCD Display**: Displays decoded commands in hexadecimal format.
- **Interrupt-Driven Design**: Utilizes the ATmega16A’s INT2 interrupt on falling edges for precise signal capture.

## Hardware Requirements
- **ATmega16A Microcontroller**
- **TSOP1838 IR Receiver Module**
- **16x2 LCD Display**
- **Potentiometer**

## Connections
### ATmega16A to TSOP38
- **TSOP38 Signal** → **ATmega16A INT2 (PB2)**

### ATmega16A to LCD
- **LCD D4** → **PD0**
- **LCD D5** → **PD1**
- **LCD D6** → **PD2**
- **LCD D7** → **PD3**
- **LCD RS** → **PD5**
- **LCD E** → **PD7**
- **LCD RW** → Ground

rest of the pins are power connections , pretty much found everywhere online.

## Software
- **Toolchain**: Atmel Studio
- **Programming Language**: C

## How It Works
1. The TSOP38 IR receiver detects modulated IR signals and sends them to the INT2 pin of the ATmega16A after filtering the 38Kkhz carrier wave.
2. The microcontroller processes the signal by triggering the interrupt on falling edges , and utilizing Timer2 for measuring bits width.
3. Decoded data is displayed on the LCD.
- Timer prescalar is 1024
- Chip clock is external 16 Mhz oscillator.
- 
## Future Enhancements
- Add support for HID USB communication to communicate with the host computer.
- Implement device control based on decoded commands.

## License
This project is licensed under the MIT License. Feel free to use and modify it.


