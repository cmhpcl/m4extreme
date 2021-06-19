// LumpedMass.h: interface for the LumpedMass class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_UTILS_THREADS_THREADMONITORIFACE_H
#define _M4EXTREME_UTILS_THREADS_THREADMONITORIFACE_H

#include <vector>

namespace m4extreme {
	namespace Utils {

		void CreateThreadMonitor(int numberThreads);

		void DestroyThreadMonitor();

		void RunThreadMonitor(void* (*function)(void* argument),
			void *argument);
		int GetMyThreadID(void);

		int GetNumberofThreads(void);

		int GetDataShare (int my_id,
			int number_threads,
			int number_data,
			int & number_data_start,
			int & number_data_end);

		int GetDataShare (int my_id,
			int number_threads,
			const std::vector<int> & costs,
			int & number_data_start,
			int & number_data_end);

	}
}
#endif /* _M4EXTREME_UTILS_THREADS_THREADMONITORIFACE_H */
