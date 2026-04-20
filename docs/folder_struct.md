# Folder Structure Documentation

This document describes where each type of file or project should be placed in the repository, mapped to the course syllabus and project structure.

---

## 01_hal_basics/
- led_blink/
  - main.c / main.cpp
  - led_blink.c/h
  - CubeIDE project files
  - README.md (explain LED blink with HAL)
- button_input/
  - main.c / main.cpp
  - button.c/h
  - CubeIDE project files
  - README.md (button input, debouncing)
- uart_hal/
  - main.c / main.cpp
  - uart_hal.c/h
  - CubeIDE project files
  - README.md (UART with HAL)

## 02_under_the_hood/
- startup_analysis/
  - startup_stm32.s
  - vector_table_diagram.png
  - README.md (explain startup flow)
- linker_analysis/
  - linker_script.ld
  - memory_map.png
  - README.md (explain linker script)
- memory_layout/
  - annotated_memory_map.md
  - README.md (explain memory sections)

## 03_baremetal/
- gpio/
  - main.c / main.cpp
  - gpio_baremetal.c/h
  - README.md (baremetal GPIO)
- uart/
  - main.c / main.cpp
  - uart_baremetal.c/h
  - README.md (baremetal UART)
- spi/
  - main.c / main.cpp
  - spi_baremetal.c/h
  - README.md (baremetal SPI)
- i2c/
  - main.c / main.cpp
  - i2c_baremetal.c/h
  - README.md (baremetal I2C)

## 04_embedded_concepts/
- interrupts/
  - interrupt_demo.c/h
  - README.md (external interrupts, NVIC)
- polling_vs_interrupt/
  - polling_example.c
  - interrupt_example.c
  - README.md (compare polling and interrupt)
- circular_buffer/
  - circular_buffer.c/h
  - test_circular_buffer.c
  - README.md (buffer concept, usage)
- producer_consumer/
  - producer_consumer_demo.c
  - README.md (ADC producer, UART/display consumer)

## 05_projects/
- baremetal_uart_buffer/
  - main.c
  - uart_buffer.c/h
  - README.md (UART with buffer)
- oled_display/
  - main.c
  - oled_driver.c/h
  - README.md (display project)
- sensor_logger/
  - main.c
  - sensor_logger.c/h
  - README.md (data logging)
- ldr_data_logger/
  - main.c
  - ldr_adc.c/h
  - display.c/h
  - uart.c/h
  - circular_buffer.c/h
  - README.md (real-time LDR logger project)

## 06_zephyr/
- getting_started/
  - prj.conf
  - main.c
  - README.md (Zephyr setup)
- gpio/
  - main.c
  - README.md (Zephyr GPIO)
- uart/
  - main.c
  - README.md (Zephyr UART)
- i2c/
  - main.c
  - README.md (Zephyr I2C)
- final_project/
  - main.c
  - README.md (multi-threaded RTOS project)

## docs/
- concepts/
  - Each concept folder: .md files explaining theory, diagrams, and code snippets.
- datasheets/
  - STM32 datasheet PDFs, reference manuals.
- images/
  - Diagrams, pinouts, flowcharts.

## tools/
- setup_guide.md (tool installation)
- toolchain/
  - scripts for installing/configuring toolchains
- flash_scripts/
  - scripts for flashing firmware

## assignments/
- week1/ to week5/
  - assignment.md (weekly tasks)
  - starter code or templates if needed

---

This structure ensures all code, documentation, and resources are easy to find and logically organized for both learning and project development.
