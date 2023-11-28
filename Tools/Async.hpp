/**
 * @file        Async.hpp
 * @author      CodeDog
 * @brief       Minimalistic asynchronous function helper.
 *
 * @remarks     Defines a set of classes and functions to create, manage and consume asynchronous operations.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "AsyncPool.hpp"

namespace Async
{

/**
 * @brief A pre-allocated pool of instance pointers.
 */
static AsyncPool& pool = AsyncPool::getInstance();

/**
 * @brief Discards an asynchronous result.
 *
 * @param pointer Asynchronous result pointer.
 */
inline void discardResult(void* pointer)
{
    if (!pointer) return;
    reinterpret_cast<AsyncResultGeneric*>(pointer)->setAvailable();
}

/**
 * @brief Discards an asynchronous result. Clears the containing pointer.
 *
 * @param pointer A pointer to the result pointer.
 */
inline void discardResult(void** pointer)
{
    if (!pointer) return;
    reinterpret_cast<AsyncResultGeneric*>(pointer)->setAvailable();
    *pointer = nullptr;
}

}

/**
 * @class AsyncBaseT
 * @brief Base class for both asynchronous state and results.
 * @tparam T Passed value type.
 */
template<typename T>
class AsyncBaseT
{
public:

    AsyncBaseT() { }
    ~AsyncBaseT() = default;
    AsyncBaseT(const AsyncBaseT&) = delete;
    AsyncBaseT(const AsyncBaseT&&) = delete;

protected:

    void(*m_success)(T){};  // Successful completion function pointer.
    void(*m_failure)(){};   // Error completion function pointer.
};

/**
 * @brief Asynchronous result that passes a return value.
 * @tparam T Passed value type.
 */
template<typename T>
class AsyncResultT : private AsyncBaseT<T>
{
public:

    AsyncResultT() : AsyncBaseT<T>() { }
    AsyncResultT(const AsyncResultT&) = delete;
    AsyncResultT(const AsyncResultT&&) = delete;

    /**
     * @brief Provides a function to be called when the asynchronous operation completes.
     *
     * @param callback Callback accepting the operation result.
     * @return This asynchronous result.
     */
    AsyncResultT<T>* then(void(*callback)(T)) { this->m_success = callback; return this; }

    /**
     * @brief Provides a function to be called when the asynchronous operation fails.
     *
     * @param callback Void callback.
     * @return This asynchronous result.
     */
    AsyncResultT<T>* failed(void(*callback)()) { this->m_failure = callback; return this; }

};

/**
 * @brief Asynchronous state that passes a return value.
 *
 * @tparam T Passed value type.
 */
template<typename T>
class AsyncStateT : private AsyncBaseT<T>
{
public:

    AsyncStateT() : AsyncBaseT<T>() { }
    AsyncStateT(const AsyncStateT&) = delete;
    AsyncStateT(const AsyncStateT&&) = delete;

    /**
     * @brief   Calls the completion function if defined with the value provided.
     *          Deletes the state object afterwards.
     *
     * @param value A value to pass.
     */
    void setValue(T value) { if (this->m_success) this->m_success(value); Async::discardResult(this); }

    /**
     * @brief Calls the error function if defined. Deletes the state object afterwards.
     */
    void fail() { if (this->m_failure) this->m_failure(); Async::discardResult(this); }
};

/**
 * @brief Base class for both asynchronous state and result that doesn't pass a return value.
 */
template <>
class AsyncBaseT<void>
{
public:

    AsyncBaseT() { }
    AsyncBaseT(const AsyncBaseT&) = delete;
    AsyncBaseT(const AsyncBaseT&&) = delete;

protected:

    void(*m_success)(); // Successful completion function pointer.
    void(*m_failure)(); // Error completion function pointer.
};

/**
 * @class AsyncResultT
 * @brief Asynchronous result that doesn't pass a return value.
 */
template <>
class AsyncResultT<void> : private AsyncBaseT<void>
{
public:

    AsyncResultT() : AsyncBaseT<void>() { }
    AsyncResultT(const AsyncResultT&) = delete;
    AsyncResultT(const AsyncResultT&&) = delete;

    /**
     * @brief Provides a function to be called when the asynchronous operation completes.
     *
     * @param callback Void callback.
     * @return This asynchronous result.
     */
    AsyncResultT<void>* then(void(*callback)()) { this->m_success = callback; return this; }

    /**
     * @brief Provides a function to be called when the asynchronous operation fails.
     *
     * @param callback Void callback.
     * @return This asynchronous result.
     */
    AsyncResultT<void>* failed(void(*callback)()) { this->m_failure = callback; return this; }

};

/**
 * @brief Asynchronous state that doesn't pass a return value.
 */
template <>
class AsyncStateT<void> : private AsyncBaseT<void>
{
public:

    AsyncStateT() : AsyncBaseT<void>() { }
    AsyncStateT(const AsyncStateT&) = delete;
    AsyncStateT(const AsyncStateT&&) = delete;

    /**
     * @brief Calls the completion function if defined. Deletes the state object afterwards.
     */
    void complete() { if (this->m_success) this->m_success(); Async::discardResult(this); }

    /**
     * @brief Calls the error function if defined. Deletes the state object afterwards.
     */
    void fail() { if (this->m_failure) this->m_failure(); Async::discardResult(this); }

};

using AsyncState = AsyncStateT<void>;
using AsyncResult = AsyncResultT<void>;

/**
 * @brief Creates, completes and discards asynchronous results.
 */
namespace Async
{

/**
 * @brief Creates a new asynchronous result that doesn't pass a value.
 *
 * @return AsyncResult* Result pointer.
 */
inline AsyncResult* createResult() {
    auto instance = pool.take();
    return new(instance) AsyncResult;
}

/**
 * @brief Creates a new asynchronous result that passes a value.
 *
 * @tparam T Type of the value to pass.
 * @return AsyncResultT<T>*
 */
template<typename T>
inline AsyncResultT<T>* createResult()
{
    auto instance = pool.take();
    return new(instance) AsyncResultT<T>;
}

/**
 * @brief Gets the asynchronous state from the asynchronous result pointer.
 *
 * @param pointer Asynchronous result pointer.
 * @return AsyncState* A pointer to the asynchronous state (the same pointer, reinterpretted).
 */
inline AsyncState* getState(void* pointer)
{
    return reinterpret_cast<AsyncState*>(pointer);
}

/**
 * @brief Gets the asynchronous state from the asynchronous result pointer.
 *
 * @tparam T Passed value type.
 * @param pointer Asynchronous result pointer.
 * @return AsyncStateT<T>* A pointer to the asynchronous state (the same pointer, reinterpretted).
 */
template<typename T>
inline AsyncStateT<T>* getState(void* pointer)
{
    return reinterpret_cast<AsyncStateT<T>*>(pointer);
}

/**
 * @brief Completes the operation associated with the result if applicable, clears the containing pointer.
 *
 * @param result A pointer to the result pointer.
 */
inline void complete(AsyncResult** result)
{
    if (!result || !*result) return;
    getState(*result)->complete();
    *result = nullptr;
}

/**
 * @brief   Completes the operation associated with the result if applicable,
 *          passes a return value, clears the containing pointer.
 *
 * @tparam T Passed value type.
 * @param result A pointer to the result pointer.
 * @param value A value to pass.
 */
template<typename T>
inline void setValue(AsyncResultT<T>** result, T value)
{
    if (!result || !*result) return;
    getState<T>(*result)->setValue(value);
    *result = nullptr;
}

/**
 * @brief Fauls the operation associated with the result if applicable.
 *
 * @param result A pointer to the result pointer.
 */
inline void fail(AsyncResult** result)
{
    if (!result || !*result) return;
    getState(*result)->fail();
    *result = nullptr;
}

/**
 * @brief Fails the operation associated with the result if applicable.
 *
 * @tparam T Passed value type.
 * @param result A pointer to the result pointer.
 */
template<typename T>
inline void fail(AsyncResultT<T>** result)
{
    if (!result || !*result) return;
    getState<T>(*result)->fail();
    *result = nullptr;
}

};
