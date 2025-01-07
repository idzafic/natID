// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#ifdef USE_BOOST_THREAD
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp> 
namespace cpp11 = boost;
#else
#include <chrono>
#include <thread>
#include <mutex>
namespace cpp11 = std;
#include <condition_variable>
#endif

namespace thread
{
	enum class Status {OK=0, StopRequested=1, FinishConsumingOnEmpty = 2, TimeOut=-1};	

	typedef cpp11::mutex Mutex;
	//typedef cpp11::mutex::scoped_lock ScopedLock;
	typedef cpp11::condition_variable Condition;

	inline void sleepMicroSeconds(int usec)
	{
		cpp11::chrono::duration<double, cpp11::micro> uSec(usec);
		cpp11::this_thread::sleep_for(uSec);	
	}
	
	inline void sleepMilliSeconds(int msec)
	{
		cpp11::chrono::duration<double, cpp11::milli> mSec(msec);
		cpp11::this_thread::sleep_for(mSec);
	}
	
	inline void sleepSeconds(int sec)
	{
		cpp11::chrono::duration<double> Sec(sec);
		cpp11::this_thread::sleep_for(Sec);
	}		
}

