#ifndef M_SERVER_INSTANCE_MGR_INCLUDE
#define M_SERVER_INSTANCE_MGR_INCLUDE

#include "slience/base/singletion.hpp"
#include "protolib/src/routersvr_config.pb.h"
#include <set>
#include <unordered_map>

struct InstanceInfo {
	int policy;
	std::set<int> inst_vec;
};

class SeverInstanceMgr {
public:
	typedef m_unorder_map_t<unsigned int, InstanceInfo> SvrType_InstInfo_Map;

	SeverInstanceMgr();

	int Init(const config::RouterSvrConfig* conf);

	void AddInstance(unsigned int server_type, int instance_id);

	void DelInstance(unsigned int server_type, int instance_id);

	void RouterPolicy(base::s_uint64_t uid, unsigned int server_type, bool is_broadcast, std::vector<int>& inst_vec);

protected:
	const config::Policy* GetPolicy(unsigned int server_type);

private:
	const config::RouterSvrConfig* _config;
	SvrType_InstInfo_Map _type_id_map;
};

#ifndef SeverInstanceMgrSgl
#define SeverInstanceMgrSgl base::singletion<SeverInstanceMgr>::mutable_instance()
#endif


#endif