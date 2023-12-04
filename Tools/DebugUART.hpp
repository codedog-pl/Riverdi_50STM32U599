#pragma once

#include HAL_HEADER
#include "IDebugOutput.hpp"
#include "ILogMessagePool.hpp"

/// @brief UART port debugger output.
class DebugUART final : public IDebugOutput
{

private:

    /// @brief Creates UART port debugger output.
    /// @param huart UART handle pointer.
    /// @param pool Message pool reference.
    DebugUART(UART_HandleTypeDef* huart, ILogMessagePool& pool) : m_uart(huart), m_pool(pool)
    {
        HAL_UART_RegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID, tx_complete);
        sendNext();
    }

    DebugUART(const DebugUART&) = delete;
    DebugUART(DebugUART&&) = delete;

    ~DebugUART()
    {
        m_pool.clear();
        HAL_UART_UnRegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID);
        m_uart = nullptr;
    }

public:

    static DebugUART* getInstance(UART_HandleTypeDef* huart, ILogMessagePool& pool)
    {
        static DebugUART instance(huart, pool);
        return m_instance = &instance;
    }

    static inline DebugUART* getInstance()
    {
        return m_instance;
    }

    void send(int index) override
    {
        static bool isSending = false;
        if (isSending || index > m_pool.lastIndex()) return;
        isSending = true;
        m_pool.lastSentIndex(m_pool.lastIndex());
        LogMessage* message = m_pool[index];
        auto [buffer, length] = message->buffer();
        if (!length) return;
        if (m_uart) HAL_UART_Transmit_DMA(m_uart, buffer, length);
        isSending = false;
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
        if (!message->buffer().second) return;
        message->clear();
        m_instance->sendNext();
    }

private:

    UART_HandleTypeDef* m_uart;                 // Configured UART handle pointer.
    ILogMessagePool& m_pool;                    // Log message pool reference.
    static inline DebugUART* m_instance = {};   // Singleton instance pointer for static methods.

};
