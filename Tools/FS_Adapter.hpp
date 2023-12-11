#pragma once

#if defined(USE_FILEX)
#include "FS_FILEX.hpp"
#define USE_ADAPTER static FILEX adapter;
#else
#include "FS_NullAdapter.hpp"
#define USE_ADAPTER static NullAdapter adapter;
#endif
