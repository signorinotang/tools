//
//  vnThread.cpp
//  vnbase
//
//  Created by Wayne on 13-11-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnThread.h"
#include <Windows.h>

_vn_begin

namespace internal {
	class ThreadDataImpl : public ThreadData {
		DWORD m_index;
	public:
		ThreadDataImpl() {
			m_index = ::TlsAlloc();
		}

		virtual ~ThreadDataImpl() {
			::TlsFree(m_index);
		}

		virtual void set(void *value) {
			::TlsSetValue(m_index, value);
		}

		virtual void * get() {
			return ::TlsGetValue(m_index);
		}
	};

	class ThreadImpl : public Thread {
		::HANDLE m_thread;
	public:
		ThreadImpl() : m_thread(0) {}
		virtual bool join(u32 *retval) {
			if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_thread, INFINITE)) {
				return false;
			}
			if (retval) {
				DWORD code = 0;
				::GetExitCodeThread(m_thread, &code);
				*retval = (u32)code;
			}
			::CloseHandle(m_thread);
			delete this;
			return true;
		}

		virtual void detach() {
			::CloseHandle(m_thread);
			delete this;
		}

		virtual long tid() {
			return ::GetThreadId(m_thread);
		}

		static DWORD WINAPI routine(LPVOID param) {
			function<u32()>::impl_type *func = static_cast<function<u32()>::impl_type *>(param);
			DWORD ret = (DWORD)(*func)();
			func->drop();
			::ExitThread(ret);
			// keep compiler happy.
			return 0;
		}

		static Thread * create(const function<u32()> &func) {
			function<u32()>::impl_type *p = func.impl_ptr();
			if (!p) {
				return 0;
			}
			p->grab();
			ThreadImpl *thread = vnnew ThreadImpl();
			thread->m_thread = ::CreateThread(NULL, 0, routine, p, 0, NULL);
			if (!thread->m_thread) {
				delete thread;
				return 0;
			}
			return thread;
		}
	};
}

ThreadData * ThreadData::create() {
	return vnnew internal::ThreadDataImpl();
}

Thread * Thread::create(const function<u32()> &func) {
	return internal::ThreadImpl::create(func);
}

void Thread::sleep(u32 milliseconds) {
	::Sleep(milliseconds);
}

long Thread::current_tid() {
	return ::GetCurrentThreadId();
}



_vn_end