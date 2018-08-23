#ifndef M_COMMONLIB_TRANSACTION_MGR_H
#define M_COMMONLIB_TRANSACTION_MGR_H

#include "slience/coroutine/coroutine.hpp"
#include "slience/base/time_pool.h"

class Transaction;

class TransactionMgr {
	friend class Transaction;

public:
	TransactionMgr();

	~TransactionMgr();

	static void Init();

	static void Update(const base::timestamp& now);

	static void CoroutineEnter(void* p);

	static void TimerCallback(base::s_uint32_t trans_id);

	static int CancelTimer(base::s_uint64_t id);

	static base::s_uint64_t AddTimer(int interval, base::s_uint32_t trans_id);

	static base::s_uint32_t GeneratorTransId();

	static Transaction* CreateTransaction(base::s_uint64_t uid, base::s_uint32_t cmd);

protected:
	static void RecyleTransaction(Transaction*);

private:
	static base::TimerPool _timer_pool;
	static base::s_uint32_t _trans_id_generator;
	static m_unorder_map_t<base::s_uint32_t, Transaction*> _active_trans_map;
};

#endif