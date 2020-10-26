
#ifndef _ZOO_KANGAROO_LOGFILE_H__
#define _ZOO_KANGAROO_LOGFILE_H__
#include <memory>
#include<string>
#include<type_traits>
 
#define DECLARE_HAS_CLASS_MEMBER(NAME)                                                                                                    \
        template <typename T, typename... Args>                                                                                           \
        struct has_member_##NAME                                                                                                          \
        {                                                                                                                                 \
                template <typename U>                                                                                                     \
                constexpr static auto check(const void *) -> decltype(std::declval<U>().NAME(std::declval<Args>()...), std::true_type()); \
                                                                                                                                          \
                template <typename U>                                                                                                     \
                constexpr static std::false_type check(...);                                                                              \
                                                                                                                                          \
                static constexpr bool value = decltype(check<T>(nullptr))::value;                                                         \
        }

#define HAS_CLASS_MEMBER(CLASS, MEMBER, ...) \
        has_member_##MEMBER<CLASS, __VA_ARGS__>::value

namespace zoo
{
        namespace kangaroo
        {
                enum class LogFileWriterType{APPENDFILE,MMAPFILE};
                class AppendFileWriter
                {
                public:
                        AppendFileWriter(std::string basename);
                        void append(const char *msg, int len);
                        void flush();
                        off_t writtenBytes() const;
                };

                class MMapFileWriter
                {
                public:
                        MMapFileWriter(const std::string& basename, int memsize);
                         ~MMapFileWriter();
                        void Append(const char *msg, int len);
                        void Flush();
                        off_t WrittenBytes() const{
                                return m_writed;
                        }
                private:
                    int m_fd;
                    char* m_buffer;
                    int m_memSize;
                    off_t m_writed;
                };

                template <typename T>
                class LogFileWriter
                {
                public:
                
                        LogFileWriter(const std::string &basename,
                                      off_t rollSize,
                                      bool threadSafe = true,
                                      int flushInterval = 3,
                                      int checkEveryN = 1024);
                        ~LogFileWriter();
                        //关联背后具体的文件

                        typename std::enable_if<HAS_CLASS_MEMBER(T, append, const char *msg, int len)>::type append(T &obj, const char *msg, int len)
                        {
                        }

                        typename std::enable_if<HAS_CLASS_MEMBER(T, flush)>::type flush(T &obj)
                        {
                        }

                        bool rollFile();

                private:
                        void append_unlocked(const char *msg, int len);

                        static string getLogFileName(const string &basename, time_t *now);

                        const std::string m_baseName;
                        const off_t m_rollSize;
                        const int m_flushInterval;
                        const int m_checkEveryN;

                        int m_count;

                        std::unique_ptr<MutexLock> m_mutex;
                        time_t m_startOfPeriod_;
                        time_t m_lastRoll_;
                        time_t m_lastFlush;
                        const static int m_kRollPerSeconds_ = 60 * 60 * 24;
                };
        } // namespace kangaroo
} // namespace zoo

#endif