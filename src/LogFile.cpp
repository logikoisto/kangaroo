#include "LogFile.h"
#include <stdio.h>
#include <ctype.h>
#include <sys/mman.h> /*mmap munmap*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <assert.h>
using namespace std;
using namespace zoo;
using namespace zoo::kangaroo;

MMapFileWriter::MMapFileWriter(const string &baseName, int memSize)
{
    assert(!baseName.empty() && memSize > 0);
    m_memSize = memSize;
    m_writed=0;
    if (m_fd >= 0)
    {
        close(m_fd);
    }
    m_fd = open(baseName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (m_fd < 0)
    {
        fprintf(stderr, "open new file failed,errno=%d", errno);
    }
    else
    {
        ftruncate(m_fd, memSize);
        m_buffer = (char *)mmap(NULL, memSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
        if (m_buffer == MAP_FAILED)
        {
            fprintf(stderr, "mmap file failed,errno=%d", errno);
        }
    }
}

MMapFileWriter::~MMapFileWriter()
{
    if (m_fd >= 0)
    {
        close(m_fd);
        m_fd=-1;
    }
    if (m_buffer != MAP_FAILED)
    {
        munmap(m_buffer, m_memSize);
    }
}

void MMapFileWriter::Append(const char *msg, int len)
{
    if(len>m_memSize-m_writed){
        fprintf(stderr,"mmap memory overflow ,errno=%d", errno);
        return ;
    }
    memcpy(m_buffer+m_writed,msg,len);
    m_writed+=len;
}
void MMapFileWriter::Flush()
{
    if (m_buffer != MAP_FAILED)
    {
        msync(m_buffer, m_memSize, MS_ASYNC);  
    }
    
}
off_t MMapFileWriter::WrittenBytes() const
{
    return m_writed;
}
AppendFileWriter::AppendFileWriter(const std::string& basename, int bufsize)
{
    assert(!basename.empty() && bufsize > 0);
    buf_size = bufsize;
    writed = 0;
    if (fd >= 0){
        close(fd);
    }
    // O_CREAT 要指定访问权限属性? 是的 用户读 写 组 读 其他读
    fd = open(basename.c_str(), O_WRONLY | O_CREAT| O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
    if (fd < 0)
    {
        fprintf(stderr, "open new file failed,errno=%d", errno);
    }
    else
    {
        // 这样创建一个动态的数组 正确吗?  char* buf;
        buf = new char[buf_size]();
    }
}
AppendFileWriter::~AppendFileWriter()
{
    if (fd >= 0)
    {
        close(fd);
        fd=-1;
    }
    // 这可以释放一个动态的数组吗?
    delete [] buf;
}
void AppendFileWriter::Append(const char *msg, int len)
{
    int next_char_idx = 0;
    while (next_char_idx < len)
    {
        // 直接写入msg 减少一次copy开销
        if(buf_size > len - next_char_idx)
        {
            if (write(fd,msg,len) != len){
                fprintf(stderr, "write log file failed,errno=%d", errno);
                return;
            }
            return;
        }
        for (int i = 0; i < buf_size;i++)
        {
            buf[i] = *(msg + next_char_idx);
            next_char_idx++;
        }
        if (write(fd,buf,buf_size) != len){
            fprintf(stderr, "write log file failed,errno=%d", errno);
            return;
        }
    }
}
void AppendFileWriter::Flush()
{
  // 不强制刷新, 始终异步的追加写入
  return;
}