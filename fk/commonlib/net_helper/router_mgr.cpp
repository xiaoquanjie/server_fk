#include "commonlib/net_helper/router_mgr.h"
#include "slience/base/logger.hpp"
#include "commonlib/net_helper/net_helper.h"

RouterMgr::RouterMgr() {

}

int RouterMgr::Init(const std::string& router_file) {
	_router_file = router_file;
	int ret = 0;
	do {
		ret = Reload();
		if (0 != ret) {
			LogError("reload fail");
			break;
		}
	} while (false);
	return ret;
}

int RouterMgr::Reload() {
	if (!_router_config.Parse(_router_file.c_str())) {
		LogError("_router_config.Parse fail: " << _router_file);
		return -1;
	}
	if (0 != ConnectRouters()) {
		LogError("ConnectRouters fail");
		return -1;
	}
	return 0;
}

int RouterMgr::ConnectRouters() {
	return 0;
}