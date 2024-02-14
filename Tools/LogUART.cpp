/**
 * @file        LogUART.cpp
 * @author      Adam Łyskawa
 *
 * @brief       HAL UART port debug output implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "LogUART.hpp"

LogUART::LogUART(UART_HandleTypeDef* huart, ILogMessagePool& pool) : m_uart(huart), m_pool(pool)
{
    HAL_UART_RegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID, tx_complete);
    sendNext(); // In case if the pool already contains unsent messages.
}

LogUART::~LogUART()
{
    HAL_UART_UnRegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID);
    m_uart = nullptr;
}

LogUART *LogUART::getInstance(UART_HandleTypeDef *huart, ILogMessagePool &pool)
{
    static LogUART instance(huart, pool);
    return m_instance = &instance;
}

void LogUART::send(int index)
{
    if (m_isSending || !m_uart || index > m_pool.lastIndex()) return;
    LogMessage* message = m_pool[index];
    if (!message || message->empty()) return;
    m_isSending = true;
    m_pool.lastSentIndex(index);
    auto [buffer, length] = message->buffer();
    HAL_UART_Transmit_DMA(m_uart, buffer, length);
}

void LogUART::sendNext()
{
    if (m_pool.lastSentIndex() < m_pool.lastIndex()) send(m_pool.lastSentIndex(m_pool.lastSentIndex() + 1));
    else m_pool.clear();
}

void LogUART::tx_complete(UART_HandleTypeDef *huart)
{
    if (!m_instance) return;
    auto message = m_instance->m_pool.lastSent();
    if (!message || message->empty()) return;
    message->clear();
    m_instance->m_isSending = false;
    m_instance->sendNext();
}
