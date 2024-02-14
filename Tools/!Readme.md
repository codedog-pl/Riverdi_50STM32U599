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

The character encoding is either ASCII or UTF-8
(when supported by the target backend).

The current version supports following middlewares and APIs:

- STM32 HAL
- Azure RTOS
  - FILEX
  - USBX
- FreeRTOS
  - FATFS
  - USB_HOST
- TouchGFX

"On the other end" (the compatible one) it provides a common,
target independent C++ API.

It is designed to be:

- minimalistic (KISS, SRP)
- light weight (using as little resources as possible)
- intuitive
- extensible

## APIs provided

- File System (FS namespace) - tools to access various embedded filesystems, buildable `Path` type, a `File` object that can be used in RAII way.

- RTOS (OS namespace) - tools to synchronize threads and schedule tasks (with ISR support) using either Azure RTOS or FreeRTOS backend.

An application that uses those APIs is target operation
system agnostic.

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
- Naming convention:
  - pascal case type names,
  - snake case method and member names,
  - private members start with `m_` prefix.
  - interface names start with `I` prefix.
  - template parameters start with `T` prefix.

Some exceptions are allowed in C code, like 2 spaces
indentation that is consistent with the STM32 HAL source.

Each file should contain identically formatted header that
exactly describes what is it for in the project.

None of the WTK files can trigger compiler errors related to dependencies. If some portions of the code require external dependencies, they have to be marked for conditional compilation (see FS and OS for example use).

`Enum`s are preferred over `enum class`es, unless member names could collide with other namespace members.

This rule gives the benefit of greater readability when
used enumerations as bit flags.

### Error handling

**Using the C++ exceptions is strictly forbidden!**

All functions that can fail should return either a `false`, `0`, `nullptr` or other kind of empty or invalid object in case they cannot return a valid result.

In case of an unrecoverable error when the system stability cannot be guaranteed, the code should enter the inifinite loop to prevent damage to the software and hardware components. The infinite loop also makes finding the crash source easier when using a debugger.

Use the dedicated `Crash::here()` function to handle an unrecoverable error.

### Custom terms used

- *action* - the most basic function pointer; a pointer to a function that takes no arguments and returns no value.

- *binding action* - a pointer to a function that takes a `void*` pointer as an argument and returns no value.

- *binding (1)* - data binding; a technique of passing a `void*` pointer in order to pass data without enforcing the type; the function receiving the pointer is responsible for casting the pointer to the appropriate type.

- *binding (2)* - providing interoperable elements, like functions to call C++ class members from the C language.

- *bit flags* - an enumeration type based on an unsigned integer type that has named members corresponding to the bits in the number; different members can be combined (usually using bitwise `OR`) that is the main difference between bit flags and a basic enumeration.

- *interface* - a class that contains mostly pure virtual functions to be implemented by derived classes.

- *pool* - here it's just a statically allocated array of elements that can be taken, returned, cleared and reused.

- *static class* - a class that contains only static members; a kind of simplified singleton.

- *task* - an object that has an assigned function that is not called immediately, but is scheduled to be called either in another thread, or in the main thread, but later.

- *timeout (1)* - usually a number of system clock ticks after an operation is considered not done / canceled.

- *timeout (2)* - an object that will call a specified action if it's not cleared before the configured time elapses.

## Issues

Please report all issues found on GitHub.

## Author

The toolkit is created by Adam Łyskawa for CodeDog Sp. z o.o.

It's realeased as Open Source under MIT license.
