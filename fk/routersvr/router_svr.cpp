#include "routersvr/router_svr.h"
#include "slience/base/logger.hpp"
#include "commonlib/transaction/transaction_mgr.h"
#include "protolib/src/cmd.pb.h"
#include "routersvr\server_instance_mgr.h"
#include "commonlib/net_helper/net_helper.h"

int RouterApplication::OnInit() {
	if (_svr_config.Data().listen_list_size() <= 0) {
		LogError("listen_list is empty");
		return -1;
	}
	for (int idx = 0; idx < _svr_config.Data().listen_list_size(); ++idx) {
		auto item = _svr_config.Data().listen_list(idx);
		if (!NetHelper::ListenOne(item.listen_ip(), item.listen_port())) {
			LogError(item.listen_ip() << " " << item.listen_port() << "listen error:" << NetHelper::GetLastError().What());
			return -1;
		}
		else {
			LogInfo("listen in: " << item.ShortDebugString());
		}
	}

	SeverInstanceMgrSgl.Init(&_svr_config.Data());
	LogInfo("server listen success");
	return 0;
}

int RouterApplication::OnReload() {
	const std::string config_path = ConfigFilePath();
	if (_svr_config.Parse(config_path.c_str()) != 0) {
		LogError("_svr_config.Parse fail");
		return -1;
	}
	return 0;
}

int RouterApplication::OnTick(const base::timestamp& now) {
	return 0;
}

int RouterApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	switch (frame.get_cmd()) {
	case proto::CMD::CMD_SOCKET_CLIENT_OUT:
	case proto::CMD::CMD_SOCKET_CLIENT_IN:
	case proto::CMD::CMD_REGISTER_SERVER_REQ:
		TransactionMgr::ProcessFrame(fd, frame, data);
		return 0;
	default:
		// ×ª·¢
		break;
	}
	if (frame.get_dst_inst_id() != 0) {
		ForwardPkg(fd, frame.get_dst_inst_id(), frame, data, data_len);
	}
	else {
		std::vector<int> inst_vec;
		SeverInstanceMgrSgl.RouterPolicy(frame.get_userid(), frame.get_dst_svr_type(),
			frame.get_is_broadcast(), inst_vec);
		for (auto iter = inst_vec.begin(); iter != inst_vec.end(); ++iter) {
			ForwardPkg(fd, *iter, frame, data, data_len);
		}
	}
	return 0;
}

int RouterApplication::ForwardPkg(base::s_int64_t fd, int dst_inst_id, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	const char* src = data - sizeof(frame);
	int src_len = data_len + sizeof(frame);
	NetHelper::SendDataByInstId(dst_inst_id, src, src_len);
	return 0;
}