#include "commonlib/transaction/base_transaction.h"
#include "slience/coroutine/coroutine.hpp"
#include "commonlib/transaction/transaction_mgr.h"
#include "slience/base/buffer.hpp"
#include "commonlib/net_handler/net_handler.h"

void Transaction::Construct() {
	_trans_id = 0;
	_userid = 0;
	_co_id = 0;
	_fd = 0;
	_cur_fd = 0;
	_cur_frame_head = 0;
	_cur_frame_data = 0;
	_self_svr_type = 0;
	_self_inst_id = 0;
	_state = E_STATE_IDLE;
	_timer_id = 0;
}

Transaction::Transaction(int cmd) {
	_cmd = cmd;
	Construct();
}

Transaction::~Transaction() {

}

int Transaction::ParseMsg(google::protobuf::Message& message) {
	if (!_cur_frame_data || !_cur_frame_head) {
		assert(0);
		LogError("userid: " << userid() << " cmd: " << cmd() << "_cur_frame_data or _cur_frame_head is nullptr");
		return -1;
	}
	bool ret = message.ParseFromArray(_cur_frame_data, _cur_frame_head->get_cmd_length());
	if (ret) {
		return 0;
	}
	else {
		LogError("userid: " << userid() << " cmd: " << cmd() << " parse message fail, len:" << _cur_frame_head->get_cmd_length());
		return -1;
	}
}

int Transaction::Process(base::s_int64_t fd, base::s_uint32_t self_svr_type,
	base::s_uint32_t self_inst_id,
	const AppHeadFrame& frame, const char* data) {
	if (!_cur_frame_data) {
		_userid = frame.get_userid();
		_cmd = frame.get_cmd();
		_fd = fd;
		_self_svr_type = self_svr_type;
		_self_inst_id = self_inst_id;
		_ori_frame_head = frame;
	}
	else {
		_state = E_STATE_ACTIVE;
	}
	_cur_frame_data = data;
	_cur_frame_head = &frame;
	_cur_fd = fd;
	OnState();
	return 0;
}

void Transaction::OnState() {
	CancelTimer();
	int ret = 0;
	switch (_state) {
	case E_STATE_IDLE:
		ret = OnIdle();
		break;
	case E_STATE_ACTIVE:
		ret = OnActive();
		break;
	case E_STATE_TIMEOUT:
		ret = OnTimeOut();
		break;
	}
}

void Transaction::CancelTimer() {
	if (0 != _timer_id) {
		if (0 != TransactionMgr::CancelTimer(_timer_id)) {
			LogError("cancel timer fail, id: " << _timer_id);
		}
		_timer_id = 0;
	}
}

void Transaction::SetTimer(int interval) {
	CancelTimer();
	base::s_uint64_t id = TransactionMgr::AddTimer(interval, trans_id());
	if (id == 0) {
		LogError("add timer fail, interval: " << interval);
		return;
	}
}

Transaction::Wait_Return Transaction::Wait(int interval) {
	if (0 != co_id()) {
		SetTimer(interval);
		coroutine::Coroutine::yield();
		int state = _state;
		_state = E_STATE_IDLE;

		if (state == E_STATE_ACTIVE) {
			return E_RETURN_ACTIVE;
		}
		else {
			return E_RETURN_TIMEOUT;
		}
	}
	else {
		// don't allow
		LogError("don't call the wait interface on main coroutine directly or indirectly");
		throw int(-1);
		return E_RETURN_ERROR;
	}
}

int Transaction::OnIdle() {
	// 分配trans_id
	_trans_id = TransactionMgr::GeneratorTransId();
	// 启动协程
	coroutine::CoroutineTask::doTask(&TransactionMgr::CoroutineEnter, (void*)this);
	return 0;
}

int Transaction::OnActive() {
	// 唤醒协程
	if (0 != co_id()) {
		coroutine::Coroutine::resume(co_id());
	}
	return 0;
}

int Transaction::OnTimeOut() {
	// 唤醒协程
	_timer_id = 0;
	if (0 != co_id()) {
		coroutine::Coroutine::resume(co_id());
	}
	return 0;
}

int Transaction::SendMsgByServerType(int cmd, int svr_type,
	google::protobuf::Message& request, google::protobuf::Message& respond) {
	return 0;
}

int Transaction::SendMsgByServerType(int cmd, int svr_type,
	google::protobuf::Message& request) {
	AppHeadFrame frame;
	frame.set_is_broadcast(false);
	frame.set_src_svr_type(self_svr_type());
	frame.set_dst_svr_type(svr_type);
	frame.set_src_inst_id(self_inst_id());
	frame.set_dst_inst_id(0);
	frame.set_src_trans_id(0);
	frame.set_dst_trans_id(0);
	frame.set_cmd(cmd);
	frame.set_userid(userid());
	std::string msg = request.SerializePartialAsString();
	frame.set_cmd_length(msg.length());
	RouterMgrSgl.SendMsg(frame, msg);
	return 0;
}

int Transaction::SendMsgByFd(int cmd, base::s_int64_t fd,
	google::protobuf::Message& request, google::protobuf::Message& respond) {
	return 0;
}

int Transaction::SendMsgByFd(int cmd, base::s_int64_t fd,
	google::protobuf::Message& request) {
	AppHeadFrame frame;
	frame.set_is_broadcast(false);
	frame.set_src_svr_type(ori_frame_head().get_dst_svr_type());
	frame.set_dst_svr_type(ori_frame_head().get_src_inst_id());
	frame.set_src_inst_id(ori_frame_head().get_dst_inst_id());
	frame.set_dst_inst_id(ori_frame_head().get_src_inst_id());
	//frame.set_dst_trans_id()
	frame.set_cmd(_cmd);
	frame.set_userid(userid());
	std::string data = request.SerializePartialAsString();
	frame.set_cmd_length(data.length());
	base::Buffer buffer;
	buffer.Write(frame);
	NetIoHandlerSgl.SendDataByFd(fd, buffer.Data(), buffer.Length());
	return 0;
}

base::s_uint32_t Transaction::trans_id() {
	return _trans_id;
}

base::s_uint32_t Transaction::cmd() {
	return _cmd;
}

base::s_uint64_t Transaction::userid() {
	return _userid;
}

base::s_int32_t Transaction::co_id() {
	return _co_id;
}

base::s_int64_t Transaction::fd() {
	return _fd;
}

base::s_int64_t Transaction::cur_fd() {
	return _cur_fd;
}

base::s_uint32_t Transaction::self_svr_type() {
	return _self_svr_type;
}

base::s_uint32_t Transaction::self_inst_id() {
	return _self_inst_id;
}

void Transaction::set_co_id(base::s_int32_t id) {
	_co_id = id;
}

const AppHeadFrame& Transaction::cur_frame_head() {
	return *_cur_frame_head;
}

const AppHeadFrame& Transaction::ori_frame_head() {
	return _ori_frame_head;
}