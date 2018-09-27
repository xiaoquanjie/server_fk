#include "slience/coroutine/coroutine.h"
M_COROUTINE_NAMESPACE_BEGIN
#ifdef M_PLATFORM_WIN

template<int N>
void __stdcall pub_coroutine(LPVOID p) {
	_schedule_& schedule = gschedule;
	if (schedule._curco) {
		(schedule._curco->_function)(schedule._curco->_data);
		schedule._curco->_status = COROUTINE_DEAD;
		::SwitchToFiber(schedule._ctx);
	}
}



#endif
M_COROUTINE_NAMESPACE_END
