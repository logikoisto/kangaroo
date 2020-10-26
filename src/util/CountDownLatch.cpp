#include "CountDownLatch.h"

using namespace std;
using namespace zoo;
using namespace zoo::ants;

CountDownLatch::CountDownLatch(int count) : m_count(count), m_mutex(),m_condition(m_mutex)
{
}

void CountDownLatch::wait()
{
     
    MutexGuard lk(m_mutex);
    while (m_count>0)
    {
        m_condition.wait();
    }
    
}

void CountDownLatch::countDown(){
    MutexGuard lk(m_mutex);
    m_count--;
    if(m_count==0){
        m_condition.notifyAll();
    }
    
}
