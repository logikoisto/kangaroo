#ifndef _ZOO_KANGAROO_LOGCONFIG_H__
#define _ZOO_KANGAROO_LOGCONFIG_H__
 
namespace zoo{
    namespace kangaroo{
        const int kSmallBuffer = 4000;
        const int kLargeBuffer = 4000*1000;
        const int kBufferCount=16;
        extern enum class LogLevel gLogLevel;
        extern class LogFormat gLogFormat;
    }
}
#endif // !_ZOO_