/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/HeaderBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

HeaderBase::HeaderBase()
{
    setWidth(204);
    setHeight(26);
    headerTxt.setXY(0, -6);
    headerTxt.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    headerTxt.setLinespacing(0);
    headerTxt.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DTOK));
    add(headerTxt);
}

HeaderBase::~HeaderBase()
{

}

void HeaderBase::initialize()
{

}