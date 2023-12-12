# Riverdi 50STM32U599 documentation

## Memory layout

| Type              | Start         | End           | Size          | [KB/MB]   |
|-------------------|---------------|---------------|---------------|-----------|
| FLASH             | 0x0800_0000   | 0x083F_FFFF   | 0x0040_0000   | 4MB       |
| RAM (internal)    | 0x2000_0000   | 0x2026_FFFF   | 0x0027_0000   | 2496KB    |
| RAM (static)      | 0x2800_0000   | 0x2800_3FFF   | 0x0027_4000   | 16KB      |
| RAM (external)    | 0x9000_0000   | 0x93FF_FFFF   | 0x0400_0000   | 64MB      |

### Frame buffer memory

A single frame buffer takes `0x0011_9400` bytes (1125KB).

### Memory with the single frame buffer

| Type              | Start         | End           | Size          | [KB/MB]   |
|-------------------|---------------|---------------|---------------|-----------|
| FLASH             | 0x0800_0000   | 0x083F_FFFF   | 0x0040_0000   | 4MB       |
| RAM (internal)    | 0x2000_0000   | 0x2015_6BFF   | 0x0015_6C00   | 1372KB    |
| RAM (framebuffer) | 0x2015_6C00   | 0x2015_6BFF   | 0x0011_9400   | 1126KB    |
| RAM (static)      | 0x2800_0000   | 0x2800_3FFF   | 0x0027_4000   | 16KB      |
| RAM (external)    | 0x9000_0000   | 0x93FF_FFFF   | 0x0400_0000   | 64MB      |

### Single buffer layout

```ld
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 4096K
  RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 0x156C00  /* 1371KB */
  VRAM (xrw)  : ORIGIN = 0x20156C00, LENGTH = 0x119400  /* 1125KB */
  SRAM4 (xrw) : ORIGIN = 0x28000000, LENGTH = 16K
  OSPI (rw)   : ORIGIN = 0x90000000, LENGTH = 64M
}
```

Addresses are defined in `.ioc`, `Multimedia/LTDC` and `Middlewares/TouchGFX` sections.

`Vector Rendering` feature takes **365KB** of RAM.

Empty project template takes **188KB** of RAM.

## ThreadX (AZURE RTOS) memory pools configuration

| Pool                          | Size  | Macro
|-------------------------------|-------|---------------------------
| THREADX (APP THREAD)          | 128KB | TX_APP_MEM_POOL_SIZE
|                               |  64KB | TX_APP_STACK_SIZE
| FILEX                         |  64KB | FX_APP_MEM_POOL_SIZE
|                               |   4KB | FX_APP_THREAD_STACK_SIZE
| USBX                          |  64KB | UX_HOST_APP_MEM_POOL_SIZE
|                               |  46KB | USBX_HOST_MEMORY_STACK_SIZE
|                               |   4KB | UX_HOST_APP_THREAD_STACK_SIZE
| TOUCHGFX                      |  32KB | TOUCHGFX_APP_MEM_POOL_SIZE
|                               |  16KB | TOUCHGFX_STACK_SIZE
---
Total: 304KB

## Static tools

| Static allocation             | Size  | Notes
|-------------------------------|-------|----------------------------
| Log                           |  36KB | Message pool
| OS wrapper                    |  22KB | Disposable resource handles
---
Total: 58KB
