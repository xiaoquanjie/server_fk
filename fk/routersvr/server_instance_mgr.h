#ifndef M_SERVER_INSTANCE_MGR_INCLUDE
#define M_SERVER_INSTANCE_MGR_INCLUDE

#include "slience/base/singletion.hpp"
#include "protolib/src/routersvr_config.pb.h"
#include <set>
#include <unordered_map>

class SeverInstanceMgr {
public:
	typedef std::set<int> InstVector;
	typedef m_unorder_map_t<int, InstVector> SvrType_InstVec_Map;

	SeverInstanceMgr();

	int Init(const config::RouterSvrConfig* conf);

	void AddInstance(int server_type, int instance_id);

	void DelInstance(int server_type, int instance_id);

	void RouterPolicy(base::s_uint64_t uid, int server_type, bool is_broadcast, std::vector<int>& inst_vec);

private:
	const config::RouterSvrConfig* _config;
	SvrType_InstVec_Map _type_id_map;
};

#ifndef SeverInstanceMgrSgl
#define SeverInstanceMgrSgl base::singletion<SeverInstanceMgr>::mutable_instance()
#endif


#endif