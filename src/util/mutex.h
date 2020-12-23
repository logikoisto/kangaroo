#ifndef KANGAROON_UTIL_MUTEX_H_
#define KANGAROON_UTIL_MUTEX_H_
#include <pthread.h>
namespace zoo {
namespace kangaroon {
class Mutex {
   public:
    Mutex() {
        pthread_mutex_init(&mutex_, NULL);
    }
    ~Mutex() {
        pthread_mutex_destroy(&mutex_);
    }

    void lock();
    void unlock();

    pthread_mutex_t* getMutex() {
        return &mutex_;
    }
   private:
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