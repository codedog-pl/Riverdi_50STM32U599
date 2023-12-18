/**
 * @file        LogUART.hpp
 * @author      CodeDog
 *
 * @brief       HAL UART port debug output implementation. Header only.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "hal.h"
#include "ILogOutput.hpp"
#include "ILogMessagePool.hpp"

/// @brief UART port debugger output.
class LogUART final : public ILogOutput
{

private:

    /// @brief Creates UART port debugger output.
    /// @param huart UART handle pointer.
    /// @param pool Message pool reference.
    LogUART(UART_HandleTypeDef* huart, ILogMessagePool& pool) : m_uart(huart), m_pool(pool)
    {
        HAL_UART_RegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID, tx_complete);
        sendNext(); // In case if the pool already contains unsent messages.
    }

    LogUART(const LogUART&) = delete; // Instances should not be copied.

    LogUART(LogUART&&) = delete; // Instances should not be moved.

    /// @brief Unregisters the HAL UART callback.
    ~LogUART()
    {
        HAL_UART_UnRegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID);
        m_uart = nullptr;
    }

public:

    /// @brief Creates the UART debug output instance.
    /// @param pool Message pool reference.
    /// @return Singleton instance.
    static LogUART* getInstance(UART_HandleTypeDef* huart, ILogMessagePool& pool)
    {
        static LogUART instance(huart, pool);
        return m_instance = &instance;
    }

    /// @brief Gets the singleton instance of the UART debug output.
    /// @return Singleton instance.
    static inline LogUART* getInstance()
    {
        return m_instance;
    }

    /// @brief Sends a message to the output.
    /// @param index Message index.
    void send(int index) override
    {
        if (m_isSending || !m_uart || index > m_pool.lastIndex()) return;
        LogMessage* message = m_pool[index];
        if (!message || message->empty()) return;
        m_isSending = true;
        m_pool.lastSentIndex(index);
        auto [buffer, length] = message->buffer();
        HAL_UART_Transmit_DMA(m_uart, buffer, length);
    }

private:

    /// @brief Sends the next message from the pool if available.
    void sendNext()
    {
        if (m_pool.lastSentIndex() < m_pool.lastIndex()) send(m_pool.lastSentIndex(m_pool.lastSentIndex() + 1));
        else m_pool.clear();
    }

    /// @brief Called when the UART completes sending the message.
    /// @param huart UART handle pointer.
    static void tx_complete(UART_HandleTypeDef* huart)
    {
        if (!m_instance) return;
        auto message = m_instance->m_pool.lastSent();
        if (!message || message->empty()) return;
        message->clear();
        m_instance->m_isSending = false;
        m_instance->sendNext();
    }

private:

    UART_HandleTypeDef* m_uart;                 // Configured UART handle pointer.
    ILogMessagePool& m_pool;                    // Log message pool reference.
    bool m_isSending;                           // True if the port DMA is busy sending a message.
    static inline LogUART* m_instance = {};   // Singleton instance pointer for static methods.

};
