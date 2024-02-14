#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#if DEVICE
#include "HMI.hpp"
#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    static bool isFirstTick = true;
    if (isFirstTick)
    {
        isFirstTick = false;
#if DEVICE
        HMI::init(HMI_DISPLAY);
#endif
    }
}
