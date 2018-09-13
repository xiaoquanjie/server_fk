#include "connsvr/conn_svr.h"
#include "commonlib/net_helper/svr_net_handler.h"
#include "commonlib/transaction/transaction_mgr.h"

int ConnApplication::OnInitNetWork() {
	auto func = m_bind_t(&ConnApplication::OnProc, this, placeholder_1,
		placeholder_2, placeholder_3, placeholder_4);
	SvrNetIoHandlerSgl.Init(_now, func);
	SvrNetIoHandlerSgl.Start(_svr_thread_cnt, false);
	return 0;
}

void ConnApplication::OnStopNetWork() {
	SvrNetIoHandlerSgl.Stop();
}

int ConnApplication::UpdateNetWork() {
	SvrNetIoHandlerSgl.Update();
	return 0;
}

int ConnApplication::OnInit() {
	return 0;
}

int ConnApplication::OnReload() {
	const std::string path = ConfigFilePath();
	int ret = _svr_config.Parse(path.c_str());
	if (0 != ret) {
		LogError("_svr_config.Parse fail:" << path);
	}
	return ret;
}

int ConnApplication::OnExit() {
	OnStopNetWork();
	return 0;
}

int ConnApplication::OnTick(const base::timestamp& now) {
	SvrNetIoHandlerSgl.OnTick();
	return 0;
}

int ConnApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	TransactionMgr::ProcessFrame(fd, frame, data);
	return 0;
}