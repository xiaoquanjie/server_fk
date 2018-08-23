#include "commonlib/transaction/transaction_mgr.h"
#include "slience/coroutine/coroutine.hpp"
#include "slience/base/logger.hpp"
#include "commonlib/transaction/base_transaction.h"

base::TimerPool TransactionMgr::_timer_pool(1);
base::s_uint32_t TransactionMgr::_trans_id_generator = 1;
m_unorder_map_t<base::s_uint32_t, Transaction*> TransactionMgr::_active_trans_map;

TransactionMgr::TransactionMgr() {

}

TransactionMgr::~TransactionMgr() {

}

void TransactionMgr::Init() {
	coroutine::Coroutine::initEnv(128 * 1024, true);
}

void TransactionMgr::Update(const base::timestamp& now) {
	_timer_pool.Update(now);
}

void TransactionMgr::CoroutineEnter(void* p) {
	Transaction* trans = (Transaction*)p;
	trans->set_co_id(coroutine::Coroutine::curid());
	_active_trans_map.insert(std::make_pair(trans->trans_id(), trans));
	
	LogInfo(
		"run a new transaction"
		<< "co_id£º"
		<< trans->co_id()
		<< " trans_id: "
		<< trans->trans_id()
	);
	trans->InCoroutine();
	LogInfo(
		"end a transaction"
		<< "co_id£º"
		<< trans->co_id()
		<< " trans_id: "
		<< trans->trans_id()
	);

	_active_trans_map.erase(trans->trans_id());
	RecyleTransaction(trans);
}

void TransactionMgr::TimerCallback(base::s_uint32_t trans_id) {
	auto iter = _active_trans_map.find(trans_id);
	if (iter != _active_trans_map.end()) {
		iter->second->_state = Transaction::E_STATE_TIMEOUT;
		iter->second->OnState();
	}
}

int TransactionMgr::CancelTimer(base::s_uint64_t id) {
	return _timer_pool.CancelTimer(id);
}

base::s_uint64_t TransactionMgr::AddTimer(int interval, base::s_uint32_t trans_id) {
	m_function_t<void()> cb = m_bind_t(&TimerCallback, trans_id);
	return _timer_pool.AddTimer(interval, cb);
}

base::s_uint32_t TransactionMgr::GeneratorTransId() {
	return _trans_id_generator++;
}

Transaction* TransactionMgr::CreateTransaction(base::s_uint64_t uid, base::s_uint32_t cmd) {
	return 0;
}

void TransactionMgr::RecyleTransaction(Transaction* trans) {

}