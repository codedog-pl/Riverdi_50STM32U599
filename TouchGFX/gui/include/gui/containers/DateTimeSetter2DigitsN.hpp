#ifndef DATETIMESETTER2DIGITSN_HPP
#define DATETIMESETTER2DIGITSN_HPP

#include <gui_generated/containers/DateTimeSetter2DigitsNBase.hpp>

class DateTimeSetter2DigitsN : public DateTimeSetter2DigitsNBase
{
public:
    DateTimeSetter2DigitsN();
    virtual ~DateTimeSetter2DigitsN() {}

    virtual void initialize();
    void setValue(uint16_t value) override;
protected:
};

#endif // DATETIMESETTER2DIGITSN_HPP
