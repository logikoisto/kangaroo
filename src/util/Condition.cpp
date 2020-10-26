#include "Condition.h"

#include <errno.h>
#include <stdint.h>

 
using namespace zoo;
using namespace zoo::ants;
Condition::Condition(Mutex& mutex)
	:m_mutex(mutex) {
	pthread_cond_init(&m_condvar, nullptr);
}

Condition::~Condition() {
	pthread_cond_destroy(&m_condvar);
}

void Condition::wait() {
	pthread_cond_wait(&m_condvar, m_mutex.getMutex());
}

bool Condition::waitForSeconds(time_t seconds) {
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1000000000;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);
	return ETIMEDOUT == pthread_cond_timedwait(&m_condvar, m_mutex.getMutex(), &abstime);
}

void Condition::notifyOne() {
	pthread_cond_signal(&m_condvar);
}

void Condition::notifyAll() {
	pthread_cond_broadcast(&m_condvar);
}
 
