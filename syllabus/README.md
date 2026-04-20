# STM32 Blue Pill Course Syllabus

## WEEK 1 — Getting Started (IDE + First Success)

**Day 1 — Course Introduction, Hardware Overview, and Development Environment Setup**
  - Introduction to the course structure, goals, and the journey from beginner to advanced embedded developer.
  - Detailed roadmap: What you will learn each week, how topics build on each other, and the skills you’ll gain.
  - Setting expectations: Active participation, hands-on labs, weekly assignments, and the importance of self-driven exploration.
  - Overview of the STM32 Blue Pill hardware: microcontroller features, pinout, peripherals, and why it’s a great learning platform.
  - Introduction to essential tools: STM32CubeIDE for development, Git for version control, and a preview of the final projects (LED blink, UART communication, OLED display).

**Outcome:** Environment ready, motivation set


**Day 2 — Creating Your First Project: Blinking an LED with STM32CubeIDE**
  - Step-by-step guide to creating your first STM32 project in CubeIDE.
  - Understanding project structure, source files, and configuration settings.
  - GPIO basics: configuring pins as outputs, connecting an LED, and writing code to blink it.
  - Introduction to the ST-Link V2 debugger: hardware overview, how to connect it to the Blue Pill, and its role in programming and debugging.
  - Practical demonstration: building, flashing, and running your first firmware on real hardware.

**📝 Assignment:** Change blink pattern

**Outcome:** First working firmware


**Day 3 — Introduction to Debugging: Flashing, Breakpoints, and Variable Watching**
  - What is debugging and why it’s essential for embedded development.
  - How to flash firmware to the microcontroller using CubeIDE and ST-Link V2.
  - Setting and using breakpoints to pause code execution and inspect program flow.
  - Watching and modifying variables in real time to understand program state.
  - How the ST-Link V2 debugger communicates between your PC and the MCU: code loading, breakpoint handling, and live variable inspection.
  - Debugging demonstration: finding and fixing a bug in your LED blink code.

**📝 Assignment:** Debug and modify delay

**Outcome:** Basic debugging skills


**Day 4 — Understanding GPIO: Inputs, Outputs, and Button Control**
  - Deep dive into GPIO: what are digital inputs and outputs, and how microcontrollers interact with the outside world.
  - Practical wiring: connecting a button and an LED to the Blue Pill.
  - Writing code to read button state and control an LED.
  - Explanation of button debouncing: why mechanical switches bounce, how it affects digital input, and common hardware/software debouncing techniques (with code examples).
  - Experiment: observe bouncing in action and implement debouncing in your code.

**📝 Assignment:** Button → LED control

**Outcome:** Understand GPIO behavior

**Day 5 — Exploring the Datasheet: GPIO, RCC, and Register Mapping**
  - Introduction to the STM32 reference manual and datasheet: how to find and interpret key information.
  - Guided reading: locating GPIO and RCC sections, understanding what each register does.
  - Register mapping: how peripheral registers are organized in memory, and how to use this knowledge for low-level programming.
  - Hands-on: find the base address for GPIO and explain how to use it in code.

**📝 Assignment:** Find GPIO base address

**Outcome:** Datasheet reading skill

**Day 6 — Comparing HAL and Register-Level Programming**
  - What is the Hardware Abstraction Layer (HAL) and why is it used?
  - How HAL simplifies peripheral access, and what happens “under the hood.”
  - Pros and cons of using HAL vs direct register-level programming.
  - Transitioning from HAL to baremetal: when and why you might want to “drop down” to registers.
  - Example: compare blinking an LED with HAL vs direct register access.

**Outcome:** Ready for baremetal

**Day 7 — Blinking an LED with Baremetal (Register-Level) Code**
  - Introduction to baremetal programming: what it means to write code without libraries or abstraction layers.
  - How to set up and manipulate microcontroller registers directly to control hardware.
  - Step-by-step: rewrite your LED blink code using only register access (no HAL).
  - Discussion: benefits, risks, and debugging tips for baremetal code.

**📝 Assignment:** Modify timing

**Outcome:** First baremetal control

---

## WEEK 2 — Core Peripherals (Baremetal)

**Day 8 — Understanding the Startup Code: Vector Table and Reset Handler**
  - What happens when the microcontroller powers up? Explore the boot process step by step.
  - Learn about the vector table: what it is, where it lives in memory, and how it directs execution.
  - Understand the reset handler’s role in system initialization and how it prepares the MCU to run your code.
  - Visualize the startup sequence with diagrams and debug walkthroughs.

**Outcome:** Understand boot process

**Day 9 — Analyzing the Linker Script: FLASH, RAM, and Memory Sections**
  - Introduction to linker scripts: why they matter in embedded development.
  - Explore how FLASH and RAM are mapped and divided for code, data, and stack.
  - Deep dive into memory sections: .text (code), .data (initialized data), .bss (zero-initialized data), and stack/heap.
  - Hands-on: read and interpret a real STM32 linker script.

**Outcome:** Memory layout understanding

**Day 10 — Minimal Runtime: Stack Initialization and Data Copying**
  - What is a minimal runtime and why is it needed for baremetal systems?
  - How the stack is set up before main() runs, and why stack initialization is critical.
  - How global/static variables are initialized: copying .data from FLASH to RAM, zeroing .bss.
  - Debug and observe the runtime setup in action.

**Outcome:** Understand how main() runs

**Day 11 — UART Communication in Baremetal: Registers and Transmitting Data**
  - Introduction to UART: what it is, why it’s used, and how it works at the hardware level.
  - Explore the STM32 UART peripheral registers and how to configure them for transmission.
  - Write code to send data from the Blue Pill to a PC or terminal (using USB-to-TTL or other methods).
  - Discuss practical wiring, baud rate selection, and troubleshooting tips.

**📝 Assignment:** Print your name

**Outcome:** Serial communication

**Day 12 — Receiving Data with UART: Polling Method**
  - Learn how to receive data over UART using polling (actively checking for new data).
  - Write code to echo received characters back to the sender.
  - Discuss the limitations of polling and introduce the idea of interrupt-driven reception.

**📝 Assignment:** Echo input

**Outcome:** Two-way communication

**Day 13 — Introduction to SPI: Master Mode and Data Transfer**
  - What is SPI? Learn about the protocol, signals, and use cases.
  - Configure STM32 as an SPI master and connect to a slave device (e.g., sensor or display).
  - Write code to transfer data over SPI and observe results with logic analyzer or debug prints.

**Outcome:** SPI understanding

**Day 14 — Introduction to I2C: Communicating with Peripherals (OLED Example)**
  - Introduction to I2C protocol: how it differs from SPI, wiring, and addressing.
  - Configure STM32 as an I2C master and communicate with a simple peripheral (e.g., OLED display).
  - Write code to send and receive data, and debug common I2C issues.

**📝 Assignment:** Send data to device

**Outcome:** I2C fundamentals

---

## WEEK 3 — Embedded Concepts


**Day 15 — Interrupts vs Polling: Concepts, Comparison, and Use Cases**
  - Explore the difference between polling (actively checking for events) and interrupts (reacting to events as they occur)
  - Discuss advantages, disadvantages, and real-world scenarios for each method
  - Analyze timing, CPU usage, and responsiveness

**Outcome:** Develop real-time thinking and understand when to use polling or interrupts


**Day 16 — Implementing External Interrupts: Button-Triggered Events**
  - Learn how to configure and use external interrupts on STM32
  - Set up a button to trigger an interrupt and control an LED
  - Understand interrupt service routines (ISRs) and their best practices

**📝 Assignment:** Implement an interrupt-based LED control using a button

**Outcome:** Gain experience with event-driven design and external interrupt handling


**Day 17 — NVIC (Nested Vectored Interrupt Controller): Priorities and Control**
  - Deep dive into the NVIC: how STM32 manages multiple interrupts
  - Learn about interrupt priorities, preemption, and enabling/disabling interrupts
  - Practice configuring priorities for different peripherals

**Outcome:** Master interrupt control and prioritization in embedded systems


**Day 18 — Circular Buffer: Concept, Design, and Applications**
  - Understand the need for buffering in embedded systems
  - Learn how circular (ring) buffers work and why they are efficient
  - Design and implement a circular buffer for data storage

**Outcome:** Gain practical skills in data buffering and memory management



**Day 19 — Real-Time Data Handling: ADC Producer, Circular Buffer, and Display/UART Consumer**
  - Integrate ADC sampling (LDR sensor) as a data producer, storing values in a circular buffer
  - Implement a consumer that reads from the buffer and outputs data to both a display (OLED/LCD) and UART
  - Address synchronization and timing between producer and consumer tasks
  - Discuss real-world applications of this pattern (e.g., data logging, sensor monitoring)

**📝 Assignment:** Build a complete producer–consumer system: ADC fills the buffer, display/UART consumes and presents the data

**Outcome:** Achieve hands-on experience with real-time data acquisition, buffering, and multi-peripheral output


**Day 20 — Integration Project: Combining UART, Interrupts, and Buffering**
  - Bring together UART communication, interrupt handling, and circular buffering in a single project.
  - Design a mini-system where multiple peripherals interact in real time.
  - Practice modular code organization and interface definition.

**Outcome:** Develop system-level thinking and integration skills.


**Day 21 — Code Structuring: Separating Drivers and Application Layer**
  - Learn best practices for organizing embedded code into reusable drivers and application logic.
  - Refactor previous projects to separate hardware abstraction from business logic.
  - Discuss maintainability, scalability, and portability.

**Outcome:** Achieve clean, maintainable, and scalable code architecture.

---

## WEEK 4 — Build Systems


**Day 22 — Build System Fundamentals: Compiler, Linker, and Manual GCC Build**
  - Explore the embedded build process: compiling, linking, and generating firmware binaries.
  - Manually build a project using GCC toolchain commands.
  - Understand the role of each build step and output file.

**Outcome:** Gain a clear understanding of the embedded build flow.


**Day 23 — Getting Started with CMake: Writing Basic CMakeLists and Targets**
  - Introduction to CMake as a cross-platform build system.
  - Write simple CMakeLists.txt files to define build targets and source files.
  - Build your first project using CMake.

**Outcome:** Successfully create and build a project with CMake.


**Day 24 — Setting Up the ARM Toolchain and Cross-Compiling Firmware**
  - Install and configure the ARM GCC toolchain for STM32 development.
  - Cross-compile firmware for the target microcontroller.
  - Troubleshoot common toolchain and build issues.

**Outcome:** Set up a robust embedded build environment.


**Day 25 — Accelerating Builds with Ninja Build System**
  - Learn about the Ninja build system and its advantages for embedded projects.
  - Integrate Ninja with CMake for faster incremental builds.
  - Compare build times and workflows.

**Outcome:** Achieve efficient and rapid build cycles.


**Day 26 — CMake Integration for Baremetal Project: Porting and Automating Your Build**
  - Convert an existing baremetal project to use CMake for building and automation.
  - Organize source files, headers, and linker scripts for CMake compatibility.
  - Build and flash firmware without relying on an IDE.

**📝 Assignment:** Successfully build and run firmware using only CMake and command-line tools.

**Outcome:** Gain confidence in IDE-free, automated build workflows.


**Day 27 — Analyzing Build Outputs: ELF, BIN Files, and Firmware Size**
  - Examine the different output files generated by the build process (.elf, .bin, .hex).
  - Use tools to analyze firmware size and memory usage.
  - Understand how to optimize and interpret build results.

**Outcome:** Develop skills in analyzing and optimizing firmware binaries.

---

## WEEK 5 — RTOS (Zephyr)


**Day 28 — Real-Time Operating System (RTOS) Concepts: Tasks and Scheduling**
  - Introduction to RTOS concepts: multitasking, scheduling, and task priorities.
  - Explore why and when to use an RTOS in embedded systems.
  - Discuss context switching, inter-task communication, and timing.

**Outcome:** Build foundational understanding of RTOS principles.


**Day 29 — Getting Started with Zephyr RTOS: Setup and First LED Project**
  - Set up the Zephyr RTOS development environment.
  - Create and configure a basic Zephyr project (e.g., LED blink).
  - Learn about Zephyr’s build system, configuration, and device tree.

**Outcome:** Gain hands-on experience with Zephyr RTOS.


**Day 30 — Final Project: Multi-Threaded System with LED and UART Tasks (RTOS vs Baremetal)**
  - Design and implement a complete embedded system using both baremetal and RTOS approaches.
  - Create multiple threads/tasks (e.g., LED control, UART communication).
  - Compare and contrast the design, performance, and maintainability of both approaches.

**Outcome:** Demonstrate complete system design and compare industry-relevant techniques.

---

## Final Learning Journey

By Day 30, students will:
- ✔ Program STM32 using IDE
- ✔ Understand startup + linker
- ✔ Write baremetal drivers
- ✔ Use CMake
- ✔ Handle interrupts & buffers
- ✔ Build RTOS-based systems with Zephyr