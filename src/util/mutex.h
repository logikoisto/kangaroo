#ifndef KANGAROON_UTIL_MUTEX_H_
#define KANGAROON_UTIL_MUTEX_H_
#include <pthread.h>
namespace zoo {
namespace kangaroon {
class Mutex {
   public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

   private:
    pthread_mutex_t* getMutex();

    pthread_mutex_t mutex_;
};

class MutexGuard {
   public:
    MutexGuard(Mutex& mutex);
    ~MutexGuard();

   private:
    Mutex& mutex_;
};

}  // namespace kangaroon

}  // namespace zoo

#endif