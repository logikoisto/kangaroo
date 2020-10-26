
#include "../include/LogAppender.h"
#include <functional>

using namespace std;
using namespace zoo;
using namespace zoo::ants;
using namespace zoo::kangaroo;

AsyncFileAppender::AsyncFileAppender(const string &basename,
                                     off_t rollSize,
                                     int flushInterval,
                                     LogFileWriterType logFileWriterType) : m_flushInterval(flushInterval), m_thread(std::bind(&AsyncFileAppender::threadFunc, this), "kangaroo-Logger"),
                                                                            m_mutex(), m_cond(m_mutex), m_latch(1), m_currentBuffer(new Buffer), m_nextBuffer(new Buffer),
                                                                            m_buffers()
{
    m_basename = basename;
    m_rollSize = rollSize;
    m_logFileWriterType = logFileWriterType;
    m_currentBuffer->bzero();
    m_nextBuffer->bzero();
    m_buffers.reserve(kBufferCount);
}

void AsyncFileAppender::append(const char *msg, int len)
{
    MutexGuard lk(m_mutex);
    if(m_currentBuffer->avail()>len){
        //current buffer is good
        m_currentBuffer->append(msg,len);
    }
    else
    {
        //需要将当前的buffer移动到队列中去
        m_buffers.emplace_back(m_currentBuffer);
        if(m_nextBuffer){
            //将备用的buffer移动过去
            m_currentBuffer = std::move(m_nextBuffer);
        }else{
            //说明需要创建的新的buffer
            m_currentBuffer.reset(new Buffer);
        }
        m_currentBuffer->append(msg, len);
        //通知持久化线程，你该干活了。
        m_cond.notifyOne();
    }

}
//负责刷盘的线程
void AsyncFileAppender::threadFunc()
{
    
}