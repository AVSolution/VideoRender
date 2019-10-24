#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "Lock.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <mutex>
#endif

namespace util {

	template<typename T> 
	class CSingleTon {
	public:
		struct CGraba {
			CGraba() {
				printf("%s \n", __FUNCTION__);
			}

			~CGraba() {
				printf("%s \n",__FUNCTION__);
#ifdef WIN32
				CAutoLock<CCriticalSection> autoLock(cs_);
#else
				CAutoLock<std::mutex>	autoLock(mutex_);
#endif
				if (nullptr != pInstance_) {
					delete pInstance_;
					pInstance_ = nullptr;
				}
			}
		};

	public:
		static T* getInstance();

	protected:
		CSingleTon();
		virtual ~CSingleTon();

	private:
		static T* pInstance_;
#ifdef WIN32
		static CCriticalSection cs_;
#else
		static std::mutex mutex_;
#endif
		static std::unique_ptr<CGraba> pGraba_;
	};

	template<typename T>
	T* CSingleTon<T>::pInstance_ = nullptr;
	template<typename T>
	std::unique_ptr<typename CSingleTon<T>::CGraba> CSingleTon<T>::pGraba_ = nullptr;
	template<typename T>
#ifdef WIN32
	CCriticalSection CSingleTon<T>::cs_;
#else
	std::mutex CSingleTon<T>::mutex_;
#endif

	template<typename T>
	CSingleTon<T>::CSingleTon() {
		printf("%s\n", __FUNCTION__);
	}

	template<typename T>
	CSingleTon<T>::~CSingleTon() {
		printf("%s\n",__FUNCTION__);
	}

	template<typename T>
	T* CSingleTon<T>::getInstance() {
		if (nullptr == pInstance_) {
#ifdef WIN32
			CAutoLock<CCriticalSection> autoLock(cs_);
#else
			CAutoLock<std::mutex> autoLock(mutex_);
#endif
			if (nullptr == pInstance_)
				pInstance_ = new T;
		}

		return pInstance_;
	}

#ifdef WIN32
#define IMPLEMENT_SINGLETON(classname)\
	classname* CSingleTon<classname>::pInstance_ = nullptr;\
	std::unique_ptr<CSingleTon<classname>::CGraba> CSingleTon<classname>::pGraba_ = std::unique_ptr<CSingleTon<classname>::CGraba>(new CSingleTon<classname>::CGraba);\
	CCriticalSection CSingleTon<classname>::cs_;
#else 
#define IMPLEMENT_SINGLETON(classname)\
	classname* CSingleTon<classname>::pInstance_ = nullptr; \
	std::unique_ptr<CSingleTon<classname>::CGraba> CSingleTon<classname>::pGraba_ = std::unique_ptr<CSingleTon<classname>::CGraba>(new CSingleTon<classname>::CGraba); \
	std::mutex CSingleTon<classname>::mutex_;
#endif
}//namespace util

#endif