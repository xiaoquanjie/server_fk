#pragma once
#include "slience/base/singletion.hpp"
#include "protolib/src/router.pb.h"
#include "commonlib/svr_base/server_cfg.h"
#include "slience/netio/netio.hpp"
#include <vector>

struct RouterInfo {
	std::string ip;
	unsigned short port;
	int number;
	base::s_int64_t fd;
};

class RouterMgr {
public:
	RouterMgr();

	int Init(const std::string& router_file);

	int Reload();

	bool ExistRouter(const std::string& ip, unsigned int port, int number);

	int AddRouter(const std::string& ip, unsigned int port, int number,
		base::s_int64_t fd);

	int DelRouter(const std::string& ip, unsigned int port, int number,
		base::s_int64_t fd);

protected:
	int ConnectRouters();

private:
	std::string _router_file;
	ServerCfg<config::RouterConfig> _router_config;
	std::vector<RouterInfo> _router_info_vec;
};

#ifndef RouterMgrSgl
#define RouterMgrSgl base::singleton<RouterMgr>::mutable_instance()
#endif
