#ifndef SYSBAR_HPP
#define SYSBAR_HPP

#include <gui_generated/containers/SysBarBase.hpp>
#include "DateTime.hpp"

class SysBar final : public SysBarBase
{

public:

    virtual void initialize();
    void resetTime();
    DateTime& time();

private:

    void updateTime() override;

    DateTime m_currentTime;

};

#endif // SYSBAR_HPP
