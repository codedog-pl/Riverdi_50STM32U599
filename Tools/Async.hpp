/**
 * @file        Async.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Minimalistic asynchronous function helper. Header only.
 *              Defines a set of classes and functions to create, manage and await asynchronous operations.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "AsyncPool.hpp"

namespace Async
{ // Following elements are needed for further declarations.

/// @brief A pre-allocated pool of instance pointers.
static AsyncPool& pool = AsyncPool::getInstance();

/// @brief Discards an asynchronous result.
/// @param pointer Asynchronous result pointer.
inline void discardResult(void* pointer)
{
    if (!pointer) return;
    reinterpret_cast<AsyncResultGeneric*>(pointer)->setAvailable();
}

/// @brief Discards an asynchronous result. Clears the containing pointer.
/// @param pointer A pointer to the result pointer.
inline void discardResult(void** pointer)
{
    if (!pointer || !*pointer) return;
    reinterpret_cast<AsyncResultGeneric*>(*pointer)->setAvailable();
    *pointer = nullptr;
}

} // Async namespace ends for now, it's open later.

/// @brief Base class for both asynchronous state and results.
/// @tparam T Passed value type.
template<typename T>
class AsyncBaseT
{

public:

    /// @brief Initializes the function pointers.
    AsyncBaseT() : m_success(), m_failure() { }

    /// @brief This type cannot be copied.
    AsyncBaseT(const AsyncBaseT&) = delete;

    /// @brief This type cannot be moved.
    AsyncBaseT(AsyncBaseT&&) = delete;

protected:

    void(*m_success)(T);// Successful completion function pointer.
    void(*m_failure)(); // Error completion function pointer.

};

/// @brief Asynchronous result that passes a return value.
/// @tparam T Passed value type.
template<typename T>
class AsyncResultT final : private AsyncBaseT<T>
{

public:

    /// @brief Initializes the function pointers.
    AsyncResultT() : AsyncBaseT<T>() { }

    /// @brief This type cannot be copied.
    AsyncResultT(const AsyncResultT&) = delete;

    /// @brief This type cannot be moved.
    AsyncResultT(AsyncResultT&&) = delete;

    /// @brief Provides a function to be called when the asynchronous operation completes.
    /// @param callback A function that takes the operation result.
    /// @returns This asynchronous result pointer.
    AsyncResultT<T>* then(void(*callback)(T)) { this->m_success = callback; return this; }

    /// @brief Provides a function to be called when the asynchronous operation fails.
    /// @param callback A function that takes no argument.
    /// @returns This asynchronous result pointer.
    AsyncResultT<T>* failed(void(*callback)()) { this->m_failure = callback; return this; }

};

/// @brief Asynchronous state that passes a return value.
/// @tparam T Passed value type.
template<typename T>
class AsyncStateT final : private AsyncBaseT<T>
{

public:

    /// @brief Initializes the function pointers.
    AsyncStateT() : AsyncBaseT<T>() { }

    /// @brief This type cannot be copied.
    AsyncStateT(const AsyncStateT&) = delete;

    /// @brief This type cannot be moved.
    AsyncStateT(AsyncStateT&&) = delete;

    /// @brief Calls the completion function if defined with the value provided. Discards the result after calling continuation.
    /// @param value The value to pass.
    void setValue(T value) { if (this->m_success) this->m_success(value); Async::discardResult(this); }

    /// @brief Calls the error function if defined. Discards the result after calling continuation.
    void fail() { if (this->m_failure) this->m_failure(); Async::discardResult(this); }

};

/// @brief Base class for both asynchronous state and result that doesn't pass a return value.
template<>
class AsyncBaseT<void>
{

public:

    /// @brief Initializes the function pointers.
    AsyncBaseT() : m_success(), m_failure() { }

    /// @brief This type cannot be copied.
    AsyncBaseT(const AsyncBaseT&) = delete;

    /// @brief This type cannot be moved.
    AsyncBaseT(AsyncBaseT&&) = delete;

protected:

    void(*m_success)(); // Successful completion function pointer.
    void(*m_failure)(); // Error completion function pointer.

};

/// @brief Asynchronous result that doesn't pass a return value.
template<>
class AsyncResultT<void> final : private AsyncBaseT<void>
{

public:

    /// @brief Initializes the function pointers.
    AsyncResultT() : AsyncBaseT<void>() { }

    /// @brief This type cannot be copied.
    AsyncResultT(const AsyncResultT&) = delete;

    /// @brief This type cannot be moved.
    AsyncResultT(AsyncResultT&&) = delete;

    /// @brief Provides a function to be called when the asynchronous operation completes.
    /// @param callback A function that takes no argument.
    /// @returns This asynchronous result pointer.
    AsyncResultT<void>* then(void(*callback)()) { this->m_success = callback; return this; }

    /// @brief Provides a function to be called when the asynchronous operation fails.
    /// @param callback A function that takes no argument.
    /// @returns This asynchronous result pointer.
    AsyncResultT<void>* failed(void(*callback)()) { this->m_failure = callback; return this; }

};

/// @brief Asynchronous state that doesn't pass a return value.
template<>
class AsyncStateT<void> final : private AsyncBaseT<void>
{

public:

    /// @brief Initializes the function pointers.
    AsyncStateT() : AsyncBaseT<void>() { }

    /// @brief This type cannot be copied.
    AsyncStateT(const AsyncStateT&) = delete;

    /// @brief This type cannot be moved.
    AsyncStateT(const AsyncStateT&&) = delete;

    /// @brief Calls the completion function if defined. Discards the result after calling continuation.
    void complete() { if (this->m_success) this->m_success(); Async::discardResult(this); }

    /// @brief Calls the error function if defined. Discards the result after calling continuation.
    void fail() { if (this->m_failure) this->m_failure(); Async::discardResult(this); }

};

/// @brief Asynchronous state that doesn't pass a return value.
using AsyncState = AsyncStateT<void>;

/// @brief Asynchronous result that doesn't pass a return value.
using AsyncResult = AsyncResultT<void>;

namespace Async
{ // API:

/// @brief Creates a new asynchronous result that doesn't pass a value.
/// @returns Asynchronous result pointer.
inline AsyncResult* createResult() {
    auto instance = pool.take();
    return new(instance) AsyncResult;
}

/// @brief Creates a new asynchronous result that passes a value.
/// @tparam T Type of the value to pass.
/// @returns Asynchronous result pointer.
template<typename T>
inline AsyncResultT<T>* createResult()
{
    auto instance = pool.take();
    return new(instance) AsyncResultT<T>;
}

/// @brief Gets the asynchronous state from the asynchronous result pointer.
/// @param pointer Asynchronous result pointer.
/// @returns A pointer to the asynchronous state (the same pointer, reinterpretted).
inline AsyncState* getState(void* pointer)
{
    return reinterpret_cast<AsyncState*>(pointer);
}

/// @brief Gets the asynchronous state from the asynchronous result pointer.
/// @tparam T Passed value type.
/// @param pointer Asynchronous result pointer.
/// @returns A pointer to the asynchronous state (the same pointer, reinterpretted).
template<typename T>
inline AsyncStateT<T>* getState(void* pointer)
{
    return reinterpret_cast<AsyncStateT<T>*>(pointer);
}

/// @brief Completes the operation associated with the result if applicable, then clears the containing pointer.
/// @param result A pointer to the result pointer.
inline void complete(AsyncResult** result)
{
    if (!result || !*result) return;
    getState(*result)->complete();
    *result = nullptr;
}

/// @brief  Completes the operation associated with the result if applicable,
///         passes a return value, then clears the containing pointer.
/// @tparam T Passed value type.
/// @param result A pointer to the result pointer.
/// @param value A value to pass.
template<typename T>
inline void setValue(AsyncResultT<T>** result, T value)
{
    if (!result || !*result) return;
    getState<T>(*result)->setValue(value);
    *result = nullptr;
}

/// @brief Fails the operation associated with the result if applicable, then clears the containing pointer.
/// @param result A pointer to the result pointer.
inline void fail(AsyncResult** result)
{
    if (!result || !*result) return;
    getState(*result)->fail();
    *result = nullptr;
}

/// @brief Fails the operation associated with the result if applicable, then clears the containing pointer.
/// @tparam T Passed value type.
/// @param result A pointer to the result pointer.
template<typename T>
inline void fail(AsyncResultT<T>** result)
{
    if (!result || !*result) return;
    getState<T>(*result)->fail();
    *result = nullptr;
}

};
