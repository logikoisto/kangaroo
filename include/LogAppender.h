#ifndef _ZOO_KANGAROO_LOGAPPENDER_H__
#define _ZOO_KANGAROO_LOGAPPENDER_H__

#include<memory>
#include<vector>
#include<atomic>
 
#include "../src/LogBuffer.h"
#include "src/util/CountDownLatch.h"
#include "Mutex.h"
#include "src/util/Condition.h"
#include "Thread.h"
#include "LogConfig.h"
#include "../src/LogFile.h"
using namespace std;
using namespace zoo;
using namespace zoo::ants;

namespace zoo
{
    namespace kangaroo
    {
        class LogAppender
        {
        public:
            using Ptr = shared_ptr<LogAppender>;

            virtual ~LogAppender() {}
            virtual void append(const char *msg, int len) = 0;
        };

        class ConsoleAppender : public LogAppender
        {
        public:
            virtual void append(const char *msg, int len) override;
        };

        class SocketAppender : public LogAppender
        {
        public:
            virtual void append(const char *msg, int len) override;
        };

        class AsyncFileAppender : public LogAppender
        {
         
              
        public:
            AsyncFileAppender(const string &basename,
                              off_t rollSize,
                              int flushInterval = 3,LogFileWriterType logFileWriterType=LogFileWriterType::APPENDFILE);

            ~AsyncFileAppender()
            {
                if (m_running)
                {
                    stop();
                }
            }

            virtual void append(const char *msg, int len) override;

            void start()
            {
                m_running = true;
                m_thread.start();
                m_latch.wait();
            }

            void stop()
            {
                m_running = false;
                m_cond.notifyOne();
                m_thread.join();
            }

        private:
            void threadFunc();
 
            typedef FixedBuffer<kLargeBuffer> Buffer;
            typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
            typedef BufferVector::value_type BufferPtr;

            const int m_flushInterval;
            std::atomic<bool>  m_running;
            string  m_basename;
            off_t  m_rollSize;
            Thread  m_thread;
            CountDownLatch  m_latch;
            Mutex  m_mutex;
            Condition  m_cond;
            LogFileWriterType m_logFileWriterType;
            BufferPtr  m_currentBuffer;
            BufferPtr  m_nextBuffer;
            BufferVector  m_buffers;
        };
    } // namespace kangaroo
} // namespace zoo
#endif // !1
