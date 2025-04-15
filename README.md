# ESP32 433MHz Signal Cloner

A capstone project for capstone project at VIT Vellore (B.Tech CSE)

## Team Members

- Raunak Jha (Project Lead)
- Sahith (Team Member)
- Chaitainya (Team Member)

## Project Overview

This project implements a 433MHz RF signal cloner using an ESP32 microcontroller. The system can capture, store, and replay RF signals commonly used in remote controls, garage door openers, and other wireless devices operating in the 433MHz frequency band.

## Features

- Signal Capture: Records 433MHz RF signals with code and protocol information
- Signal Storage: Saves captured signals for later use
- Signal Replay: Transmits stored signals on demand
- Web Interface: User-friendly web interface for signal management
- Bluetooth Connectivity: Wireless control via Bluetooth
- Real-time Feedback: Visual and audio feedback for signal operations

## Hardware Requirements

- ESP32 Development Board
- 433MHz RF Transmitter Module
- 433MHz RF Receiver Module
- Antenna (for both transmitter and receiver)
- Power Supply (USB or battery)

## Software Requirements

- Arduino IDE
- ESP32 Board Support Package
- Required Libraries:
  - RCSwitch
  - RcSwitchReceiver
  - BluetoothSerial

## Installation

1. Clone this repository:

   ```bash
   git clone https://github.com/pattag0bi/esp32_433_cloner.git
   ```

2. Install the required libraries in Arduino IDE:

   - RCSwitch
   - RcSwitchReceiver
   - BluetoothSerial

3. Upload the code to your ESP32:
   - Open `6_esp32_433mhz_cloner.ino` in Arduino IDE
   - Select your ESP32 board
   - Upload the code

## Usage

1. Power on the ESP32
2. Connect to the ESP32's web interface using a web browser
3. To capture a signal:
   - Press the "Capture Signal" button
   - Press the button on your remote control
   - The signal will be captured and displayed
4. To replay a signal:
   - Select the signal from the list
   - Press the "Replay Signal" button

## Web Interface

The web interface provides:

- Real-time signal monitoring
- Signal capture and replay controls
- Signal storage management
- Visual feedback for signal operations
- Audio feedback for signal detection

## Troubleshooting

If you encounter issues:

1. Check the transmitter and receiver connections
2. Verify the antenna is properly attached
3. Ensure the power supply is adequate
4. Check the serial monitor for error messages
5. Verify the signal is within the 433MHz frequency range

## Project Structure

```
esp32_433_cloner/
├── 6_esp32_433mhz_cloner.ino    # Main ESP32 code
├── index.html                    # Web interface
├── libraries/                    # Required libraries
│   ├── rc-switch/               # RCSwitch library
│   └── RcSwitchReceiver/        # RcSwitchReceiver library
└── used_libraries/              # Additional libraries
    └── BluetoothSerial/         # Bluetooth library
```

## Contributing

This project is part of our college capstone. While we're not accepting external contributions, we welcome feedback and suggestions.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Our college faculty for guidance and support
- The open-source community for the libraries used in this project
- Our capstone advisor for their valuable input

## Contact

For questions or support, please contact:

- Raunak Jha: jharaunak@outlook.com
- Sahith: Sahithvd@gmail.com
- Chaitainya: chaitanya.sharma2021@vitstudent.ac.in
