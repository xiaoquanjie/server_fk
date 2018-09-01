#include "routersvr/router_svr.h"
#include "slience/base/logger.hpp"
#include "commonlib/transaction/transaction_mgr.h"

int RouterApplication::OnInit() {
	if (_svr_config.Data().listen_list_size() <= 0) {
		LogError("listen_list is empty");
		return -1;
	}
	for (int idx = 0; idx < _svr_config.Data().listen_list_size(); ++idx) {
		auto item = _svr_config.Data().listen_list(idx);
		if (!ListenOne(item.listen_ip(), item.listen_port())) {
			LogError(item.listen_ip() << " " << item.listen_port() << "listen error:" << GetLastError().What());
			return -1;
		}
		else {
			LogInfo("listen in: " << item.ShortDebugString());
		}
	}
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
	TransactionMgr::ProcessFrame(fd, frame, data);
	return 0;
}
