#ifndef BACKBTN_HPP
#define BACKBTN_HPP

#include <gui_generated/containers/BackBtnBase.hpp>

class BackBtn : public BackBtnBase
{
public:
    BackBtn();
    virtual ~BackBtn() {}

    virtual void initialize();
protected:
};

#endif // BACKBTN_HPP
