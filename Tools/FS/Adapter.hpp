/**
 * @file        Adapter.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Includes the appropriate file system adapter depending on the target configuration.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#if defined(USE_FILEX)
#include "AdapterFILEX.hpp"
#define USE_ADAPTER static FS::AdapterFILEX adapter;
#elif defined(USE_FATFS)
#include "AdapterFATFS.hpp"
#define USE_ADAPTER static FS::AdapterFATFS adapter;
#else
#include "AdapterNull.hpp"
#define USE_ADAPTER extern FS::AdapterNULL adapter;
#endif
