/**
 * @file        Thread.hpp
 * @author      CodeDog
 *
 * @brief       Quick thread synchronization tool, header file.
 *
 * @remarks     In order to use:
 *                  - call dispatchLoop() at the end of the main application thread,
 *                  - call tick() from the frame display thread.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "hal_mcu.h"
#include "target.h"
#include "Action.hpp"
#include "Crash.hpp"
#include "OS.hpp"
#include "StaticClass.hpp"
#include <cstdint>

/// @brief Quick thread synchronization tool.
class Thread final
{

    STATIC(Thread)

public:

    /// @brief Thread context type.
    enum Context : uint32_t
    {
        none,           // No context (uninitialized value).
        application,    // Main application thread, `dispatcherLoop` method.
        frame,          // Display thread, synchronized with display frame, `tick` method.
    };

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    static void sync(Action action, Context context = application)
    {
        if (!action || !context) return; // Ignore call if no data to proceed.
        for (uint8_t i = 0; i < max; i++)
        {
            if (scheduled[i].action.plain == action) return; // Action is already scheduled.
            if (!scheduled[i].action.plain) // An empty task is found.
            {
                scheduled[i].action = action;
                scheduled[i].binding = nullptr;
                scheduled[i].context = context;
                ++q;
                if (context == application && dispatchSemaphore) OS::semaphoreRelease(dispatchSemaphore);
                return;
            }
        }
        Crash::here(); // Scheduled tasks queue is full!
    }

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes an argument pointer.
    /// @param argument Pointer to pass to the action.
    /// @param context Target thread context.
    static void sync(BindingAction action, void* argument, Context context = application)
    {
        if (!action || !context) return; // Ignore call if no data to proceed.
        for (uint8_t i = 0; i < max; i++)
        {
            if (scheduled[i].action.binding == action) return; // Action is already scheduled.
            if (!scheduled[i].action.binding) // An empty task is found.
            {
                scheduled[i].action = action;
                scheduled[i].binding = argument;
                scheduled[i].context = context;
                ++q;
                if (context == application && dispatchSemaphore) OS::semaphoreRelease(dispatchSemaphore);
                return;
            }
        }
        Crash::here(); // Scheduled tasks queue is full!
    }

    /// @returns 1: Called from ISR. 0: Not called from ISR.
    static bool isISRContext() { return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0; }

    /// @brief A loop waiting for synchronization events. Call at the end of the main application thread.
    static void dispatchLoop()
    {
        if (!dispatchSemaphore) dispatchSemaphore = OS::semaphoreCreate("Thread::dispatchSemaphore");
        dispatcherStarted = true;
        while (1)
        {
            if (q)
            {
                for (uint8_t i = 0; i < max; i++)
                {
                    ScheduledTask task = scheduled[i];
                    if (!task.action.plain || task.context != application) continue;
                    if (q) --q;
                    if (task.binding) task.action.binding(task.binding);
                    else task.action.plain();
                    scheduled[i].action.plain = nullptr;
                    scheduled[i].binding = nullptr;
                    scheduled[i].context = none;
                }
            }
            OS::semaphoreWait(dispatchSemaphore);
        }
    }

    /// @brief Call from the display thread on frame synchronization.
    static void tick()
    {
        displayStarted = true;
        if (!q) return;
        for (uint8_t i = 0; i < max; i++)
        {
            ScheduledTask task = scheduled[i];
            if (!task.action.plain || task.context != frame) continue;
            if (q) --q;
            if (task.binding) task.action.binding(task.binding);
            else task.action.plain();
            scheduled[i].action.plain = nullptr;
            scheduled[i].binding = nullptr;
            scheduled[i].context = none;
        }
    }

private:

    /// @brief Represent an action bound to a specified thread.
    struct ScheduledTask final
    {
        OptionalBindingAction action;   // Action callback.
        void* binding;                  // Optional action binding.
        Context context;                // Thread context.
        ScheduledTask() : action(), binding(), context(none) { } // Creates a blank action.
    };

    constexpr static const uint8_t max = WTK_TASKS;         // Maximal number of tasks that can be scheduled.
    static inline ScheduledTask scheduled[max] = {};        // Scheduled tasks array.
    static inline uint8_t q = 0;                            // 1: Tasks scheduled. 0: No tasks scheduled.
    static inline bool displayStarted = 0;                  // 1: Display thread is started.
    static inline bool dispatcherStarted = 0;               // 1: Dispatcher thread is started.
    static inline OS::SemaphoreId dispatchSemaphore = 0;    // Dispatch semaphore for the application thread.

};
