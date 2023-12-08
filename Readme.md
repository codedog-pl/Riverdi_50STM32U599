# Riverdi U599 HMI documentation

## Memory layout

| Type              | Start         | End           | Size          | Readable  |
|-------------------|---------------|---------------|---------------|-----------|
| FLASH             | 0x0800_0000   | 0x083F_FFFF   | 0x0040_0000   | 4MB       |
| RAM (internal)    | 0x2000_0000   | 0x2026_FFFF   | 0x0027_0000   | 2496KB    |
| RAM (static)      | 0x2800_0000   | 0x2800_3FFF   | 0x0027_4000   | 16KB      |
| RAM (external)    | 0x9000_0000   | 0x93FF_FFFF   | 0x0400_0000   | 64MB      |

### Frame buffer memory

A single frame buffer takes `0x0011_9400` bytes (1125KB).

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

| Pool                          | Size
|-------------------------------|------
| ThreadX memory pool           | 4KB
| FileX memory pool             | 4KB
| UXHost memory pool            | 32KB
| USBX host system stack        | 22KB
| TouchGFX memory pool          | 16KB
| TouchGFX memory stack         | 8KB

| Total                         | Size
|-------------------------------|------
| Pools                         | 56KB
| Stacks                        | 30KB
