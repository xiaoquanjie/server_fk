#include "routersvr/server_instance_mgr.h"
#include "slience/base/logger.hpp"

SeverInstanceMgr::SeverInstanceMgr() {
	_config = 0;
}

int SeverInstanceMgr::Init(const config::RouterSvrConfig* conf) {
	if (!conf) {
		LogError("conf is null");
		return -1;
	}
	_config = conf;
	return 0;
}

void SeverInstanceMgr::AddInstance(int server_type, int instance_id) {
	auto iter_type = _type_id_map.find(server_type);
	if (iter_type == _type_id_map.end()) {
		_type_id_map[server_type] = InstVector();
		iter_type = _type_id_map.find(server_type);
	}
	
	iter_type->second.insert(instance_id);
	LogInfo("add server instance: server_type=" << server_type << " instance_id=" << instance_id);
}

void SeverInstanceMgr::DelInstance(int server_type, int instance_id) {
	auto iter_type = _type_id_map.find(server_type);
	if (iter_type == _type_id_map.end()) {
		LogError("can't find server instance: server_type=" << server_type << " instance_id=" << instance_id);
	}
	else {
		iter_type->second.erase(instance_id);
		LogInfo("del server instance: server_type=" << server_type << " instance_id=" << instance_id);
	}
}

void SeverInstanceMgr::RouterPolicy(
	base::s_uint64_t uid, int server_type, bool is_broadcast, std::vector<int>& inst_vec) {

}
