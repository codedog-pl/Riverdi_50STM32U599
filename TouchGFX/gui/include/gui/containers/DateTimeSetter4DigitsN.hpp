#ifndef DATETIMESETTER4DIGITSN_HPP
#define DATETIMESETTER4DIGITSN_HPP

#include <gui_generated/containers/DateTimeSetter4DigitsNBase.hpp>

class DateTimeSetter4DigitsN : public DateTimeSetter4DigitsNBase
{
public:
    DateTimeSetter4DigitsN();
    virtual ~DateTimeSetter4DigitsN() {}

    virtual void initialize();
    void setValue(uint16_t value) override;
protected:
};

#endif // DATETIMESETTER4DIGITSN_HPP
