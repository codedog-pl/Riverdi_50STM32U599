/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <touchgfx/Color.hpp>

Screen1ViewBase::Screen1ViewBase() :
    fwdBtnClickCallback(this, &Screen1ViewBase::fwdBtnClickCallbackHandler)
{
    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setXY(0, 0);
    add(background);

    fwdBtn.setXY(510, 386);
    fwdBtn.setClickCallback(fwdBtnClickCallback);
    add(fwdBtn);

    header.setXY(15, 15);
    add(header);

    dateTimeSetter1.setXY(160, 104);
    add(dateTimeSetter1);
}

Screen1ViewBase::~Screen1ViewBase()
{

}

void Screen1ViewBase::setupScreen()
{
    background.initialize();
    fwdBtn.initialize();
    header.initialize();
    dateTimeSetter1.initialize();
}

void Screen1ViewBase::fwdBtnClickCallbackHandler()
{
    //Interaction1
    //When fwdBtn Click change screen to Screen2
    //Go to Screen2 with no screen transition
    application().gotoScreen2ScreenNoTransition();
}
