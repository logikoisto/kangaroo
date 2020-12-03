#ifndef KANGAROON_UTIL_SINGLETON_H_
#define KANGAROON_UTIL_SINGLETON_H_

#include <pthread.h>
namespace zoo {
namespace kangaroon {

template <typename T>
class Singleton  {
   public:
    static T* getInstance() {
        pthread_once(&once_control, [&]() {
            value_ = new T();
            ::atexit(destroy);
        });
        return value_;
    }
    static void destroy() { delete value_; }

   private:
    Singleton();
    ~Singleton();

    static T* value_;
    static pthread_once_t once_control;
};

template <typename T>
pthread_once_t Singleton<T>::once_control = PTHREAD_ONCE_INIT;

template <typename T>
T* Singleton<T>::value_;

}  // namespace kangaroon
}  // namespace zoo
#endif