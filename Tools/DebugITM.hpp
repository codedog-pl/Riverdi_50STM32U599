#pragma once

#include "hal.h"
#include "IDebugOutput.hpp"
#include "ILogMessagePool.hpp"
#include "OS.hpp"

/// @brief ITM console debug output.
class DebugITM : public IDebugOutput
{

private:

    /// @brief Creates ITM console debug output for the message pool.
    /// @param pool Message pool reference.
    DebugITM(ILogMessagePool& pool) :
        m_pool(pool), m_threadId(0), m_semaphoreId(0), m_isAsync(0), m_isSending(0)
    {
        DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
        ITM->LAR = 0xC5ACCE55;
        ITM->TER = 0x1;
        if (!isITMAvailable()) return;
        for (int i = 0; i < 2; ++i)
        {
            while (!isITMReadyToSend()) __NOP();
            sendITM('\n');
        }
        sendNext(); // In case if the pool already contains unsent messages.
    }

    DebugITM(const DebugITM&) = delete;
    DebugITM(DebugITM&&) = delete;

    ~DebugITM()
    {
        OS::threadDelete(m_threadId);
        OS::semaphoreDelete(m_semaphoreId);
        m_semaphoreId = 0;
        m_threadId = 0;
    }

public:

    /// @brief Creates the ITM debug output instance.
    /// @param pool Message pool reference.
    /// @return Singleton instance.
    static DebugITM* getInstance(ILogMessagePool& pool)
    {
        static DebugITM instance(pool);
        return m_instance = &instance;
    }

    /// @brief Gets the singleton instance of the ITM debug output.
    /// @return Singleton instance.
    static inline DebugITM* getInstance()
    {
        return m_instance;
    }

    /// @returns true if the output is currently available.
    bool isAvailable(void) override
    {
        return isITMAvailable();
    }

    /// @brief Switches to asynchronous operation as soon as the RTOS is started.
    void startAsync(void) override
    {
        if (m_threadId) return;
        m_semaphoreId = OS::semaphoreCreate("DebugITM");
        m_threadId = OS::threadStart("ITM", senderThreadEntry);
    }

    /// @brief Sends a message to the output.
    /// @param index Message index.
    void send(int index) override
    {
        if (m_isSending || index > m_pool.lastIndex() || !isITMAvailable()) return;
        LogMessage* msg = m_pool[index];
        if (!msg || msg->empty()) return;
        m_isSending = true;
        m_pool.lastSentIndex(index);
        if (m_isAsync) sendAsync(); else sendImmediately();
    }

private:

    /// @brief Sends the next message from the pool if available.
    void sendNext()
    {
        if (m_pool.lastSentIndex() < m_pool.lastIndex()) send(m_pool.lastSentIndex(m_pool.lastSentIndex() + 1));
        else m_pool.clear();
    }

    /// @brief Sends the current message immediately in blocking mode.
    void sendImmediately()
    {
        LogMessage& msg = *m_pool.lastSent();
        for (size_t i = 0; i < msg.length(); i++)
        {
            while (!isITMReadyToSend()) __NOP();
            sendITM(msg[i]);
        }
        msg.clear();
        m_isSending = false;
        sendNext();
    }

    /// @brief Sends the current message asynchronously without blocking the calling thread.
    void sendAsync()
    {
        if (m_semaphoreId) OS::semaphoreRelease(m_semaphoreId);
    }

    /// @brief Sends the current message asynchronously if it's ready and yields the thread waiting for the next message.
    static void senderThreadEntry(OS::ThreadEntryArgType)
    {
        m_instance->m_isAsync = true;
        while (m_instance->m_semaphoreId && OS::semaphoreWait(m_instance->m_semaphoreId))
        { // When it gets the signal from `sendAsync()` it starts sending the data with yielding the thread after each character...
            if (m_instance->m_pool.lastSentIndex() < 0) continue;
            LogMessage& msg = *m_instance->m_pool.lastSent();
            if (msg.empty()) continue;
            for (size_t i = 0, n = msg.length(); i < n; ++i)
            {
                while (!isITMReadyToSend()) OS::yield();
                sendITM(msg[i]);
            }
            msg.clear();
            m_instance->m_isSending = false;
            m_instance->sendNext();
        } // Then it waits for the next signal.
        m_instance->m_isAsync = false;
    }

    /// @returns true if the ITM debugger is enabled.
    static inline bool isDebuggerEnabled()
    {
        return ((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & 1UL) != 0UL);
    }

    /// @returns true if the ITM debugger is connected.
    static inline bool isDebuggerConnected()
    {
        return (DCB->DHCSR & 1UL) != 0UL;
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
    OS::ThreadId m_threadId;                    // Sender thread identifier.
    OS::SemaphoreId m_semaphoreId;              // Sender thread release semaphore identifier.
    bool m_isAsync;                             // True if the sender thread is started (asynchronous mode).
    bool m_isSending;                           // True if the sender thread is busy sending a message.

    static inline DebugITM* m_instance = {};    // Singleton instance pointer for static methods.

};
