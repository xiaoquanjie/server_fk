#include "server_agent/server_agent.h"
#include "commonlib/net_handler/net_handler.h"
#include <climits>
#include "slience/base/util.h"

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
	ExecuteScript();
	return 0;
}

int AgentApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	// TransactionMgr::ProcessFrame(fd, ServerType(), InstanceId(), frame, data);
	return 0;
}

void AgentApplication::ExecuteScript() {
	const base::timestamp& now = GetNow();
	for (int idx = 0; idx < _svr_config.Data().script_info_list_size(); ++idx) {
		auto& item = *(_svr_config.Data().mutable_script_info_list(idx));
		time_t next_time = 0;
		if (item.type() == config::E_SCRIPT_TYPE_CERTAIN_TIME) {
			next_time = base::CertainTimesOfDay(now.second(), item.day_certain_time());
		}
		else if (item.type() == config::E_SCRIPT_TYPE_PERIOD_TIME) {
			next_time = base::ClosestTimeOfDay(now.second(), item.interval_seconds());
		}

		if (!item.has_next_check_time()) {
			item.set_next_check_time((unsigned int)next_time);
			LogInfo(item.script() << " script next execute time is: " << next_time);
		}

		if (now.second() < item.next_check_time()) {
			continue;
		}

#ifndef M_PLATFORM_WIN
		char cwd[PATH_MAX];
		getcwd(cwd, sizeof(cwd));
		chdir(item.script_dir().c_str());
		System(item.script());
		chdir(cwd);
#endif
		item.set_next_check_time((unsigned int)next_time);
		LogInfo(item.script() << " script next execute time is: " << next_time);
	}
}

int AgentApplication::System(const std::string& cmd) {
#ifndef M_PLATFORM_WIN
	signal(SIGCHLD, SIG_DFL);
	int ret = system(cmd.c_str());
	signal(SIGCHLD, SIG_IGN);
	if (ret < 0) {
		LogError("fail to call system " << cmd << ", return: " << ret << ", errno: " << errno);
	}
	return ret;
#else
	return 0;
#endif
}