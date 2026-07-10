// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SingleApp.h
    @brief Provides a single-instance application guard using Boost interprocess shared memory. */
#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include "td/DateTime.h"
#include "thread/Thread.h"
#include <boost/bind.hpp>


#ifdef TRACE_SINGLE_APP
#include <iostream>
#define TRACE_SINGLE_APP_F(x) std::cout << (x) << std::endl
#define TRACE_SINGLE_APP_F2(x, y) std::cout << (x) << (y) << std::endl
#else
#define TRACE_SINGLE_APP_F(x)
#define TRACE_SINGLE_APP_F2(x, y)
#endif

namespace mu
{
	//using namespace boost::interprocess;
	/// @brief Enforces single-instance execution of an application using Boost interprocess shared memory.
	class SingleApp
	{
		/// @brief Shared memory data block exchanged between application instances.
		struct shmData
		{

			/// @brief Initializes the shared memory data with default values.
			shmData()
				: close(false)
				, msg_in(false)
			{}

			//Mutex to protect access to the queue
			boost::interprocess::interprocess_mutex      mutex; ///< Mutex protecting access to shared data.
			//boost::mutex mutex;

			//Condition to wait when the queue is empty
			boost::interprocess::interprocess_condition  cond_resp; ///< Condition variable signalled by the running instance in response.
			//boost::condition  cond_resp;

			//Condition to wait when the queue is full
			boost::interprocess::interprocess_condition  cond_call; ///< Condition variable signalled by the new instance to call the running one.
			//boost::condition  cond_call;

			//Items to fill
			//char   items[LineSize];
			td::DateTime dt; ///< Timestamp used to coordinate the handshake between instances.

			//Is there any message
			bool close; ///< Flag indicating the running instance should close.

			bool msg_in; ///< Flag indicating a message is pending in shared memory.
		};

		const char* _name;                   ///< Name of the shared memory object.
		shmData* _data;                      ///< Pointer to the mapped shared memory data.
		boost::thread* _shmThread;           ///< Background thread managing the shared memory lifecycle.
		int _status;                         ///< Status code: -1=unknown, 0=not running, 1=already running.
		bool _shmCreatedByThisInstance;      ///< True if this instance created the shared memory object.
		bool _notified;                      ///< True after a notify_one call completes successfully.

	protected:

		/// @brief Attempts to create the named shared memory object.
		/// @return true if the shared memory already existed; false if it was freshly created.
		bool createShm()
		{
			try
			{
				//Create a shared memory object.
				boost::interprocess::shared_memory_object shm(boost::interprocess::create_only, _name, boost::interprocess::read_write);
			}
			catch (boost::interprocess::interprocess_exception &ex)
			{
				TRACE_SINGLE_APP_F(ex.what());
				ex;
				return true;
			}
			return false;
		}

		/// @brief Checks whether the other application instance has responded to a message.
		/// @return true if the message has been consumed (msg_in is false).
		bool appResponded()
		{
			return !_data->msg_in;
		}

		/// @brief Notifies the running instance by signalling the call condition variable.
		void notify()
		{
			_notified = false;
			_data->cond_call.notify_one();
			_notified = true;
		}

		/// @brief Background thread entry point that handles shared memory creation, detection, and coordination.
		void thIsRunning()
		{
			bool shmExisted = createShm();
			_shmCreatedByThisInstance = false;



			if (shmExisted)
			{
				TRACE_SINGLE_APP_F("Shm vec postoji!");
				{
					boost::interprocess::shared_memory_object shm(boost::interprocess::open_only, _name, boost::interprocess::read_write);

					//Map the whole shared memory in this process
					//Map it as read-write
					boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);

					void * addr = region.get_address();

					//Construct the shared structure in memory
					if (!shmExisted)
						_data = new (addr)shmData;
					else
						_data = static_cast<shmData*>(addr);
					boost::system_time timeOutMutex = boost::get_system_time() + boost::posix_time::milliseconds(50);
					boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(_data->mutex, timeOutMutex);
					if (!lock.owns())
					{
						TRACE_SINGLE_APP_F("Brisem shm!!Aplikacija je ranije crashirala...");
						_data->mutex.unlock();
						boost::interprocess::shared_memory_object::remove(_name);
						bool bRet = createShm();
						assert(!bRet);
						shmExisted = false;
						_shmCreatedByThisInstance = true;
					}
				}
			}
			else
			{
				TRACE_SINGLE_APP_F("Shm ne postoji! Aplikacija tek startana");
				_shmCreatedByThisInstance = true;
			}

		pocetak:
			try
			{
				//read-write mode
				boost::interprocess::shared_memory_object shm(boost::interprocess::open_only, _name, boost::interprocess::read_write);

				//Set size
				if (!shmExisted)
					shm.truncate(sizeof(shmData));

				//Map the whole shared memory in this process
				//Map it as read-write
				boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);

				//Get the address of the mapped region
				void * addr = region.get_address();

				//Construct the shared structure in memory
				if (!shmExisted)
					_data = new (addr)shmData;
				else
					_data = static_cast<shmData*>(addr);

				while (true)
				{
					TRACE_SINGLE_APP_F("Entering mutex lock....");

					boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(_data->mutex);
					TRACE_SINGLE_APP_F("Mutex lock obtained....");
					//thread::ScopedLock lock(_data->mutex);
					//if (data->message_in){
					if (shmExisted)
					{
						//if (!m_not_full.timed_wait(lock, boost::posix_time::milliseconds(wait_m_sec), boost::bind(&BoundedBuffer<value_type>::is_not_full, this)))
						//{
						//	return TimeOut;
						//}
						td::DateTime dtNow;
						dtNow.now();

						//td::Time timeNow;
						//timeNow.now();
						_data->msg_in = true;
						_data->dt = dtNow;
						TRACE_SINGLE_APP_F("Calling notify_one....");
						{
							boost::thread threadNotify(boost::bind(&SingleApp::notify, this));
							thread::sleepMilliSeconds(200);

							if (!_notified)
							{
								TRACE_SINGLE_APP_F("Deleting shm!! Application crashed in last usage...");
								_data->mutex.unlock();
								boost::interprocess::shared_memory_object::remove(_name);
								bool bRet = createShm();
								assert(!bRet);
								shmExisted = false;
								_shmCreatedByThisInstance = true;
								goto pocetak;
							}
						}


						TRACE_SINGLE_APP_F("notify_one passed....");
						td::DateTime nextDay(dtNow);
						nextDay.addDays(1);
						boost::system_time timeOut = boost::get_system_time() + boost::posix_time::milliseconds(1000);
						//if (_data->cond_resp.timed_wait(lock, boost::posix_time::milliseconds(5000), boost::bind(&SingleApp::appResponded, this)))
						TRACE_SINGLE_APP_F("Entering timed_wait 1000 ms...");
						if (_data->cond_resp.timed_wait(lock, timeOut)) // , boost::bind(&SingleApp::appResponded, this)))
						{
							TRACE_SINGLE_APP_F("Application responded...");
							if (!_data->msg_in && _data->dt == nextDay)
							{
								_shmCreatedByThisInstance = false;
								TRACE_SINGLE_APP_F("Application has already been started...!");
								_status = 1;
								return;
							}
							else
							{
								_status = 0;
								shmExisted = false;
								_shmCreatedByThisInstance = true;
								TRACE_SINGLE_APP_F("Warning!! Application did not respond correctly...!");
							}
						}
						else
						{
							//timeout
							TRACE_SINGLE_APP_F("Timeout... Application did not respond... Shm will be treated as abondened and will be deleted...");
							_data->mutex.unlock();
							boost::interprocess::shared_memory_object::remove(_name);
							bool bRet = createShm();
							assert(!bRet);
							shmExisted = false;
							_shmCreatedByThisInstance = true;
							goto pocetak;
						}
					}
					else
					{
						//shm did not exist
						_status = 0;
						TRACE_SINGLE_APP_F("Waiting on next start (sleep)...");
						_data->cond_call.wait(lock);
						TRACE_SINGLE_APP_F("Waked...");
						if (_data->close)
						{
							if (_data->msg_in)
							{
								_data->msg_in = false;
								TRACE_SINGLE_APP_F("Application is called to close!");
								return;
							}
							else
							{
								_data->close = false;
								_data->msg_in = false;
							}
						}
						if (_data->msg_in)
						{
							_data->dt.addDays(1);
							TRACE_SINGLE_APP_F2("Responding with: ", _data->dt);
							//int x;
							//std::cin >> x;
							_data->msg_in = false;
							_data->cond_resp.notify_one();
						}

					}
				}
			}
			catch (boost::interprocess::interprocess_exception &ex)
			{
				TRACE_SINGLE_APP_F(ex.what());
				ex;
			}
		}

	public:
		/// @brief Default constructor; initializes all members to safe defaults.
		SingleApp()
			: _name(nullptr)
			, _data(nullptr)
			, _status(-1)
			, _shmCreatedByThisInstance(false)
			, _notified(false)
		{}

		/// @brief Destructor; closes and cleans up the shared memory guard.
		~SingleApp()
		{
			close();
		}

		/// @brief Checks whether another instance of the application is already running.
		/// @param shmName The unique shared memory name that identifies this application.
		/// @return true if another instance is already running; false if this is the first instance.
		bool isRunning(const char* shmName)
		{
			_name = shmName;

			_status = -1;

			_shmThread = new boost::thread(boost::bind(&SingleApp::thIsRunning, this));

			for (int i = 0; i < 50; ++i)
			{
				thread::sleepMilliSeconds(100);
				if (_status == 1)
				{
					_shmThread->join();
					TRACE_SINGLE_APP_F("Deleting thread Pos1... ");
					delete _shmThread;
					_shmThread = nullptr;
					break;
				}
				else if (_status == 0)
				{
					break;
				}
			}

			return (_status == 1);
		}
	protected:
		/// @brief Signals the background thread to shut down and releases shared memory resources.
		void close()
		{
			if (_shmThread == nullptr)
				return;

			{
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(_data->mutex);
				_data->close = true;
				_data->msg_in = true;
				_data->cond_call.notify_one();
			}
			_shmThread->join();

			if (_shmCreatedByThisInstance)
			{
				TRACE_SINGLE_APP_F2("Deleting shm: ", _name);
				boost::interprocess::shared_memory_object::remove(_name);
			}

			TRACE_SINGLE_APP_F("Deleting thread Pos2 ... ");
			delete _shmThread;
			_shmThread = nullptr;
			TRACE_SINGLE_APP_F("Closed... ");
		}

	};
}
