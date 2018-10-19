#include "dbtool/mysql_executor.h"
#include "dbtool/dbtool.pb.h"
#include "slience/base/logger.hpp"
#include "slience/base/compatibility.hpp"
#include "commonlib/svr_base/server_cfg.h"

int main(_In_ int argc, _In_reads_(argc) _Pre_z_ char** argv, _In_z_ char** envp) {
	do {
		SetLogFileName("dbtool", false);
		if (argc < 2) {
			LogInfo("Usage: dbtool <file1> <file2> ....");
			break;
		}

		bool flag = true;
		typedef ServerCfg<dbtool::MysqlSchemaConf> SchemaCfg;
		std::vector<SchemaCfg> SchemaCfgVec;
		for (int idx = 1; idx < argc; ++idx) {
			SchemaCfg cfg;
			int ret = cfg.Parse(argv[idx]);
			if (ret != 0) {
				LogError("parse file: " << argv[1] << " fail");
				flag = false;
				break;
			}
			else {
				SchemaCfgVec.push_back(cfg);
			}
		}

		if (!flag) {
			break;
		}

		// ¼ì²éÓï·¨


	} while (false);

	Sleep(1000);
	StopLogger();
	return 0;
}