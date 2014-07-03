//
//  vnThread.cpp
//  vnbase
//
//  Created by Wayne on 13-6-14.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnThread.h"
#include <pthread.h>
#include <unistd.h>

_vn_begin

namespace internal {

	class ThreadDataImpl : public ThreadData {
	public:
        static ThreadData * create() {
            return vnnew ThreadDataImpl();
        }
        
		ThreadDataImpl()
		: m_key(0) {
			pthread_key_create(&m_key, 0);
		}
		~ThreadDataImpl() {
			pthread_key_delete(m_key);
			m_key = 0;
		}
		virtual void * get() {
			return pthread_getspecific(m_key);
		}
		virtual void set(void *value) {
			pthread_setspecific(m_key, value);
		}
	private:
		pthread_key_t m_key;
	};

	class ThreadImpl : public Thread {
	public:
		static Thread * create(const function<u32()> &func) {
			function<u32()>::impl_type *p = func.impl_ptr();
			if (!p) {
				return 0;
			}
			p->grab();
			ThreadImpl *thread = vnnew ThreadImpl();
			if (pthread_create(&thread->m_thread, NULL, routine, p)) {
				delete thread;
				thread = 0;
			}
			return thread;
		}

		static void sleep(u32 milliseconds) {
			usleep((useconds_t)milliseconds * 1000);
		}

		static long current_tid() {
			return reinterpret_cast<long>(pthread_self());
		}

		virtual long tid() {
			return reinterpret_cast<long>(m_thread);
		}
		
		virtual void detach() {
			pthread_detach(m_thread);
			delete this;
		}
		
		virtual bool join(u32 *retval) {
			void * ret = 0;
			if (pthread_join(m_thread, &ret) ) {
				return false;
			}
			if (retval) {
				*retval = *reinterpret_cast<u32 *>(&ret);
			}
			delete this;
			return true;
		}
		
		static void * routine(void *param) {
			function<u32()>::impl_type *func = static_cast<function<u32()>::impl_type *>(param);
			void *ret = 0;
			*reinterpret_cast<u32 *>(&ret) = (*func)();
			func->drop();
			pthread_exit(ret);
			// keep compiler happy.
			return 0;			
		}

		ThreadImpl() : m_thread(0) {}
        
	private:
		pthread_t m_thread;
	};
}

Thread * Thread::create(const function<u32()> &func) {
	return internal::ThreadImpl::create(func);
}

void Thread::sleep(u32 milliseconds) {
	internal::ThreadImpl::sleep(milliseconds);
}

long Thread::current_tid() {
	return internal::ThreadImpl::current_tid();
}

_vn_end