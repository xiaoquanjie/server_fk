#ifndef M_COROUTINE_COROUTINE_INCLUDE
#define M_COROUTINE_COROUTINE_INCLUDE

#include "slience/coroutine/config.hpp"
#include "slience/base/svector.hpp"
M_COROUTINE_NAMESPACE_BEGIN

#ifdef M_PLATFORM_WIN

struct _coroutine_ {
	int _id;
	int _status;
	LPVOID _ctx;
	void* _data;
	_coroutine_func_ _function;
};

struct _schedule_ {
	_schedule_() {
		_cap = 0;
		_nco = 0;
		_curco = 0;
		_ctx = 0;
		_co = 0;
	}
	int _cap;
	int _nco;
	LPVOID _ctx;
	_coroutine_** _co;
	_coroutine_* _curco;
	base::svector<int> _freeid;
};

#else

struct _coroutine_ {
	int	_id;
	int	_status;
	char*_stack;
	int	_size;
	int	_cap;
	ucontext_t_ctx;
	void*_data;
	_coroutine_func_ _function;
};

struct _schedule_ {
	_schedule_() {
		_cap = 0;
		_nco = 0;
		_curco = 0;
		_co = 0;
		_pri_stack = false;
	}
	int _cap;
	int _nco;
	bool _pri_stack;
	ucontext_t _ctx;
	_coroutine_** _co;
	_coroutine_* _curco;
	base::svector<int> _freeid;
	char _stack[M_COROUTINE_STACK_SIZE];
};

#endif

class Coroutine {
public:
	// init environment
	static bool init(unsigned int stack_size = 128 * 1204, bool pri_stack = false);

	// create one new coroutine
	static int create(_coroutine_func_ routine, void* data);

	// close
	static void close();

	// resume
	static void resume(int co_id);

	// yield
	static void yield();

	// current coroutine id
	static unsigned int curid();

	static void destroy(int co_id);
};

M_COROUTINE_NAMESPACE_END
#endif