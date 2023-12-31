/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <touchgfx/Color.hpp>

Screen2ViewBase::Screen2ViewBase() :
    backBtnClickCallback(this, &Screen2ViewBase::backBtnClickCallbackHandler)
{
    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setXY(0, 0);
    add(background);

    backBtn.setXY(0, 386);
    backBtn.setClickCallback(backBtnClickCallback);
    add(backBtn);

    header.setXY(15, 15);
    add(header);
}

Screen2ViewBase::~Screen2ViewBase()
{

}

void Screen2ViewBase::setupScreen()
{
    background.initialize();
    backBtn.initialize();
    header.initialize();
}

void Screen2ViewBase::backBtnClickCallbackHandler()
{
    //Interaction1
    //When backBtn Click change screen to Screen1
    //Go to Screen1 with no screen transition
    application().gotoScreen1ScreenNoTransition();
}
