#pragma once

#include "commonlib/svr_base/ApplicationBase.h"
#include "slience/base/singletion.hpp"
#include "commonlib/svr_base/server_cfg.h"
#include "protolib/src/server_agent_conf.pb.h"

class AgentApplication : public ApplicationBase {
public:
	int ServerType() override;

	int InstanceId() override;

	int OnInitNetWork() override;

	void OnStopNetWork() override;

	int UpdateNetWork() override;

	int OnInit() override;

	int OnReload() override;

	int OnExit() override;

	int OnTick(const base::timestamp& now) override;

	int OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) override;

private:
	ServerCfg<config::ServerAgentConfig> _svr_config;
};

#ifndef AgentApplicationSgl
#define AgentApplicationSgl base::singleton<AgentApplication>::mutable_instance()
#endif