/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/BackgroundBase.hpp>
#include <images/BitmapDatabase.hpp>

BackgroundBase::BackgroundBase()
{
    setWidth(800);
    setHeight(480);
    backgroundImg.setXY(0, 0);
    backgroundImg.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_800X480_SHATTERED_RAIN_LIGHT_ID));
    add(backgroundImg);
}

BackgroundBase::~BackgroundBase()
{

}

void BackgroundBase::initialize()
{

}
