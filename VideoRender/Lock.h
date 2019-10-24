#ifndef __LOCK_H__
#define __LOCK_H__

#include <memory>
#include <windows.h>

namespace util {
	
	class CCriticalSection {
	public:
		explicit CCriticalSection();
		explicit CCriticalSection(const CCriticalSection& cs);
		~CCriticalSection();

		void lock();
		void unlock();

	private:
		std::shared_ptr<CRITICAL_SECTION>	m_upcs;
	};

	template<class T> 
	class CAutoLock {
	public:
		explicit CAutoLock() {
			printf("%s default constructor\n",__FUNCTION__);
			m_lock.lock();
		}
		explicit CAutoLock(T& lock):m_lock(lock) {
			printf("%s copy constructor\n",__FUNCTION__);
			m_lock.lock();
		}
		~CAutoLock() {
			printf("%s\n", __FUNCTION__);
			m_lock.unlock();
		}

	private:
		T& m_lock;
	};

}//namespace util

#endif
