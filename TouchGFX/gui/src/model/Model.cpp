#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "HMI.hpp"

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    static bool isFirstTick = true;
    if (isFirstTick)
    {
        isFirstTick = false;
        HMI::init(HMI_DISPLAY);
    }
}
