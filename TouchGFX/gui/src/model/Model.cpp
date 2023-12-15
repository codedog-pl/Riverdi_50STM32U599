#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#if DEVICE
#include "HMI.hpp"
#endif

Model::Model() : modelListener(0) { }

void Model::tick()
{
#if DEVICE
    static bool isFirstTick = true;
    if (isFirstTick)
    {
        isFirstTick = false;
        HMI::init(HMI_DISPLAY);
    }
#endif
}
