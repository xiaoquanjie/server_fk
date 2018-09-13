#include "connsvr/conn_svr.h"

int ConnApplication::OnInitNetWork() {
	return 0;
}

void ConnApplication::OnStopNetWork() {
	
}

int ConnApplication::UpdateNetWork() {
	return 0;
}

int ConnApplication::OnInit() {
	return 0;
}

int ConnApplication::OnReload() {
	return 0;
}

int ConnApplication::OnExit() {
	return 0;
}

int ConnApplication::OnTick(const base::timestamp& now) {
	return 0;
}

int ConnApplication::OnProc(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len) {
	return 0;
}