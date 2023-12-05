#pragma once

#ifdef ITM_HEADER
#include "IDebugOutput.hpp"
#include "ILogMessagePool.hpp"
#include "OS.hpp"
#include ITM_HEADER

/// @brief ITM console debug output.
class DebugITM : public IDebugOutput
{

private:

    /// @brief Creates ITM console debug output for the message pool.
    /// @param pool Message pool reference.
    DebugITM(ILogMessagePool& pool) :
        m_pool(pool), m_senderThreadId(0), m_senderThreadStarted(0), m_events(), m_ptr(nullptr), m_length(0)
    {
        DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
        ITM->LAR = 0xC5ACCE55;
        ITM->TER = 0x1;
        if (!isITMAvailable()) return;
        for (int i = 0; i < 2; ++i)
        {
            while (!isITMReadyToSend()) __NOP();
            sendITM(0);
        }
        sendNext();
    }

    DebugITM(const DebugITM&) = delete;
    DebugITM(DebugITM&&) = delete;

    ~DebugITM()
    {
        m_pool.clear();
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
        if (m_senderThreadId) return;
        OS::eventGroupCreate(m_events, "DebugITM");
        m_senderThreadId = OS::threadStart("ITM", senderThreadEntry);
    }

    /// @brief Sends a message to the output.
    /// @param index Message index.
    void send(int index) override
    {
        if (!isITMAvailable()) return;
        static bool isSending = false;
        if (isSending || index > m_pool.lastIndex()) return;
        isSending = true;
        m_pool.lastSentIndex(m_pool.lastIndex());
        LogMessage* message = m_pool[index];
        auto [buffer, length] = message->buffer();
        if (!length) return;
        if (m_senderThreadStarted) sendAsync(buffer, length);
        else sendImmediately(buffer, length);
        isSending = false;
    }

private:

    /// @brief Sends the next message from the pool if available.
    void sendNext()
    {
        if (m_pool.lastSentIndex() < m_pool.lastIndex()) send(m_pool.lastSentIndex(m_pool.lastSentIndex() + 1));
        else m_pool.clear();
    }

    /// @brief Sends the message immediately in blocking mode.
    /// @param buffer Message buffer.
    /// @param length Message length.
    void sendImmediately(const uint8_t* buffer, size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            while (!isITMReadyToSend()) __NOP();
            sendITM(buffer[i]);
        }
        auto message = m_pool.lastSent();
        if (!message->buffer().second) return;
        message->clear();
        sendNext();
    }

    /// @brief Sends the message asynchronously without blocking the calling thread.
    /// @param buffer Message buffer.
    /// @param length Message length.
    void sendAsync(const uint8_t* buffer, size_t length)
    {
        m_ptr = (uint8_t*)buffer;
        m_length = length;
        OS::eventGroupSignal(m_events, 1);
    }

    /// @brief Sends the current message asynchronously if it's ready and yields the thread waiting for the next message.
    static void senderThreadEntry(OS::ThreadEntryArgType)
    {
        m_instance->m_senderThreadStarted = true;
        while (OS::eventGroupWait(m_instance->m_events))
        {
            if (m_instance->m_ptr && m_instance->m_length)
            {
                for (size_t i = 0; i < m_instance->m_length; i++)
                {
                    while (!isITMReadyToSend()) OS::yield();
                    sendITM(m_instance->m_ptr[i]);
                }
                m_instance->m_ptr = nullptr;
                m_instance->m_length = 0;
                auto message = m_instance->m_pool.lastSent();
                if (!message->buffer().second) continue;
                message->clear();
                m_instance->sendNext();
            }
        }
        m_instance->m_senderThreadStarted = false;
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

private:
    ILogMessagePool& m_pool;                    // Log message pool reference.
    OS::ThreadId m_senderThreadId;              // True if the sender thread is started.
    bool m_senderThreadStarted;                 // True if the sender thread is started.
    OS::EventGroup m_events;                    // Event group to receive asynchronous request start.
    uint8_t* m_ptr;                             // Current message data pointer for asynchronous operation.
    size_t m_length;                            // Current message length for asynchronous operation.
    static inline DebugITM* m_instance = {};    // Singleton instance pointer for static methods.

};

#endif
