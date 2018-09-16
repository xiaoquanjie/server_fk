#include "commonlib/net_handler/router_mgr.h"
#include "slience/base/logger.hpp"
#include "commonlib/net_handler/svr_net_handler.h"
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
	if (!tmp_router_config.Parse(_router_file.c_str())) {
		LogError("_router_config.Parse fail: " << _router_file);
		return -1;
	}

	std::set<int> number_set;
	for (int idx = 0; idx < tmp_router_config.Data().router_list_size(); ++idx) {
		int number = tmp_router_config.Data().router_list(idx).number();
		if (number_set.count(number) > 0) {
			LogError("router number is duplicated: " << number);
			return -1;
		}
	}

	_router_config.Data().Swap(&tmp_router_config.Data());
	if (0 != ConnectRouters()) {
		LogError("ConnectRouters fail");
		return -1;
	}
	return 0;
}

int RouterMgr::ConnectRouters() {
	// 设计原则是，多连少关
	std::map<int, RouterInfo> tmp_router_info_map;
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
	for (auto iter = _router_info_vec.begin(); iter != _router_info_vec.end();) {
		bool exist = false;
		for (int idx = 0; idx < _router_config.Data().router_list_size(); ++idx) {
			auto& item = _router_config.Data().router_list(idx);
			if (iter->ip == item.listen_ip()
				&& iter->port == item.listen_port()
				&& iter->number == item.number()) {
				exist = true;
				break;
			}
		}
		if (!exist) {
			// close
			if (iter->ptr) {
				iter->ptr->Close();
			}
			iter = _router_info_vec.erase(iter);
		}
		else {
			tmp_router_info_map[iter->number] = *iter;
			iter++;
		}
	}

	_router_info_vec.clear();
	for (auto iter = tmp_router_info_map.begin(); iter != tmp_router_info_map.end();
		++iter) {
		_router_info_vec.push_back(iter->second);
	}
	return 0;
}

bool RouterMgr::ExistRouter(const std::string& ip, unsigned int port, int number) {
	for (auto iter = _router_info_vec.begin(); iter != _router_info_vec.end(); ++iter) {
		if (ip == iter->ip
			&& port == iter->port
			&& number == iter->number) {
			return true;
		}
	}
	return false;
}

int RouterMgr::AddRouter(const std::string& ip, unsigned int port, int number,
	netiolib::TcpConnectorPtr ptr) {
	for (auto iter = _router_info_vec.begin(); iter != _router_info_vec.end();
		++iter) {
		if (ip == iter->ip
			&& port == iter->port
			&& number == iter->number) {
			iter->ptr = ptr;
			return 0;
		}
	}
	return -1;
}

int RouterMgr::DelRouter(const std::string& ip, unsigned int port, int number,
	netiolib::TcpConnectorPtr ptr) {
	for (auto iter = _router_info_vec.begin(); iter != _router_info_vec.end();
		++iter) {
		if (ip == iter->ip
			&& port == iter->port
			&& number == iter->number) {
			_router_info_vec.erase(iter);
			return 0;
		}
	}
	return -1;
	return 0;
}