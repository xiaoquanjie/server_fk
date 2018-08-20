#include "slience/base/time_pool.h"
#include <map>
#include "slience/base/object_pool.h"

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

int TimerPool::AddTimer(int interval, m_function_t<void()> func) {
	if (interval <= 0) {
		func();
		return 0;
	}
	if (interval > big_bucket_cnt * _max_interval_day * 1000) {
		return -1;
	}

	base::s_int64_t now = base::timestamp().millisecond();
	int inteval_mil = (int)(now - _beg_time + interval);
	int interval_sec = inteval_mil / 1000;
	int big_bucket = interval_sec % (big_bucket_cnt * _max_interval_day);
	int small_bucket = ((inteval_mil % 1000) / 100) % 10;
	void** pp = (void**)_big_bucket[big_bucket];
	TimeNodeMap* pmap = (TimeNodeMap*)(pp[small_bucket]);

	TimeNode* node = base::ObjectPool<TimeNode>::Alloc();
	node->expire = now + interval;
	node->cb = func;
	pmap->insert(std::make_pair(node->expire, node));
	return true;
}

M_BASE_NAMESPACE_END