#ifndef M_CONNSVR_INCLUDE
#define M_CONNSVR_INCLUDE

#include "commonlib/svr_base/ApplicationBase.h"
#include "slience/base/singletion.hpp"
#include "commonlib/svr_base/server_cfg.h"

class ConnApplication : public ApplicationBase {
public:
	int OnInitNetWork() override;

	void OnStopNetWork() override;

	int UpdateNetWork() override;

	int OnInit() override;

	int OnReload() override;

	int OnExit() override;

	int OnTick(const base::timestamp& now) override;

	int OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) override;

};

#ifndef ConnApplicationSgl
#define ConnApplicationSgl base::singleton<ConnApplication>::mutable_instance()
#endif

#endif