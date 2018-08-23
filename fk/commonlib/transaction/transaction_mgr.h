#ifndef M_COMMONLIB_TRANSACTION_MGR_H
#define M_COMMONLIB_TRANSACTION_MGR_H

#include "slience/coroutine/coroutine.hpp"
#include "slience/base/time_pool.h"

class Transaction;
struct AppHeadFrame;

class TransactionBucket {
public:
	TransactionBucket(base::s_uint32_t cmd);

	virtual ~TransactionBucket();

	virtual Transaction* CreateTransaction() = 0;

	virtual void RecycleTransaction(Transaction*) = 0;

	int Size() {
		return _trans_vec.size();
	}

protected:
	base::s_uint32_t _cmd;
	std::vector<Transaction*> _trans_vec;
};

template<typename TransactionType>
class TransactionBucketImpl : public TransactionBucket {
public:
	TransactionBucketImpl(base::s_uint32_t cmd) : TransactionBucket(cmd) {
		for (int idx = 0; idx < 10; ++idx) {
			RecycleTransaction(new TransactionType(cmd));
		}
	}

	Transaction* CreateTransaction() override {
		if (_trans_vec.empty()) {
			Transaction* p = new TransactionType(_cmd);
			return p;
		}
		else {
			Transaction* p = _trans_vec.back();
			p->Construct();
			_trans_vec.pop_back();
			return p;
		}
	}

	void RecycleTransaction(Transaction* p) override {
		_trans_vec.push_back(p);
	}
};

class TransactionMgr {
	friend class Transaction;

public:
	TransactionMgr();

	~TransactionMgr();

	static void Init();

	static void Init(base::s_int32_t max_concurrent_trans);

	static void Update(const base::timestamp& now);

	static int ProcessFrame(base::s_int64_t fd, const AppHeadFrame& frame, const char* data);

	static void CoroutineEnter(void* p);

	static void TimerCallback(base::s_uint32_t trans_id);

	static int CancelTimer(base::s_uint64_t id);

	static base::s_uint64_t AddTimer(int interval, base::s_uint32_t trans_id);

	static base::s_uint32_t GeneratorTransId();

	static Transaction* CreateTransaction(base::s_uint64_t uid, base::s_uint32_t cmd);

	static Transaction* GetTransaction(base::s_uint32_t trans_id);

	static base::s_int32_t GetActiveTransCnt();

	template<int CMD, typename TransactionType>
	static int RegisterTransaction();

protected:
	static void RecyleTransaction(Transaction*);

	static void PrintStatus();

private:
	static base::s_int32_t _max_concurrent_trans;
	static base::TimerPool _timer_pool;
	static base::s_uint32_t _trans_id_generator;
	static base::timestamp _last_check_time;
	static m_unorder_map_t<base::s_uint32_t, Transaction*> _active_trans_map;
	static m_unorder_map_t<base::s_uint32_t, TransactionBucket*> _trans_bucket_map;
};

template<int CMD, typename TransactionType>
int TransactionMgr::RegisterTransaction() {
	LogInfo("register transaction info, cmd:" << CMD << " type:" << typeid(TransactionType).name());
	auto iter = _trans_bucket_map.find(CMD);
	if (iter == _trans_bucket_map.end()) {
		TransactionBucket* bucket = new TransactionBucketImpl<TransactionType>(CMD);
		_trans_bucket_map[CMD] = bucket;
	}
}

// ÊÂÎñ×¢²á
#define REGISTER_TRANSACTION(CMD, TRANS_TYPE) \
enum CMD##_##TRANS_TYPE {};\
	TransactionMgr::RegisterTransaction<CMD, TRANS_TYPE>();

#endif