#ifndef _ZOO_KANGAROO_LOG_H__
#define _ZOO_KANGAROO_LOG_H__

#include <stdarg.h> /* ANSI C header file */
#include <map>
#include "LogLevel.h"
#include "LogAppender.h"
#include "Singleton.h"

 
/*
Logger::setGlobalLogLevel(LogLevel::INFO);
Logger::getInstance()->addAppender("console", LogAppender::ptr(new ConsoleAppender()));
*/
namespace zoo
{
    namespace kangaroo
    {

        class Logger
        {

        public:
            static Logger *getLogger()
            {
                return Singleton<Logger>::getInstance();
            }

            static void setGlobalLogLevel(LogLevel level);
            static LogLevel getGlobalLogLevel();
            void info(const char *format, ...);
            void debug(const char *format, ...);
            void warn(const char *format, ...);
            void error(const char *format, ...);
            void fatal(const char *format, ...);
            /*需要加锁的*/
            void addAppender(const std::string name, LogAppender::Ptr appender);
            void delAppender(const std::string name);
            void clearAppender();

        private:
            
            Logger();
            ~Logger(){};
            void log(LogLevel logLevel,const char *fmt, va_list ap)
            {
                //
                if (logLevel < gLogLevel)
                {
                    return;
                }

                //格式化固有前缀+data
                //固定前缀：日期+时间(精确到微妙)+线程ID+日志级别+日志内容+源文件+行号
                MutexGuard guard(m_mutex);
                for (const auto &pair : m_appenders)
                {
                    pair.second->append(res.data(), res.length());
                }
            }

        private:
            string formatMsg(const char *fmt, va_list ap)
            {
                 string res;

                #ifdef HAVE_VSNPRINTF
                                vsnprintf(res.data(), res.length(), fmt, ap); /* safe */
                #else
                                vsprintf((char *)res.data(), fmt, ap); /* not safe */
                #endif
                                return res;
            }

        private:
            Mutex m_mutex;
            map<std::string, LogAppender::Ptr> m_appenders;
        };
    } // namespace kangaroo
} // namespace zoo


#define log_out(lvl, fmt, args...)  \
        if(zoo::kangaroo::gLogLevel<zoo::kangaroo::Logger::getLogger()->getGlobalLogLevel()) \
            log_center_write(lvl, __FILE__, __FUNCTION__, __LINE__ , fmt, ##args)
#define log_fatal(fmt, args...)     log_out(zoo::kangaroo::LogLevel::FATAL, fmt, ##args) 
#define log_error(fmt, args...)	    log_out(zoo::kangaroo::LogLevel::ERROR, fmt, ##args)
#define log_warn(fmt, args...)	    log_out(zoo::kangaroo::LogLevel::WARN, fmt, ##args)
#define log_debug(fmt, args...)		log_out(zoo::kangaroo::LogLevel::DEBUG, fmt, ##args)
#define log_info(fmt, args...)		log_out(zoo::kangaroo::LogLevel::INFO, fmt, ##args)
#endif