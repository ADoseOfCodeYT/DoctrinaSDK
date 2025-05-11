#ifndef _DETIMER_H_
#define _DETIMER_H_

#include <chrono>

// basic high resolution timer based on https://github.com/turanszkij/WickedEngine/blob/master/WickedEngine/wiTimer.h

namespace de
{
    struct Timer
	{
		std::chrono::high_resolution_clock::time_point TimeStamp = std::chrono::high_resolution_clock::now();

		inline void Record()
		{
			TimeStamp = std::chrono::high_resolution_clock::now();
		}

		// Elapsed time in seconds between creation or last recording and "timeStamp2"
		inline double ElapsedSecondsSince(std::chrono::high_resolution_clock::time_point timeStamp2)
		{
			std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(timeStamp2 - TimeStamp);
			return timeSpan.count();
		}

		// Elapsed time in seconds since creation or last recording
		inline double ElapsedSeconds()
		{
			return ElapsedSecondsSince(std::chrono::high_resolution_clock::now());
		}

		// Elapsed time in milliseconds since creation or last recording
		inline double ElapsedMS()
		{
			return ElapsedSeconds() * 1000.0;
		}

		// Record a reference timestamp and return elapsed time in seconds since the wi::Timer creation or last recording
		inline double RecordElapsedSeconds()
		{
			auto timeStamp2 = std::chrono::high_resolution_clock::now();
			auto elapsed = ElapsedSecondsSince(timeStamp2);
			TimeStamp = timeStamp2;
			return elapsed;
		}
	};
}

#endif // !_DETIMER_H_