#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <gui_generated/containers/BackgroundBase.hpp>

class Background : public BackgroundBase
{
public:
    Background();
    virtual ~Background() {}

    virtual void initialize();
protected:
};

#endif // BACKGROUND_HPP
