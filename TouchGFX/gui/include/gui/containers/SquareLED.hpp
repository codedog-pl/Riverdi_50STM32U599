#ifndef SQUARELED_HPP
#define SQUARELED_HPP

#include <gui_generated/containers/SquareLEDBase.hpp>

class SquareLED : public SquareLEDBase
{
public:
    SquareLED();
    virtual ~SquareLED() {}

    virtual void initialize();
protected:
};

#endif // SQUARELED_HPP
