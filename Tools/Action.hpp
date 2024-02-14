/**
 * @file        Action.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Function pointer types. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <functional>

/// @brief A pointer to a function that takes no argument and returns no value.
using Action = void(*)(void);

template<typename TArg1> using Ac1 = void(*)(TArg1); // An action taking one argument.
template<typename TArg1, typename TArg2> using Ac2 = void(*)(TArg1, TArg2); // An action taking 2 arguments.
template<typename TArg1, typename TArg2, typename TArg3> using Ac3 = void(*)(TArg1, TArg2, TArg3); // An action taking 3 arguments.
template<typename TArg1, typename TArg2, typename TArg3, typename TArg4> using Ac4 = void(*)(TArg1, TArg2, TArg3, TArg4); // An action taking 4 arguments.

template<typename TResult> using Func = TResult(*)(void); // Basic function pointer.
template<typename TArg1, typename TResult> using Fn1 = TResult(*)(TArg1); // A function taking one argument.
template<typename TArg1, typename TArg2, typename TResult> using Fn2 = TResult(*)(TArg1, TArg2); // A function taking 2 arguments.
template<typename TArg1, typename TArg2, typename TArg3, typename TResult> using Fn3 = TResult(*)(TArg1, TArg2, TArg3); // A function taking 3 arguments.
template<typename TArg1, typename TArg2, typename TArg3, typename TArg4, typename TResult> using Fn4 = TResult(*)(TArg1, TArg2, TArg3, TArg4); // A function taking 4 arguments.

/// @brief A pointer to a function that takes a pointer argument and returns no value.
using BindingAction = void(*)(void*);

/// @brief A pointer to a function that can either take a pointer argument, or no arguments, and returns no value.
union OptionalBindingAction
{
    /// @brief Creates an empty optional binding action.
    OptionalBindingAction() : plain() { }

    /// @brief Creates an optional binding action from a plain action.
    /// @param pAction Plain action function pointer.
    OptionalBindingAction(Action pAction) : plain(pAction) { }

    /// @brief Creates an optional binding action from a binding action.
    /// @param pAction Binding action function pointer.
    OptionalBindingAction(BindingAction pAction) : binding(pAction) { }

    /// @brief Provides `bool` conversions, returns true if the function pointer is set.
    inline operator bool() const { return plain != nullptr; }

    /// @brief A null pointer assignment operator.
    /// @returns This reference.
    inline OptionalBindingAction& operator =(std::nullptr_t) { plain = nullptr; return *this; }

    /// @brief Plain action assignment operator.
    /// @param value A plain action to assign.
    /// @returns This reference.
    inline OptionalBindingAction& operator =(const Action& value) { plain = value; return *this; }

    /// @brief Binding action assignment operator.
    /// @param value A binding action to assign.
    /// @returns This reference.
    inline OptionalBindingAction& operator =(const BindingAction& value) { binding = value; return *this; }

    /// @brief Optional binding action assignment operator.
    /// @param value An optional binding action to assign.
    /// @returns This reference.
    inline OptionalBindingAction& operator =(const OptionalBindingAction& value) { plain = value.plain; return *this; }

    inline bool operator ==(const Action& other) const { return plain == other; }
    inline bool operator !=(const Action& other) const { return plain != other; }
    inline bool operator ==(const BindingAction& other) const { return binding == other; }
    inline bool operator !=(const BindingAction& other) const { return binding != other; }
    inline bool operator ==(const OptionalBindingAction& other) const { return plain == other.plain; }
    inline bool operator !=(const OptionalBindingAction& other) const { return plain != other.plain; }

    Action plain;           // A function pointer for a function that takes no arguments.
    BindingAction binding;  // A function pointer for a function that takes a pointer as an argument.

};
