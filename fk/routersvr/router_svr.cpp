#include "routersvr/router_svr.h"
#include "hiredis_wrapper/wrapper/redis_wrapper.hpp"
#include "slience/base/logger.hpp"

int RouterApplication::OnInit() {
	if (ListenOne("0.0.0.0", 22002)) {
		LogInfo("listen success");
	}
	else {
		LogError("listen error:" << GetLastError().What());
		return -1;
	}
	return 0;
}

int RouterApplication::OnReload() {
	return 0;
}

int RouterApplication::OnProc(int fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	return 0;
}
