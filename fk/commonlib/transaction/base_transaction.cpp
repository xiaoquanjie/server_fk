#include "commonlib/transaction/base_transaction.h"

Transaction::Transaction(int cmd) {
	_cmd = cmd;
	_trans_id = 0;
	_userid = 0;
	_co_id = 0;
	_fd = 0;
	_cur_frame_head = 0;
	_cur_frame_data = 0;
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

int Transaction::Process(base::s_int64_t fd, const AppHeadFrame& frame, const char* data) {
	if (!_cur_frame_data) {
		_userid = frame.get_userid();
		_cmd = frame.get_cmd();
		_fd = fd;
		_ori_frame_head = frame;
	}
	_cur_frame_data = data;
	_cur_frame_head = &frame;
	return 0;
}

void Transaction::SendMessageBack(google::protobuf::Message& message) {

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

base::s_uint32_t Transaction::co_id() {
	return _co_id;
}

const AppHeadFrame& Transaction::cur_frame_head() {
	return *_cur_frame_head;
}

const AppHeadFrame& Transaction::ori_frame_head() {
	return _ori_frame_head;
}