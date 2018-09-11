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

void SeverInstanceMgr::AddInstance(unsigned int server_type, int instance_id) {
	const config::Policy* policy = GetPolicy(server_type);
	if (!policy) {
		LogError("add instance fail, server_type:" << server_type << "instance_id" << instance_id);
		return;
	}

	auto iter_type = _type_id_map.find(server_type);
	if (iter_type == _type_id_map.end()) {
		_type_id_map[server_type] = InstanceInfo();
		iter_type = _type_id_map.find(server_type);
	}
	
	iter_type->second.policy = policy->policy();
	iter_type->second.inst_vec.insert(instance_id);
	LogInfo("add server instance: server_type=" << server_type << " instance_id=" << instance_id);
}

void SeverInstanceMgr::DelInstance(unsigned int server_type, int instance_id) {
	auto iter_type = _type_id_map.find(server_type);
	if (iter_type == _type_id_map.end()) {
		LogError("can't find server instance: server_type=" << server_type << " instance_id=" << instance_id);
	}
	else {
		iter_type->second.inst_vec.erase(instance_id);
		LogInfo("del server instance: server_type=" << server_type << " instance_id=" << instance_id);
	}
}

void SeverInstanceMgr::RouterPolicy(
	base::s_uint64_t uid, unsigned int server_type, bool is_broadcast, std::vector<int>& inst_vec) {
	auto iter_type = _type_id_map.find(server_type);
	if (iter_type == _type_id_map.end()) {
		LogError("can't find instance, server_type:" << server_type);
		return;
	}

	std::vector<int> all_inst;
	for (auto iter_id = iter_type->second.inst_vec.begin(); iter_id != iter_type->second.inst_vec.end();
		++iter_id) {
		all_inst.push_back(*iter_id);
	}
	if (all_inst.empty()) {
		LogError("instance vector is empty, server_type:" << server_type);
		return;
	}

	if (is_broadcast) {
		all_inst.swap(inst_vec);
		return;
	}

	switch (iter_type->second.policy) {
	case config::POLICY_ROUTER:
		break;
	case config::POLICY_RANDOM:{
		int t = all_inst[0];
		inst_vec.push_back(t);
		break;
	}
	case config::POLICY_MOD: {
		int u = uid % all_inst.size();
		inst_vec.push_back(all_inst[u]);
		break;
	}
	case config::POLICY_BROADCAST: {
		all_inst.swap(inst_vec);
		break;
	}
	}
}

const config::Policy* SeverInstanceMgr::GetPolicy(unsigned int server_type) {
	if (!_config) {
		LogError("_config is null");
		return 0;
	}

	for (int idx = 0; idx < _config->policy_list_size(); ++idx) {
		if (_config->policy_list(idx).svr_type() == server_type) {
			return &_config->policy_list(idx);
		}
	}
	LogError("can't find server_type:" << server_type);
	return 0;
}