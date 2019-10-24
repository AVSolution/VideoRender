#include "Lock.h"

namespace util {
	CCriticalSection::CCriticalSection() {
		printf("%s default constructor \n",__FUNCTION__);
		m_upcs = std::unique_ptr<CRITICAL_SECTION>(new CRITICAL_SECTION);
		InitializeCriticalSection(m_upcs.get());
	}

	CCriticalSection::~CCriticalSection() {
		printf("%s\n",__FUNCTION__);
		DeleteCriticalSection(m_upcs.get());
	}

	CCriticalSection::CCriticalSection(const CCriticalSection& cs) {
		printf("%s copy constructor \n",__FUNCTION__);
		m_upcs = cs.m_upcs;
	}

	void CCriticalSection::lock() {
		printf("%s %p lock\n",__FUNCTION__,m_upcs.get());
		EnterCriticalSection(m_upcs.get());
	}

	void CCriticalSection::unlock() {
		printf("%s %p unlock\n",__FUNCTION__,m_upcs.get());
		LeaveCriticalSection(m_upcs.get());
	}
}//namespace util