/**
 * @file        bindings.h
 * @author      Adam Łyskawa
 *
 * @brief       Defines EXTERN_C C/C++ interoperaion macros.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#ifdef __cplusplus

/// @brief Marks the following expression as `extern "C"` for the C++ compiler.
#define EXTERN_C extern "C"

/// @brief Marks the start of an `extern "C"` block only for C++ compiler.
#define EXTERN_C_BEGIN extern "C" {

/// @brief Ends the block marked.
#define EXTERN_C_END }

#else

/// @brief It does nothing for the C compiler.
#define EXTERN_C

/// @brief It does nothing for the C compiler.
#define EXTERN_C_BEGIN

/// @brief It does nothing for the C compiler.
#define EXTERN_C_END

#endif
