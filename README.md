# RFID-Based-Smart-Car-Parking-System-Using-Arduino
An Arduino-based smart parking system that uses RFID authentication, IR sensors, and a servo-controlled gate to automate vehicle entry and exit. It displays available parking slots on an I2C LCD and provides LED and buzzer alerts for efficient parking management.
# RFID-Based Smart Car Parking System Using Arduino

## Project Overview

The RFID-Based Smart Car Parking System is an Arduino-based project designed to automate vehicle entry and exit in a parking area. It uses an RFID reader to authenticate authorized users, IR sensors to detect vehicles, and a servo motor to control the parking gate. An I2C LCD displays the number of available parking slots, while LEDs and a buzzer provide visual and audio alerts.

The system helps make parking management more organized by reducing manual effort and tracking parking slot availability.

## Features

- RFID-based vehicle access authentication
- Automatic gate control using a servo motor
- IR sensor-based vehicle entry and exit detection
- Real-time parking slot availability display
- 16x2 LCD with I2C interface
- Green LED indication for access or parking status
- Red LED indication for restricted or unavailable access
- Buzzer alerts
- Support for a maximum of four parking slots in the current code

## Components Required

1. Arduino Uno
2. MFRC522 RFID reader module
3. RFID card or tag
4. Servo motor
5. Two IR sensors
6. 16x2 LCD with I2C module
7. Green LED
8. Red LED
9. Buzzer
10. Breadboard and jumper wires
11. Suitable power supply and connecting cables

## Hardware Connections

RFID Reader (MFRC522):
- SDA/SS — Arduino D10
- SCK — Arduino D13
- MOSI — Arduino D11
- MISO — Arduino D12
- RST — Arduino D9
- VCC — Arduino 3.3V
- GND — Arduino GND

Servo Motor:
- Signal — Arduino D2
- Power and ground — Appropriate supply connections according to the servo specifications

IR Sensors:
- Entry sensor output — Arduino D4
- Exit sensor output — Arduino D3
- VCC and GND — Appropriate supply and common ground

I2C LCD:
- SDA — Arduino A4
- SCL — Arduino A5
- VCC — Suitable supply for the LCD module
- GND — Arduino GND

Other Components:
- Buzzer — Arduino D5
- Green LED — Arduino D6
- Red LED — Arduino D7

Important: The MFRC522 RFID reader operates at 3.3V. Do not connect its VCC directly to 5V. Check the voltage requirements of each module before wiring. The IR sensor pin assignments above follow the supplied Arduino code; the report diagram may show the entry and exit sensors on opposite pins.

## Software Requirements

- Arduino IDE
- MFRC522 library
- Servo library
- LiquidCrystal_I2C library
- SPI library
- Wire library

The SPI, Servo, and Wire libraries are commonly included with the Arduino environment. Install the MFRC522 and LiquidCrystal_I2C libraries through the Arduino IDE Library Manager if they are not already available.

## Installation and Setup

1. Install the Arduino IDE on your computer.
2. Connect the Arduino Uno to your computer using a USB cable.
3. Install the required libraries.
4. Assemble the circuit according to the hardware connections.
5. Open the Arduino source code in the Arduino IDE.
6. Check that the RFID reader, LCD, servo motor, LEDs, buzzer, and IR sensors are connected correctly.
7. Select Arduino Uno as the board and choose the correct serial port.
8. Upload the code to the Arduino.
9. Power the circuit and verify that the LCD initializes and the RFID reader detects cards.

## How the System Works

1. The system initializes the RFID reader, LCD, servo motor, sensors, LEDs, and buzzer.
2. The LCD displays parking information, including available slots.
3. When an RFID card is presented, the system reads its unique identifier and compares it with the authorized UID configured in the code.
4. If the card is authorized, the system operates the servo motor to open the gate.
5. The IR sensors detect vehicle movement at the entry and exit points.
6. The parking slot count is updated according to the programmed entry and exit logic.
7. The LCD displays the current parking availability.
8. LEDs and the buzzer provide status indications.

## Configuration

Before using the system, review the following settings in the Arduino code:

- Authorized RFID UID: The current code uses "BC 0A D5 05". Replace this value with the UID of your own authorized RFID card.
- Maximum parking capacity: The current code is configured for four parking slots.
- LCD I2C address: The code uses 0x27.
- LCD dimensions: 16 columns and 2 rows.
- Servo positions: The code uses angles of 0 and 100 degrees for gate positions.

The actual servo angles may need adjustment depending on the gate mechanism and servo installation.

## Safety and Limitations

- Use a suitable power supply for the servo motor and other modules.
- Ensure all components share a common ground where required.
- Verify the RFID module's voltage requirements before connecting it.
- The current implementation is a prototype and is not intended to control a full-size vehicle barrier without additional safety engineering.
- Sensor timing and vehicle detection depend on the physical placement and calibration of the IR sensors.
- Some sensor-waiting operations in the current code may block other operations while the system waits for a sensor state to change.
- The current implementation uses a predefined authorized RFID UID rather than a database of users.

## Future Improvements

- Support multiple authorized RFID cards.
- Add a mobile or web-based parking dashboard.
- Store entry and exit records in a database.
- Introduce an automatic full-parking alert.
- Improve sensor handling with non-blocking logic.
- Add vehicle identification and parking history.
- Integrate wireless monitoring using Wi-Fi or Bluetooth.
- Add more robust access control and fault detection.

## Applications

- Educational embedded-systems projects
- Smart parking prototypes
- RFID-based access control demonstrations
- Small-scale parking management models
- IoT and automation learning projects

## Conclusion

The RFID-Based Smart Car Parking System demonstrates how Arduino, RFID authentication, IR sensors, a servo motor, and an LCD can work together to automate basic parking operations. The project provides a practical introduction to embedded systems, sensor integration, access control, and automation.

## Technologies Used

Arduino Uno, C/C++ for Arduino, RFID, IR sensors, servo motor, I2C LCD, SPI communication, and I2C communication.
