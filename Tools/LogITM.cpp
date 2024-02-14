/**
 * @file        LogITM.cpp
 * @author      Adam Łyskawa
 *
 * @brief       ST ITM console debug output implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "LogITM.hpp"

LogITM::LogITM(ILogMessagePool& pool) :
    m_pool(pool), m_thread(), m_semaphore(), m_isAsync(0), m_isSending(0)
{
#ifndef DCB
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#else
    DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
#endif
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

LogITM *LogITM::getInstance(ILogMessagePool &pool)
{
    static LogITM instance(pool);
    return m_instance = &instance;
}

void LogITM::startAsync(void)
{
    if (m_thread.active()) return;
    m_thread.start(nullptr, senderThreadEntry, "ITM", OS::Thread::Priority::low);
    // FIXME: Set the portable thread priority value here!
}

void LogITM::send(int index)
{
    if (m_isSending || index > m_pool.lastIndex() || !isITMAvailable()) return;
    LogMessage* msg = m_pool[index];
    if (!msg || msg->empty()) return;
    m_isSending = true;
    m_pool.lastSentIndex(index);
    if (m_isAsync) sendAsync(); else sendImmediately();
}

void LogITM::sendNext()
{
    if (m_pool.lastSentIndex() < m_pool.lastIndex()) send(m_pool.lastSentIndex(m_pool.lastSentIndex() + 1));
    else m_pool.clear();
}

void LogITM::sendImmediately()
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

void LogITM::senderThreadEntry(OS::ThreadArg)
{
    m_instance->m_isAsync = true;
    while (m_instance->m_semaphore.wait())
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
