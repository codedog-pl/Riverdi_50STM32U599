#ifndef SYSBAR_HPP
#define SYSBAR_HPP

#include <gui_generated/containers/SysBarBase.hpp>

class SysBar : public SysBarBase
{
public:
    SysBar();
    virtual ~SysBar() {}

    virtual void initialize();
protected:
};

#endif // SYSBAR_HPP
