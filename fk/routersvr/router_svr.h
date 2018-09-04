#ifndef M_ROUTERSVR_ROUTER_SVR_INCLUDE
#define M_ROUTERSVR_ROUTER_SVR_INCLUDE

#include "commonlib/svr_base/ApplicationBase.h"
#include "slience/base/singletion.hpp"
#include "protolib/src/routersvr_config.pb.h"
#include "commonlib/svr_base/server_cfg.h"

class RouterApplication : public ApplicationBase {
protected:
	int OnInit() override;

	int OnReload() override;

	int OnTick(const base::timestamp& now) override;

	int OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) override;

	int ForwardPkg(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len);

private:
	ServerCfg<config::RouterSvrConfig> _svr_config;
};

typedef base::singletion<RouterApplication> RouterAppSgl;

#endif