#include "dbtool/mysql_executor.h"
#include "slience/base/logger.hpp"
#include "mariadb-connector-c-master/include/mysql.h"

int MysqlExecutor::Execute(const dbtool::MysqlSchemaConf& cfg) {
	for (int idx_schema = 0; idx_schema < cfg.mysql_schemas_size(); ++idx_schema) {
		auto& schema = cfg.mysql_schemas(idx_schema);

		MYSQL _st_mysql;
		mysql_init(&_st_mysql);
		/*SqlConnectionPtr sql_conn = MysqlPool::GetConnection(schema.mysql_ip().c_str(), schema.mysql_user().c_str(),
			schema.mysql_passwd().c_str(), "", schema.mysql_port());
		if (!sql_conn) {
			LogError("connect mysql fail: " << schema.mysql_ip());
			return false;
		}*/
	}
	return true;
}