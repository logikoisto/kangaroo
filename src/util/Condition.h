#ifndef _ZOO_ANTS_CONDITION_H_
#define _ZOO_ANTS_CONDITION_H_

#include "Mutex.h"
 

#include <pthread.h>

namespace zoo {

namespace ants{
class Condition   {
public:
	explicit Condition(Mutex& mutex);
	~Condition();

	void wait();
	bool waitForSeconds(time_t seconds);
	void notifyOne();
	void notifyAll();
	
private:
	Mutex& m_mutex;
	pthread_cond_t m_condvar;
};

}


}


#endif
