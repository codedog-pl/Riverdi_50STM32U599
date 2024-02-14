#pragma once

#include "bindings.h"
#include "hal_target.h"
EXTERN_C_BEGIN
#include "dac.h"
#include HAL_HEADER
#include HAL_HEADER_(dac)
#include HAL_HEADER_(dac_ex)
EXTERN_C_END
#include "Tone.hpp"
#include "PCM16S.hpp"

extern DAC_HandleTypeDef hdac1;

using Tone1kHz = Tone<PCM16S, 22050, 1000>;

class DACTest
{

public:

    // DACTest()
    // {
    //     HAL_DAC_Init(&hdac1);
    // }

    void start()
    {
        HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, tone.data<uint32_t*>(), tone.length, DAC_ALIGN_12B_L);
    }

    void stop()
    {
        HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
    }

    // ~DACTest()
    // {
    //     HAL_DAC_DeInit(&hdac1);
    // }

private:

    Tone1kHz tone;

};