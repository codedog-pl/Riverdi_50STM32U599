#pragma once

/// @brief Defines a debug output interface.
/// @remark Implementation should define how to physically send the debug text output.
class IDebugOutput
{

public:

    /// @brief Sends a message to the output.
    /// @param index Message index.
    virtual void send(int index) = 0;

};