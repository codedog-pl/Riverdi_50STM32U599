/**
 * @file        BitFlags.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A set of macros and templates for using `enum` types as bit flags.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <type_traits>

/// @brief Counter base for generating subsequent bit flags.
/// @param base A unique indentifier related to the enumeration.
#define BIT_FLAG_BASE(base) enum { base = __COUNTER__ + 1U };

/// @brief Generates subsequent bit flag for the specified base.
/// @param base A unique indentifier related to the enumeration.
#define BIT_FLAG(base) 1U << (__COUNTER__ - base)

#if 1 // Bitwise operations for `enum` types:

/// @brief A template matching `enum` types only, where TNum is `enum` type name.
#define BIT_FLAGS_TEMPLATE template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value, TEnum>::type* = nullptr>

// Caution:
// Following operators produce integers that might be not equal to any of enumeration member values.
// The operations are only meaningful and valid if the enumeration type is unsigned and the member values
// are such as `x = pow(2, n)`.

/// @brief Negates all bits of the enumeration value.
/// @param a Operand.
/// @returns Bitwise operation result.
BIT_FLAGS_TEMPLATE TEnum operator~(const TEnum a)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return static_cast<TEnum>(~static_cast<TValue>(a));
}

/// @brief Performs bitwise `AND` operation between operands.
/// @param a First operand.
/// @param b Second operand.
/// @returns Bitwise operation result.
BIT_FLAGS_TEMPLATE TEnum operator&(const TEnum a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return static_cast<TEnum>(static_cast<TValue>(a) & static_cast<TValue>(b));
}

/// @brief Performs bitwise `OR` operation between operands.
/// @param a First operand.
/// @param b Second operand.
/// @returns Bitwise operation result.
BIT_FLAGS_TEMPLATE TEnum operator|(const TEnum a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return static_cast<TEnum>(static_cast<TValue>(a) | static_cast<TValue>(b));
}

/// @brief Performs bitwise `XOR` operation between operands.
/// @param a First operand.
/// @param b Second operand.
/// @returns Bitwise operation result.
BIT_FLAGS_TEMPLATE TEnum operator^(const TEnum a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return static_cast<TEnum>(static_cast<TValue>(a) ^ static_cast<TValue>(b));
}

/// @brief Performs bitwise `AND` operation directly on the first operand.
/// @param a First operand reference.
/// @param b Second operand.
/// @returns First operand reference.
BIT_FLAGS_TEMPLATE TEnum& operator&=(TEnum& a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return a = static_cast<TEnum>(static_cast<TValue>(a) & static_cast<TValue>(b));
}

/// @brief Performs bitwise `OR` operation directly on the first operand.
/// @param a First operand reference.
/// @param b Second operand.
/// @returns First operand reference.
BIT_FLAGS_TEMPLATE TEnum& operator|=(TEnum& a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return a = static_cast<TEnum>(static_cast<TValue>(a) | static_cast<TValue>(b));
}

/// @brief Performs bitwise `XOR` operation directly on the first operand.
/// @param a First operand reference.
/// @param b Second operand.
/// @returns First operand reference.
BIT_FLAGS_TEMPLATE TEnum& operator^=(TEnum& a, const TEnum b)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return a = static_cast<TEnum>(static_cast<TValue>(a) ^ static_cast<TValue>(b));
}

#endif

/// @brief Bit flags API.
namespace BF
{

/// @brief Implicitly casts the bit flag enumeration value to bool if any bit of it is set.
/// @param value The value to cast.
/// @returns True if any bit of the value is set.
BIT_FLAGS_TEMPLATE inline bool isAnyBitSet(const TEnum& value)
{
    using TValue = typename std::underlying_type<TEnum>::type;
    return static_cast<TValue>(value) != 0;
}

/// @brief Sets bits.
/// @param what Bits to set.
/// @param where Target.
BIT_FLAGS_TEMPLATE inline void set(TEnum what, TEnum& where)
{
    where |= what;
}

/// @brief Clears bits.
/// @param what Bits to clear.
/// @param where Target.
BIT_FLAGS_TEMPLATE inline void clear(TEnum what, TEnum& where)
{
    where &= ~what;
}

/// @brief Tests if at least one of the bits is set in the target.
/// @param what Bits to test.
/// @param where Target.
/// @param clear 0: Leave target alone. 1: Clear flags specified.
/// @returns True if at least one of the specified bits is set in the `target`, false otherwise.
BIT_FLAGS_TEMPLATE inline bool isSet(TEnum what, TEnum& where, bool clear = false)
{
    if (!clear) return isAnyBitSet(where & what);
    bool result = isAnyBitSet(where & what);
    where &= ~what;
    return result;
}

}
