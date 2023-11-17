#pragma once

#include "main.h"
#include "hal_ex.h"

#define p4def(n,x,y) GPIO_PinTypeDef { GPIO##x, GPIO_PIN_##y, 0, "P4_" #n "_P" #x #y }

class GPIO_test final {

public:
    static void listPins();
    static void testInputs();
    static void testOutputs();

public:
    static inline GPIO_PinTypeDef pins[] = {
        p4def(3,A,5),
        p4def(6,I,5),
        p4def(7,B,6),
        p4def(8,B,7),
        p4def(15,B,10),
        p4def(16,B,5),
        p4def(17,B,0),
        p4def(18,I,4),
        p4def(19,A,15),
        p4def(25,B,15),
        p4def(26,B,1),
        p4def(30,A,7),
        p4def(32,H,11),
        p4def(35,B,2),
        p4def(36,A,8),
        p4def(37,H,5),
        p4def(38,H,4),
        p4def(39,A,10),

        p4def(20,A,11),
        p4def(40,A,12)
    };
    static constexpr size_t pinCount = sizeof(pins) / sizeof(GPIO_PinTypeDef);

};
