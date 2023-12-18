#pragma once

#include <cstdint>
#include "Action.hpp"
#include "Crash.hpp"
#include "OS.hpp"
#include "Thread.hpp"

/// @brief A wait handle for delayed continuation.
struct WaitHandle final
{

    /// @brief Creates an empty (free) wait handle.
    WaitHandle() : action(), binding(), threadId(), ticks(), reset(), isTerminating() { }

    /// @brief Creates the waiting thread and starts it.
    void waitAsync(void)
    {
        threadId = OS::threadStart("Delay", start);
        if (!threadId) Crash::here(); // Thread pool depleted!
    }

    /// @brief Cancels the wait handle.
    void cancel()
    {
        if (isTerminating) return;
        isTerminating = true;
        if (threadId) OS::threadDelete(threadId);
        threadId = 0;
        action.plain = nullptr;
        binding = nullptr;
        ticks = 0;
        reset = 0;
        isTerminating = false;
    }

friend class Delay;
private:

    /// @brief Delay thread start function. Starts, waits and calls the action in the end.
    /// @param arg Wait handle pointer.
    static void start(OS::ThreadEntryArgType arg)
    {
        WaitHandle* waitHandle = (WaitHandle*)(void*)arg;
        if (!waitHandle || !waitHandle->action.plain || !waitHandle->threadId) return;
        do
        {
            OS::delay(waitHandle->ticks);
            if (!waitHandle->action.plain) return;
            if (waitHandle->binding) Thread::sync(waitHandle->action.binding, waitHandle->binding);
            else Thread::sync(waitHandle->action.plain);
            waitHandle->ticks = waitHandle->reset;
        }
        while (waitHandle->reset);
        waitHandle->cancel();
    }

    OptionalBindingAction action;   // Continuation action or nullptr for a free / inactive handle.
    void* binding;                  // An optional binding to pass when calling the action.
    OS::ThreadId threadId;          // Action thread identifier (if started).
    uint32_t ticks;                 // Ticks left before calling the action.
    uint32_t reset;                 // A value to reset the `ticks` to after it goes to zero.
    bool isTerminating;             // A value indicating the wait handle wait thread is being terminated.

};
