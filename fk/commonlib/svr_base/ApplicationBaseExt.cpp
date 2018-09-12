#include "commonlib/svr_base/ApplicationBase.h"
#include "protolib/src/svr_base.pb.h"
#include "slience/base/logger.hpp"
#include "commonlib/svr_base/ApplicationFunc.hpp"
#include "commonlib/transaction/transaction_mgr.h"

int ApplicationBase::Run() {
	while (!gAppExist) {
		base::timestamp t_now;
		if (t_now.millisecond() > _now.millisecond()) {
			_now = t_now;
			TransactionMgr::Update(_now);
			OnTick(_now);
		}
		if (CheckReload()) {
			LogInfo("reload begin.................");
			OnReload();
			LogInfo("reload end....................");
		}
		if (0 != UpdateNetWork()) {
			Sleep(1);
		}
	}
	OnExit();
	LogInfo("application exit..................");
	return 0;
}
