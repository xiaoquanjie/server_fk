#include "slience/base/util.h"
#include "slience/base/string_util.hpp"
#include "slience/base/timer.hpp"

M_BASE_NAMESPACE_BEGIN

time_t HmsToSecond(const std::string& hms) {
	std::vector<int> array;
	StringUtil::Split(hms, ":", array);
	if (array.size() == 3) {
		time_t hms = array[0] * 3600;
		hms += array[1] * 60;
		hms += array[2];
		return hms;
	}
	else {
		return 0;
	}
}

time_t CertainTimesOfDay(time_t now, const std::string& hms) {
	time_t second = HmsToSecond(hms);
	struct tm tm;
	c_time::gmtime(&now, &tm);  // localtime(&now);
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	time_t next_time = mktime(&tm);
	next_time += second;
	return next_time;
}

time_t ClosestTimeOfDay(time_t now, time_t peroid) {
	time_t next_time = CertainTimesOfDay(now, "00:00:00");
	while (now >= next_time) {
		next_time += peroid;
	}
	return next_time;
}

M_BASE_NAMESPACE_END
