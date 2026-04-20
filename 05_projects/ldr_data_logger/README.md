# Project: Real-Time LDR Data Logger

## Overview
This project demonstrates a real-world producer–consumer pattern using STM32:
- The ADC samples an LDR (Light Dependent Resistor) on pin A0 and stores values in a circular buffer.
- The consumer reads values from the buffer and:
  - Displays them on an OLED/LCD display
  - Sends them over UART for logging or debugging

## Key Features
- Efficient circular buffer management
- Real-time data acquisition and display
- UART integration for remote monitoring

## Learning Outcomes
- Understand producer–consumer synchronization
- Integrate ADC, display, and UART peripherals
- Apply buffer management in embedded systems

## Extensions
- Add threshold-based alerts
- Log data to external memory
- Visualize data trends on the display
