#include "slience/base/time_pool.h"
#include <map>

M_BASE_NAMESPACE_BEGIN

// 大桶的精度是1秒
static const int big_bucket_cnt = 24 * 3600;
// 不桶的精度是1/10秒
const int small_bucket_cnt = 10;

typedef std::map<base::s_int64_t, TimerPool::TimeNode*> TimeNodeMap;

TimerPool::TimerPool(int max_interval_day) {
	if (max_interval_day > 7 || max_interval_day < 0) {
		max_interval_day = 1;
	}
	_max_interval_day = max_interval_day;

	// 初始化大桶, 精度是1秒
	_big_bucket = (void**)malloc(sizeof(void*) * big_bucket_cnt * _max_interval_day);
	for (int idx = 0; idx < big_bucket_cnt; ++idx) {
		// 初始化小桶
		void** small_bucket_pointer = (void**)malloc(sizeof(void*) * small_bucket_cnt);
		for (int idx2 = 0; idx2 < small_bucket_cnt; ++idx2) {
			small_bucket_pointer[idx2] = (void*)(new TimeNodeMap);
		}

		_big_bucket[idx] = (TimeNode*)small_bucket_pointer;
	}

	_beg_time = base::timestamp().millisecond();
	_big_bucket_idx = 0;
	_small_bucket_idx = 0;
}

TimerPool::~TimerPool() {
	for (int idx = 0; idx < big_bucket_cnt * _max_interval_day; ++idx) {
		for (int idx2 = 0; idx2 < small_bucket_cnt; ++idx2) {
			void** pp = (void**)_big_bucket[idx];
			TimeNodeMap* p = (TimeNodeMap*)pp[idx2];
			delete (p);
		}
		free(_big_bucket[idx]);
	}
	free(_big_bucket);
}

void TimerPool::Update(const base::timestamp& now) {

}

bool TimerPool::AddTimer(int interval, m_function_t<void(int param1, void* param2)> func) {
	return false;
}

M_BASE_NAMESPACE_END