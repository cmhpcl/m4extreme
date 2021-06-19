// LumpedMass.h: interface for the LumpedMass class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_UTILS_THREADS_THREADMONITOR_H
#define _M4EXTREME_UTILS_THREADS_THREADMONITOR_H

#include "cc++/thread.h"


/**
* The ThreadMonitor class is used to create a pool of threads that
* can be used to run a function 
*     void* function(void *argument)
* from multiple threads concurrently.
*
*/

namespace m4extreme {

	namespace Utils {

		/**
		* classes defined in this header file
		*/

		class MonitorThread;
		class ThreadMonitor;

		/**
		* MonitorThread
		*/

		class MonitorThread : public ost::Thread
		{
		private:
			ThreadMonitor *_threadMonitor;
			int            _threadId;

		public:
			MonitorThread() {}
			MonitorThread(ThreadMonitor *threadMonitor, int threadId) : 
			_threadMonitor(threadMonitor), _threadId(threadId) { }
			MonitorThread(const MonitorThread& thread);
			virtual ~MonitorThread() { Terminate(); }
			void Run();
			void*  getExtended() { return reinterpret_cast<void *>(_threadId); }
			int    getThreadId() const { return _threadId; }
		private:
			void   setThreadId();
		};

		/**
		* ThreadMonitor
		*/

		class ThreadMonitor {

		private:
			void*                 (*_function)(void *argument);
			void*                   _argument;
			int                     _numberThreads;
			int                     _work;
			int                     _queue;
			int                     _runners;
			ost::Conditional*       _startCondition;
			ost::Conditional*       _endCondition;
			MonitorThread**         _threads;
			bool *                  _takenData;

			friend class MonitorThread;

		public:

		private:


		public:
			/**
			* constructor
			*/

			ThreadMonitor() : _startCondition(0), _endCondition(0), _threads(0),
				_takenData(0) { }
			ThreadMonitor(int numberThreads);

			/**
			* destructor
			*/

			~ThreadMonitor();

			/**
			* run function with arguments from multiple threads
			*/

			void runFunction(void* (*function)(void *argument),
				void*   argument);
                        
			/**
			* access the number of threads
			*/

			int getNumberThreads() { return _numberThreads; }

		private:
			bool checkTakenData() const;

		};

	}

}

#endif // _M4EXTREME_UTILS_THREADS_THREADMONITOR_H
