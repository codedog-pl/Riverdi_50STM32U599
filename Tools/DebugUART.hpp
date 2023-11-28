#pragma once

#include HAL_HEADER
#include "IDebugOutput.hpp"
#include "ILogMessagePool.hpp"

class DebugUART final : public IDebugOutput
{

public:

    DebugUART(UART_HandleTypeDef* huart, ILogMessagePool& pool) : m_uart(huart), m_pool(pool), m_lastSent(0)
    {
        HAL_UART_RegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID, tx_complete);
        sendNext();
    }

    DebugUART(const DebugUART&) = delete;
    DebugUART(DebugUART&&) = delete;

    ~DebugUART()
    {
        m_pool.clear();
        m_lastSent = -1;
        HAL_UART_UnRegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID);
        m_uart = nullptr;
    }

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
        m_lastSent = m_pool.lastIndex();
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
        if (m_lastSent < m_pool.lastIndex()) send(++m_lastSent);
        else { m_pool.clear(); m_lastSent = 0; }
    }

    /// @brief Called when the UART completes sending the message.
    /// @param huart UART handle pointer.
    static void tx_complete(UART_HandleTypeDef* huart)
    {
        if (!m_instance) return;
        auto message = m_instance->m_pool[(size_t)m_instance->m_lastSent];
        if (!message->buffer().second) return;
        message->clear();
        m_instance->sendNext();
    }

private:

    UART_HandleTypeDef* m_uart;                 // Configured UART handle pointer.
    ILogMessagePool& m_pool;                    // Log message pool reference.
    int m_lastSent;                             // Last sent message index.
    static inline DebugUART* m_instance = {};   // Singleton instance pointer for static methods.

};
