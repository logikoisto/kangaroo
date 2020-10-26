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