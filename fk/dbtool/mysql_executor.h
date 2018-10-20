#pragma once
#include "dbtool/dbtool.pb.h"
#include "mysqlclient/ma_wrapper.h"

class MysqlExecutor {
public:
	int Execute(const dbtool::MysqlSchemaConf& cfg);

};
