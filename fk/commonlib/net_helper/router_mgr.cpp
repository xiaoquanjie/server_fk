#include "commonlib/net_helper/router_mgr.h"
#include "slience/base/logger.hpp"
#include "commonlib/net_helper/svr_net_handler.h"
#include <map>

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
	ServerCfg<config::RouterConfig> tmp_router_config;
	if (tmp_router_config.Parse(_router_file.c_str())) {

	}

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
	// 设计原则是，多连少关
	std::map<int, RouterInfo> tmp_router_info_map;
	std::vector<RouterInfo> tmp_router_info;
	for (int idx = 0; idx < _router_config.Data().router_list_size(); ++idx) {
		auto& item = _router_config.Data().router_list(idx);
		if (!ExistRouter(item.listen_ip(), item.listen_port(), item.number())) {
			RouterInfo* pinfo = (RouterInfo*)malloc(sizeof(RouterInfo));
			pinfo->ip = item.listen_ip();
			pinfo->port = item.listen_port();
			pinfo->number = item.number();
			SvrNetIoHandlerSgl.ConnectOne(pinfo->ip, pinfo->port, pinfo);
			tmp_router_info_map[pinfo->number] = *pinfo;
		}
	}

	// 已被关闭了的
	for (auto iter = _router_info.begin(); iter != _router_info.end();) {

	}

	return 0;
}

bool RouterMgr::ExistRouter(const std::string& ip, unsigned int port, int number) {
	for (auto iter = _router_info.begin(); iter != _router_info.end(); ++iter) {
		if (ip == iter->ip
			&& port == iter->port
			&& number == iter->number) {
			return true;
		}
	}
	return false;
}