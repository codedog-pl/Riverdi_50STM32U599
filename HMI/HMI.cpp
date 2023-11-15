/**
 * @file        HMI.cpp
 * @author      CodeDog
 * @brief       Main HMI application start.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "HMI.hpp"
#include "tx_api.h"
#include "main.h"
#include "GPIO_test.hpp"

#include <iostream>

void HMI::start()
{
    std::cout << "HMI application started." << std::endl;
    while (!(HMI_SysInit & HMI_FILEX)) tx_thread_sleep(100);
    GPIO_test::listPins();
    GPIO_test::testInputs();
    while (1)
    {
        tx_thread_sleep(1); // for now, let's just yield.
    }
}

// C bindings

extern "C"
{

uint32_t HMI_SysInit = HMI_NONE;

void HMI_Start()
{
   HMI::start();
}

}
