#include "routersvr/router_svr.h"
#include "slience/base/logger.hpp"
#include "slience/base/time_pool.h"

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

static base::TimerPool time_pool;

int RouterApplication::OnTick(const base::timestamp& now) {
	time_pool.Update(now);
	
	return 0;
}

void Print_test() {
	LogInfo("this is a test");
	time_pool.AddTimer(560, &Print_test);
}

int RouterApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	base::s_uint64_t id = time_pool.AddTimer(560, &Print_test);
	time_pool.CancelTimer(id);
	return 0;
}
