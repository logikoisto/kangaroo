#ifndef KANGAROON_UTIL_COUNT_DOWN_LATH_H_
#define KANGAROON_UTIL_COUNT_DOWN_LATH_H_

#include "mutex.h"
#include "condition.h"

namespace zoo
{
    namespace kangaroo
    {
        class CountDownLatch
        {
        public:
            explicit CountDownLatch(int count);
            void wait();
            void countDown();
            inline int getCount() const
            {
                 MutexGuard lk(mutex_);
                return m_count;
            }
        private:
            mutable Mutex mutex_;
            Condition condition_;
            int count_;
        };
    } // namespace kangaroo
    
} // namespace zoo

#endif