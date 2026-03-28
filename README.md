# pico--telemetry

In Progress

Phase 1 — Bare metal 
  - Linker script + startup assembly
  - UART driver (MMIO, ring buffer, IRQ)
  - 1-Wire + DS18B20 driver
  - CRC library
  - Flash driver
  - Bootloader (DFU + CRC verify + jump)
  - Telemetry protocol + packet framer
  - Python host monitor

Phase 2 — Add FreeRTOS on top (same project, same repo)
  - Keep bootloader bare-metal (bootloaders never run an RTOS)
  - Port app layer to FreeRTOS tasks:
      Task 1 → read DS18B20 every 1s
      Task 2 → pack + send telemetry packets
      Task 3 → listen for commands from PC
  - Add watchdog (kicks from main task)
  - Add DMA to UART driver
  - Add power management (sleep between readings)
