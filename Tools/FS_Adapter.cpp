/**
 * @file        FS_Adapter.cpp
 * @author      CodeDog
 *
 * @brief       Contains the instance of the appropriate file system adapter depending on the target configuration.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#include "FS_Adapter.hpp"

namespace FS
{

#if defined(USE_FILEX)
FILEX adapter;
#else
NullAdapter adapter;
#endif

}