/**
 * @file        LogUART.hpp
 * @author      Adam Łyskawa
 *
 * @brief       HAL UART port debug output implementation. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
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
    LogUART(UART_HandleTypeDef* huart, ILogMessagePool& pool);

    LogUART(const LogUART&) = delete; // Instances should not be copied.
    LogUART(LogUART&&) = delete; // Instances should not be moved.

    /// @brief Unregisters the HAL UART callback.
    ~LogUART();

public:

    /// @brief Creates the UART debug output instance.
    /// @param pool Message pool reference.
    /// @returns Singleton instance.
    static LogUART* getInstance(UART_HandleTypeDef* huart, ILogMessagePool& pool);

    /// @brief Gets the singleton instance of the UART debug output.
    /// @returns Singleton instance.
    static inline LogUART* getInstance() { return m_instance; }

    /// @brief Sends a message to the output.
    /// @param index Message index.
    void send(int index) override;

private:

    /// @brief Sends the next message from the pool if available.
    void sendNext();

    /// @brief Called when the UART completes sending the message.
    /// @param huart UART handle pointer.
    static void tx_complete(UART_HandleTypeDef* huart);

private:

    UART_HandleTypeDef* m_uart;                 // Configured UART handle pointer.
    ILogMessagePool& m_pool;                    // Log message pool reference.
    bool m_isSending;                           // True if the port DMA is busy sending a message.
    static inline LogUART* m_instance = {};   // Singleton instance pointer for static methods.

};
