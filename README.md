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

# RP2040 Bare-Metal Firmware Engine

Bare-metal firmware stack for RP2040 (ARM Cortex-M0+). Zero vendor HAL.
Every line written from scratch using the RP2040 datasheet and ARM architecture reference.

## Philosophy
No SDK. No HAL. No abstraction layers. Every register access is direct,
every design decision is deliberate. The goal is to understand exactly
what the hardware is doing at every step.

## What's Built

### 1. Linker Script (`scripts/boot.ld`)
Tells the linker exactly how to arrange compiled code in memory.
 
**Memory regions:**
- `FLASH` at `0x10000000` — RP2040 external flash, where code lives
- `SRAM` at `0x20000000`, 264kB — runtime data, stack

**Why `0x10000000` for flash?**
The RP2040 boot ROM at `0x00000000` is manufacturer silicon — read only.
It runs first on reset, checks flash at `0x10000000`, then jumps to our code.

**Sections:**
- `.vector_table` — must be first in flash. CPU reads initial SP and
  reset handler address from here on reset.
- `.text` — compiled code and constants
- `.rodata` — read-only data, string literals
- `.data` — initialized globals. Stored in flash, copied to SRAM at startup.
  Uses `AT> FLASH` directive — load address in flash, runtime address in SRAM.
- `.bss` — uninitialized globals. No flash storage needed, zeroed at startup.
- `.STACK` — 8kB stack at top of SRAM. `_stack_top` symbol marks the end
  since ARM stack grows downward.

**Key symbols exported to startup assembly:**
- `_sidata` — source address of `.data` in flash
- `_sdata`, `_edata` — destination range in SRAM
- `_sbss`, `_ebss` — BSS range to zero
- `_stack_top` — initial stack pointer value

---

### 2. Startup Assembly (`boot/startup.s`)
Runs before `main()`. Sets up the C runtime environment.

**Why assembly?**
C depends on initialized globals and a valid stack. Those don't exist
until startup sets them up. It's a chicken-and-egg problem — you can't
run C until the environment is ready, so you use assembly to build it.

**What it does in order:**
1. Places `_stack_top` and `reset_handler` address in `.vector_table`
   section — CPU reads these two words immediately on reset
2. Copies `.data` from flash to SRAM word by word using
   `_sidata`, `_sdata`, `_edata` symbols from linker script
3. Zeroes `.bss` region using `_sbss`, `_ebss`
4. Calls `main()`
5. Hangs in `deadloop` if `main()` ever returns

**ARM instructions used:**
- `LDR r0, =symbol` — load address of linker symbol into register
- `LDR r3, [r2]` — load word from memory address in r2
- `STR r3, [r0]` — store word to memory address in r0
- `ADDS r0, #4` — advance pointer by one word
- `CMP`, `BNE` — loop control
- `BL main` — branch with link to call main

---

### 3. UART Driver (`src/uart.c`, `include/uart.h`)
Direct register access UART driver for UART0. No SDK functions.

**Register struct design:**
```c
typedef struct __attribute__((packed)) {
    volatile uint32_t DR;
    volatile uint32_t RSRECR;
    uint32_t _reserved0[4];      /* gap 0x008-0x014 */
    const volatile uint32_t FR;
    uint32_t _reserved1;
    volatile uint32_t ILPR;
    volatile uint32_t IBRD;
    volatile uint32_t FBRD;
    volatile uint32_t LCRH;
    volatile uint32_t CR;
} uart_registers;
```

**Why `volatile` on every register field?**
The compiler optimizes code by caching variable values. Hardware registers
change without CPU involvement — a byte arrives on UART and FR changes
by itself. `volatile` tells the compiler never cache this value, always
read directly from memory. Without it the driver would miss incoming data.

**Why `const volatile` on FR?**
FR is the flag register — read only hardware. `volatile` because hardware
changes it, `const` because our code should never write to it. The compiler
will catch accidental writes at compile time.

**Why `__attribute__((packed))`?**
Normally the compiler inserts padding between struct fields for alignment.
For memory-mapped registers the struct must exactly match the hardware
layout — any padding would shift fields to wrong addresses. `packed`
prevents this. In this case all fields are `uint32_t` so no padding would
be inserted anyway, but `packed` makes the intent explicit.

**Why reserved fields instead of computing offsets?**
The struct maps directly to hardware memory. Reserved fields fill gaps
between registers exactly as specified in the datasheet. For example
RSRECR is at offset `0x004` and FR is at `0x018` — a gap of `0x14` = 20
bytes = 5 `uint32_t` fields.

**Why `#define` instead of `static` pointer?**
```c
#define UART0 ((uart_registers *)0x40034000)
```
A `static` variable in a header creates a separate instance per translation
unit — every `.c` file that includes the header gets its own copy in memory.
A `#define` is pure text substitution, zero memory overhead. For
memory-mapped peripherals where the address is fixed, a macro is correct.

**Baud rate calculation:**
```c
UART0->IBRD = UART_CLK / (16 * baud);
UART0->FBRD = ((UART_CLK % (16 * baud)) * 64) / (16 * baud);
```
UART_CLK = 125MHz (RP2040 peripheral clock default).
UART samples each bit 16 times for noise immunity — hence divide by 16.

**Initialization sequence:**
1. Disable UART (CR bit 0)
2. Wait for current transmission to finish (FR bit 3 BUSY)
3. Flush FIFO (clear LCRH bit 4 FEN)
4. Set baud rate (IBRD, FBRD)
5. Set 8N1 + enable FIFO (LCRH bits 4, 5, 6)
6. Enable UART + TX + RX (CR bits 0, 8, 9)

## In Progress
- Bootloader — UART DFU, CRC32 verification, flash routines from SRAM
- 1-Wire DS18B20 temperature driver
- Interrupt-driven UART with DMA ring buffer
- FreeRTOS port — sensor, telemetry, watchdog tasks
- Binary telemetry protocol with CRC16
- Python host decoder and monitor