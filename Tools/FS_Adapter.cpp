#include "FS_Adapter.hpp"

#if defined(USE_FILEX)
static FS::FILEX adapter;
#else
static FS::NullAdapter adapter;
#endif
