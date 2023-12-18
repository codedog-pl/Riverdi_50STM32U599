/**
 * @file        FS_Adapter.hpp
 * @author      CodeDog
 *
 * @brief       Includes the appropriate file system adapter depending on the target configuration.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#if defined(USE_FILEX)
#include "FS_FILEX.hpp"
#define USE_ADAPTER extern FILEXAdapter adapter;
#elif defined(USE_FATFS)
#include "FS_FATFS.hpp"
#define USE_ADAPTER extern FATFSAdapter adapter;
#else
#include "FS_NullAdapter.hpp"
#define USE_ADAPTER extern NullAdapter adapter;
#endif
