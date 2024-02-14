/**
 * @file        LogITM.hpp
 * @author      Adam Łyskawa
 *
 * @brief       ST ITM console debug output implementation. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "hal.h"
#include "ILogOutput.hpp"
#include "ILogMessagePool.hpp"
#include "OS/Thread.hpp"
#include "OS/Semaphore.hpp"

/// @brief ITM console debug output.
class LogITM final : public ILogOutput
{

private:

    /// @brief Creates ITM console debug output for the message pool.
    /// @param pool Message pool reference.
    LogITM(ILogMessagePool& pool);

    LogITM(const LogITM&) = delete; // Instances should not be copied.

    LogITM(LogITM&&) = delete; // Instances should not be moved.

public:

    /// @brief Creates the ITM debug output instance.
    /// @param pool Message pool reference.
    /// @returns Singleton instance.
    static LogITM* getInstance(ILogMessagePool& pool);

    /// @brief Gets the singleton instance of the ITM debug output.
    /// @returns Singleton instance.
    static inline LogITM* getInstance() { return m_instance; }

    /// @returns true if the output is currently available.
    inline bool isAvailable(void) override { return isITMAvailable(); }

    /// @brief Switches to asynchronous operation as soon as the RTOS is started.
    void startAsync(void) override;

    /// @brief Sends a message to the output.
    /// @param index Message index.
    void send(int index) override;

private:

    /// @brief Sends the next message from the pool if available.
    void sendNext();

    /// @brief Sends the current message immediately in blocking mode.
    void sendImmediately();

    /// @brief Sends the current message asynchronously without blocking the calling thread.
    inline void sendAsync()
    {
        m_semaphore.release();
    }

    /// @brief Sends the current message asynchronously if it's ready and yields the thread waiting for the next message.
    static void senderThreadEntry(OS::ThreadArg);

    /// @returns true if the ITM debugger is enabled.
    static inline bool isDebuggerEnabled()
    {
        return ((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & 1UL) != 0UL);
    }

    /// @returns true if the ITM debugger is connected.
    static inline bool isDebuggerConnected()
    {
#ifndef DCB
        return (CoreDebug->DHCSR & 1UL) != 0UL;
#else
        return (DCB->DHCSR & 1UL) != 0UL;
#endif
    }

    /// @returns true if the ITM debugger is enabled and connected.
    static inline bool isITMAvailable()
    {
        return isDebuggerEnabled() && isDebuggerConnected();
    }

    /// @returns true if the ITM port is ready to receive the next character.
    static inline bool isITMReadyToSend()
    {
        return ITM->PORT[0U].u32 != 0UL;
    }

    /// @brief Immediately and unconditionally sends a character to the ITM port.
    /// @param ch Character to send.
    static inline void sendITM(uint8_t ch)
    {
        ITM->PORT[0U].u8 = ch;
    }

    /// @brief Immediately and unconditionally sends a character to the ITM port.
    /// @param ch A pointer of the character to send.
    static inline void sendITM(uint8_t* ptr)
    {
        ITM->PORT[0U].u8 = *ptr;
    }

private:
    ILogMessagePool& m_pool;                    // Log message pool reference.
    OS::Thread m_thread;                        // Sender thread.
    OS::Semaphore m_semaphore;                  // Sender thread release semaphore.
    bool m_isAsync;                             // True if the sender thread is started (asynchronous mode).
    bool m_isSending;                           // True if the sender thread is busy sending a message.

    static inline LogITM* m_instance = {};      // Singleton instance pointer for static methods.

};
