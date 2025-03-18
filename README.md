# myRTOS - A Simple Real-Time Operating System

This repository contains a basic Real-Time Operating System (RTOS) implementation, named myRTOS, designed for the Silicon Labs EFM32 Giant Gecko 32-bit microcontroller. This project was developed as part of an academic course on Real-Time Systems.

## Project Overview

myRTOS is a simple RTOS designed to manage multiple tasks with priority-based scheduling. It includes basic features such as task creation, task scheduling, semaphores, and mailboxes for inter-task communication.

## Contents

* **`src/main.c`**: Contains sample tasks and demonstrates how to use the myRTOS API.
* **`src/myRTOS.h`**: Header file containing the RTOS interface, including:
    * Task Control Block (TCB) structure definitions.
    * Semaphore and Mailbox structures.
    * Global variable declarations.
    * Function prototypes for RTOS functions.
* **`src/myRTOS.c`**: Contains the source code for the RTOS utility functions (not included in this repository, but should be in the same directory as the header and main).
* **`src/context.s`**: Assembly file containing context switching and interrupt handlers.
* **`emlib/`**: Contains EFM32 library files for interfacing with the microcontroller hardware.
    * `em_acmp.c`: Analog Comparator (ACMP) library.
    * `em_cmu.c`: Clock Management Unit (CMU) library.
    * `em_core.c`: Core peripheral access library.
    * `em_emu.c`: Energy Management Unit (EMU) library.
    * `em_gpio.c`: General Purpose Input/Output (GPIO) library.
    * `em_int.c`: Interrupt handling library.
    * `em_lcd.c`: Liquid Crystal Display (LCD) library.
    * `em_lesense.c`: Low Energy Sensor Interface (LESENSE) library.
* **`drivers/`**: Contains device driver files.
    * `caplesense.c`: Driver for the Capacitive Low Energy Sensor (CAPLESENSE).
    * `segmentlcd.c`: Driver for the Segment LCD display.
* **`GNU ARM v7.2.1 - Debug/`**: Contains build output and debug files generated by the GNU ARM toolchain.

## Features

* **Task Management:**
    * Task creation with priority assignment.
    * Priority-based scheduling.
    * Task suspension and delay functionality.
* **Inter-Task Communication:**
    * Binary semaphores for synchronization.
    * Mailboxes for data exchange between tasks.
* **System Tick:**
    * A system tick variable for timing and scheduling.
* **Idle Task Management:**
    * Idle counting for aperiodic and sporadic tasks.
* **Context Switching:**
    * Assembly-level context switching implemented in `context.s`.
* **Interrupt Handlers:**
    * `SysTick_Handler` for system tick interrupts.
    * `SVC_Handler` for supervisor call interrupts (used for yielding).
* **Hardware Interaction:**
    * The emlib library files are used to interface with the microcontroller's peripherals.
    * Device drivers in the drivers folder provide higher-level interfaces for specific hardware components.
* **Build Output:**
    * The GNU ARM v7.2.1 - Debug folder contains the compiled output and debug information.

## Usage

1.  **Hardware:** This RTOS was developed for the Silicon Labs EFM32 Giant Gecko 32-bit microcontroller.
2.  **Development Environment:** Use a compatible development environment for EFM32 microcontrollers (e.g., Simplicity Studio).
3.  **Toolchain:** Ensure you have the GNU ARM v7.2.1 toolchain installed.
4.  **Include Files:** Include `myRTOS.h` in your project.
5.  **Task Creation:** Use the `CreateTask()` function to create real-time tasks.
6.  **Scheduling:** The `scheduler()` function manages task scheduling.
7.  **Semaphores:** Use `initSemaphoreBinary()`, `xSemaphoreTake()`, and `xSemaphoreGive()` for semaphore operations.
8.  **Mailboxes:** Use `initMailbox()`, `writeToBox()`, and `readFromBox()` for mailbox operations.
9.  **Task Delay:** Use `vTaskDelayUntil()` to delay tasks.
10. **Sample Tasks:** Refer to `main.c` for examples of task implementation.
11. **Context Switching:** the context switching is handled within the context.s assembly file.
12. **Hardware Interaction:** The emlib library files are used to interface with the microcontroller's peripherals.
13. **Device Drivers:** The drivers folder contains drivers for specific peripherals like the CAPLESENSE and Segment LCD.
14. **Build and Debug:** Use the GNU ARM v7.2.1 toolchain to build the project. The build output and debug files will be located in the `GNU ARM v7.2.1 - Debug` folder.

## Getting Started  

### Prerequisites  
- **Hardware**: Silicon Labs EFM32 Giant Gecko 32-bit microcontroller  
- **Toolchain**:  
  - ARM GCC Compiler (**GNU ARM v7.2.1**)  
  - Debugging tools (e.g., GDB, OpenOCD, SEGGER J-Link)  
  - Silicon Labs Simplicity Studio (optional)  

### Building & Running  
1. Clone the repository:  
   ```bash
   git clone https://github.com/yourusername/myRTOS.git
   cd myRTOS
2. Compile and flash to the microcontroller using your preferred IDE or:
   ```make flash```  # (if using a Makefile-based build system)
3.  Debugging with GNU ARM v7.2.1:
    * Open the GNU ARM v7.2.1 - Debug folder in your debugger.
    * Load the binary and start debugging with GDB or your preferred tool.

  
## Disclaimer

This code was developed during an academic course and is provided "as is," without warranty or liability.

## Author

Zachary Plato

