
#ifndef _ZOO_KANGAROO_LOGLEVEL_H__
#define _ZOO_KANGAROO_LOGLEVEL_H__

#include <stdint.h>

namespace zoo
{
    namespace kangaroo
    {
        enum class LogLevel : uint8_t
        {
            TRACE = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    }
} // namespace zoo
#endif // !1