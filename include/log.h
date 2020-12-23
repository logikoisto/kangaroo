#ifndef KANGAROO_LOG_H_
#define KANGAROO_LOG_H_
#include <string>
#include <map>

#include "log_appender_interface.h"
#include "log_config.h"
#include "mutex.h"
#include "singleton.h"
namespace zoo {
namespace kangaroon {
extern LogConfig kLogConfig;
class Logger {
   public:
    static Logger *getLogger() { return Singleton<Logger>::getInstance(); }
    static void setGlobalConfig(const LogConfig &log_config) {
        kLogConfig = log_config;
    }
    void info(const char *format, ...);
    void debug(const char *format, ...);
    void warn(const char *format, ...);
    void error(const char *format, ...);
    void fatal(const char *format, ...);
    /*需要加锁的*/
    void addAppender(const std::string& appender_name,
                     LogAppenderInterface::Ptr appender);
    void delAppender(const std::string& appender_name);
    void clearAppender();
    
    private:
    void writeLog(LogLevel log_level, const char *file_name,
                  const char *function_name, int32_t line_num, const char *fmt,
                  va_list ap);

   private:
    Mutex mutex_;
    std::map<std::string, LogAppenderInterface::Ptr> appenders_;
};
}  // namespace kangaroon

}  // namespace zoo

// #define log_out(lvl, fmt, args...)                                            \
//     if (lvl >= zoo::kangaroo::kLogConfig.log_level)                           \
//     zoo::kangaroo::Logger::getLogger()->writeLog(lvl, __FILE__, __FUNCTION__, \
//                                                  __LINE__, fmt, ##args)

#define log_fatal(fmt, args...) \
    zoo::kangaroon::Logger::fatal(fmt, ##args)
#define log_error(fmt, args...) \
    zoo::kangaroon::Logger::error(fmt, ##args)
#define log_warn(fmt, args...) \
    zoo::kangaroon::Logger::warn(fmt, ##args)
#define log_debug(fmt, args...) \
    zoo::kangaroon::Logger::debug(fmt, ##args)
#define log_info(fmt, args...) \
    zoo::kangaroon::Logger::info(fmt, ##args)

#endif