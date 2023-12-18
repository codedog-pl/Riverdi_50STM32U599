# Woof Toolkit

## Description

This folder contains Woof Toolkit, a set of programming tools
created as a higher level abstraction layer over C language
primitives, embedded systems drivers, firmware and middleware.

The goal of this toolkit is to make the actual application
logic clean, readable and as portable as possible.

Also it allows the application to use the power of the modern
C++ language without the expensive standard library parts.

The toolkit is guarateed to NOT USE DYNAMIC MEMORY ALLOCATIONS.

All the strings used in the API are basic null terminated
C strings.

The character encoding is either ASCII or UTF-8 (when supported by the target backend).

The current version supports following middlewares and APIs:

- STM32 HAL
- AzureRTOS
  - FILEX
  - USBX
- FreeRTOS
  - FATFS
  - USB_HOST
- TouchGFX

"On the other end" (the compatible one) it provides a common,
target independent C++ API.

It is designed to be:

- minimalistic (KISS)
- light weight (using as little resources as possible)
- intuitive
- extensible

## Usage

Add entire `/Tools` directory to the project. Add all
`.c` and `.cpp` files to the build configuration, add
`/Tools` and `/Tools/c` directiores as include directories.

Be sure to set C++ language version to at least 17.

Review and edit the `c/target.h` file according to the actual
MCU target configuration.

## Extending

Use the existing APIs and tools to add any missing features.
If the coding style is consistent with the rest of the toolkit,
feel free to propose a pull request on GitHub.

## Coding style

The source code follows some strict rules:

- 4 spaces indentation.
- Opening and closing braces in separate lines.
- No whitespace around function names.
- The pointer and reference symbols "glued" to the type,
  not the member.
- Doxygen inline documentation on all public API.

Some exceptions are allowed in C code, like 2 spaces
indentation that is consistent with the STM32 HAL source.

## Issues

Please report all issues found on GitHub.

## Author

The toolkit is created by Adam Łyskawa for CodeDog Sp. z o.o.

It's realeased as Open Source under MIT license.
