#ifndef _ZOO_ANTS_COUNTDOWNLATCH_H__
#define _ZOO_ANTS_COUNTDOWNLATCH_H__
 
#include "Mutex.h"
#include "Condition.h"
using namespace std;
namespace zoo
{
    namespace ants
    {
        class CountDownLatch
        {
        public:
            explicit CountDownLatch(int count);
            void wait();
            void countDown();
            inline int getCount() const
            {
                 MutexGuard lk(m_mutex);
                return m_count;
            }
        private:
            mutable Mutex m_mutex;
            Condition m_condition;
            int m_count;
        };
    } // namespace ants

} // namespace zoo
#endif //