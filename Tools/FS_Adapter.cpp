#include "FS_Adapter.hpp"

namespace FS
{

#if defined(USE_FILEX)
FILEX adapter;
#else
NullAdapter adapter;
#endif

}