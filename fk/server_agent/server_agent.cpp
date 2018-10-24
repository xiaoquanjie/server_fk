#include "server_agent/server_agent.h"
#include "commonlib/net_handler/net_handler.h"


int AgentApplication::ServerType() {
	return -1;
}

int AgentApplication::InstanceId() {
	return -1;
}

int AgentApplication::OnInitNetWork() {
	auto func = m_bind_t(&AgentApplication::OnProc, this, placeholder_1,
		placeholder_2, placeholder_3, placeholder_4);
	NetIoHandlerSgl.Init(_now, func);
	NetIoHandlerSgl.Start(_svr_thread_cnt, false);
	return 0;
}

void AgentApplication::OnStopNetWork() {
	NetIoHandlerSgl.Stop();
}

int AgentApplication::UpdateNetWork() {
	NetIoHandlerSgl.Update();
	return 0;
}

int AgentApplication::OnInit() {
	// listen
	std::string ip = _svr_config.Data().listen_info().listen_ip();
	int port = _svr_config.Data().listen_info().listen_port();
	if (!NetIoHandlerSgl.ListenOne(ip, port)) {
		LogError(ip << " " << port << "listen error:" << NetIoHandlerSgl.GetLastError().What());
		return -1;
	}
	else {
		LogInfo("listen in: " << ip << " " << port);
	}

	return 0;
}

int AgentApplication::OnReload() {
	const std::string path = ConfigFilePath();
	if (0 != _svr_config.Parse(path.c_str())) {
		LogError("_svr_config.Parse fail:" << path);
		return -1;
	}

	return 0;
}

int AgentApplication::OnExit() {
	OnStopNetWork();
	return 0;
}

int AgentApplication::OnTick(const base::timestamp& now) {
	NetIoHandlerSgl.OnTick();
	return 0;
}

int AgentApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	// TransactionMgr::ProcessFrame(fd, ServerType(), InstanceId(), frame, data);
	return 0;
}