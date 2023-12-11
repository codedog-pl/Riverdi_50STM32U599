#pragma once

#if defined(USE_FILEX)
#include "FS_FILEX.hpp"
#define USE_ADAPTER extern FILEX adapter;
#else
#include "FS_NullAdapter.hpp"
#define USE_ADAPTER extern NullAdapter adapter;
#endif
