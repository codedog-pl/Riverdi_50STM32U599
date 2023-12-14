/**
 * @file        Thread.hpp
 * @author      CodeDog
 * @brief       Quick thread synchronization tool, header file.
 * @remarks     In order to use:
 *                  - call dispatchLoop() at the end of the main application thread,
 *                  - call tick() from the frame display thread.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "Action.hpp"
#include "OS.hpp"
#include "StaticClass.hpp"
#include <cstdint>

/// @brief Quick thread synchronization tool.
class Thread final
{
public:

    /// @brief Thread context type.
    enum Context : uint32_t
    {
        none,           // No context (uninitialized value).
        application,    // Main application thread, `dispatcherLoop` method.
        frame,          // Display thread, synchronized with display frame, `tick` method.
    };

    STATIC(Thread)

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    static void sync(Action action, Context context = application);

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes an argument pointer.
    /// @param argument Pointer to pass to the action.
    /// @param context Target thread context.
    static void sync(BindingAction action, void* argument, Context context = application);

    /// @returns 1: Called from ISR. 0: Not called from ISR.
    static bool isISRContext();

    /// Displays a debug error message if the function was called from ISR.
    /// Should be compiled conditionally in `debug` builds.
    /// When the error is triggered it's unrecoverable, reset the device, fix the underlying problem.
    static void warnISR();

    /// @brief Call from the display thread on frame synchronization.
    static void tick();

    /// @brief A loop waiting for synchronization events. Call at the end of the main application thread.
    static void dispatchLoop();

protected:

    /// @brief Represent an action bound to a specified thread.
    struct ScheduledTask final
    {
        OptionalBindingAction action;   // Action callback.
        void* binding;                  // Optional action binding.
        Context context;                // Thread context.
        ScheduledTask() : action(), binding(), context(none) { } // Creates a blank action.
    };

    constexpr static const uint8_t max = 32;                // Maximal number of tasks that can be scheduled.
    static inline ScheduledTask scheduled[max]{};           // Scheduled tasks array.
    static inline uint8_t q = 0;                            // 1: Tasks scheduled. 0: No tasks scheduled.
    static inline bool displayStarted = 0;                  // 1: Display thread is started.
    static inline bool dispatcherStarted = 0;               // 1: Dispatcher thread is started.
    static inline OS::SemaphoreId dispatchSemaphore = 0;    // Dispatch semaphore for the application thread.

};
