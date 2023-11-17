#include "GPIO_test.hpp"
#include "tx_api.h"
#include <iostream>

void GPIO_test::listPins()
{
    std::cout << "Configured pins:" << std::endl;
    for (size_t i = 0; i < pinCount; i++)
    {
        auto pin = pins[i];
        std::cout << i + 1 << ": " << pin.Name << std::endl;
    }
}

void GPIO_test::testInputs()
{
    std::cout << "Setting all pins as GPIO inputs, pulled-up..." << std::endl;
    tx_thread_sleep(1000);
    for (size_t i = 0; i < pinCount; i++)
    {
        auto pin = &pins[i];
        pin->LastState = 1;
        if (pin->Port == GPIOA && (pin->Bit == 11 || pin->Bit == 12)) continue;
        HAL_EX_GPIO_set_mode(pin, GPIO_EX_MODE_INPUT);
        HAL_EX_GPIO_set_pull(pin, GPIO_EX_PULL_UP);
    }
    std::cout << "Done." << std::endl;
    tx_thread_sleep(1000);
    std::cout << "Testing state changed..." << std::endl;
    while (1)
    {
        for (size_t i = 0; i < pinCount; i++)
        {
            auto pin = &pins[i];
            auto state = HAL_EX_PIN_GET_STATE(pin);
            if (state != pin->LastState)
            {
                pin->LastState = state;
                std::cout << "Pin state changed: " << pin->Name << " = " << (state ? "H" : "L") << std::endl;
            }
        }
        tx_thread_sleep(100);
    }
}

void GPIO_test::testOutputs()
{
    std::cout << "Setting all pins as GPIO outputs push-pull..." << std::endl;
    tx_thread_sleep(1000);
    for (size_t i = 0; i < pinCount; i++)
    {
        auto pin = &pins[i];
        HAL_EX_GPIO_set_mode(pin, GPIO_EX_MODE_OUTPUT);
        HAL_EX_GPIO_set_OT(pin, GPIO_EX_OT_PP);
    }
    std::cout << "Done." << std::endl;
    tx_thread_sleep(1000);
    std::cout << "Setting all pins levels to 0..." << std::endl;
    tx_thread_sleep(1000);
    for (size_t i = 0; i < pinCount; i++)
    {
        auto pin = &pins[i];
        std::cout << " - " << pin->Name << "..." << std::endl;
        HAL_EX_PIN_SET_L(pin);
        tx_thread_sleep(1000);
    }
    std::cout << "Done." << std::endl;
    tx_thread_sleep(1000);
    std::cout << "Setting all pins levels to 1..." << std::endl;
    tx_thread_sleep(1000);
    for (size_t i = 0; i < pinCount; i++)
    {
        auto pin = &pins[i];
        std::cout << " - " << pin->Name << "..." << std::endl;
        HAL_EX_PIN_SET_H(pin);
        tx_thread_sleep(1000);
    }
    std::cout << "Done." << std::endl;
}
