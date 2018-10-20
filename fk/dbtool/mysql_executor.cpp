#include <windows.h>
#include "dbtool/mysql_executor.h"
#include "slience/base/logger.hpp"
#include "mariadb-connector-c-master/include/mysql.h"

int MysqlExecutor::Execute(const dbtool::MysqlSchemaConf& cfg) {
	for (int idx_schema = 0; idx_schema < cfg.mysql_schemas_size(); ++idx_schema) {
		auto& schema = cfg.mysql_schemas(idx_schema);

		// connection
		SqlConnectionPtr sql_conn = MysqlPool::GetConnection(schema.mysql_ip().c_str(), schema.mysql_user().c_str(),
			schema.mysql_passwd().c_str(), "", schema.mysql_port());
		if (!sql_conn) {
			LogError("Error: fail to connect mysql " << schema.mysql_ip());
			return -1;
		}

		// create schema
		int ret = CreateSchema(sql_conn, schema);
		if (ret != 0) {
			return -1;
		}

		// use db
		if (!MysqlPool::UseDb(sql_conn, schema.schema_name().c_str())) {
			LogError("Error: fail to use database " << schema.schema_name() << " in " << schema.mysql_ip());
			LogError(sql_conn->GetErrorMsg());
			return -1;
		}

		// create table
		for (auto iter = schema.tables().begin(); iter != schema.tables().end(); ++iter) {
			auto& table = *iter;
			int ret = CreateTable(sql_conn, schema.schema_name(), table);
			if (ret != 0) {
				return -1;
			}
		}

	}
	return 0;
}

int MysqlExecutor::CreateSchema(SqlConnectionPtr conn_ptr, const dbtool::MysqlSchema& schema) {
	std::string sql = "CREATE DATABASE IF NOT EXISTS " + schema.schema_name();
	sql += " CHARACTER SET utf8 COLLATE utf8_unicode_ci;";

	int ret = conn_ptr->Execute(sql.c_str(), sql.length());
	if (ret != 0) {
		LogError("Error: fail to create database " << schema.schema_name() << " in " << schema.mysql_ip());
		LogError(conn_ptr->GetErrorMsg());
	}
	else {
		LogInfo("create database success: " << schema.schema_name() << " in " << schema.mysql_ip())
	}
	return ret;
}

int MysqlExecutor::CreateTable(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	do {
		std::vector<std::string> old_table_fields;
		if (0 != GetTableFields(conn_ptr, schema_name, table, old_table_fields)) {
			break;
		}
		if (old_table_fields.empty()) {
			if (0 != CreateNewTable(conn_ptr, schema_name, table)) {
				break;
			}
		}
		if (0 != ChangeTable(conn_ptr, schema_name, table)) {
			break;
		}
		return 0;
	} while (false);
	return -1;
}

int MysqlExecutor::GetTableFields(SqlConnectionPtr conn_ptr, const std::string& schema_name, 
	const dbtool::MysqlTable& table, std::vector<std::string>& table_fields) {
	std::string sql = "SELECT DISTINCT COLUMN_NAME FROM information_schema.columns where table_schema='";
	sql += schema_name;
	sql += "'";
	sql += " and table_name='";
	sql += table.table_name();
	sql += "';";

	int ret = conn_ptr->Query(sql.c_str(), sql.length(), 1, [&table_fields](int idx, MYSQL_ROW row) {
		table_fields.push_back(row[0]);
	});

	if(ret != 0) {
		LogError("Error: fail to query table " << table.table_name() << "'s column name");
	}
	return ret;
}

int MysqlExecutor::CreateNewTable(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	std::string sql = "CREATE TABLE ";
	sql += table.table_name();
	sql += "(";
	for (auto iter = table.fields().begin(); iter != table.fields().end(); ++iter) {
		auto& field = *iter;
		if (iter != table.fields().begin()) {
			sql += ", ";
		}
		sql += "`";
		sql += field.name();
		sql += "` ";

		// 接field类型
		std::string desc;
		sql += GetFieldTypeDesc(field);
		
		if (field.not_null()) {
			sql += " NOT NULL";
		}
		if (field.auto_incr()) {
			sql += " AUTO_INCREMENT";
		}
		if (field.default_().size()) {
			sql += " DEFAULT '" + field.default_() + "'";
		}
	}

	for (auto iter = table.keys().begin(); iter != table.keys().end(); ++iter) {
		auto& key = *iter;
		sql += ", ";
		sql += GetFieldKeyDesc(key);
	}

	sql += ") CHARACTER SET utf8 COLLATE utf8_unicode_ci;";

	int ret = conn_ptr->Execute(sql.c_str(), sql.length());
	if (ret == 0) {
		LogInfo("create table " << table.table_name() << " in database " << schema_name << " success");
	}
	else {
		LogError("Error: fail to create table " << table.table_name() << " in database " << schema_name);
	}

	return ret;
}

int MysqlExecutor::ChangeTable(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	do {
		// 执行改名
		if (0 != RenameTableColumn(conn_ptr, schema_name, table)) {
			break;
		}

		// 更改类型
		if (0 != ModifyTableColumn(conn_ptr, schema_name, table)) {
			break;
		}

		// 执行删除列
		if (0 != DeleteTableColumn(conn_ptr, schema_name, table)) {
			break;
		}

		return 0;
	} while (false);
	return -1;
}

std::string MysqlExecutor::GetFieldTypeDesc(const dbtool::TableField& field) {
	return _GetFieldTypeDesc(field, field.type());
}

std::string MysqlExecutor::GetFieldModifyTypeDesc(const dbtool::TableField& field) {
	return _GetFieldTypeDesc(field, field.modify_type());
}

std::string MysqlExecutor::GetFieldKeyDesc(const dbtool::TableKey& key) {
	std::string desc;
	switch (key.type()) {
	case dbtool::E_KeyType_Primary:
		desc += "PRIMARY KEY(";
		break;
	case dbtool::E_KeyType_Normal:
		desc += "KEY `";
		desc += key.name();
		desc += "`(";
		break;
	case dbtool::E_KeyType_Unique:
		desc += "UNIQUE KEY `";
		desc += key.name();
		desc += "`(";
		break;
	default:
		break;
	}
	for (int idx = 0; idx < key.fields_size(); ++idx) {
		if (idx > 0) {
			desc += ", ";
		}
		desc += "`" + key.fields(idx) + "`";
	}
	desc += ")";
	return desc;
}

int MysqlExecutor::RenameTableColumn(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	std::vector<std::string> table_fields;
	if (0 != GetTableFields(conn_ptr, schema_name, table, table_fields)) {
		LogError("Error: fail to delete column in" << table.table_name() << " in table.table_name() in " << schema_name)
			return -1;
	}

	std::map<std::string, const dbtool::TableField*> want_rename_fields;
	for (auto iter = table.fields().begin(); iter != table.fields().end(); ++iter) {
		auto& field = *iter;
		if (field.has_rename_to()) {
			want_rename_fields[field.name()] = &field;
		}
	}

	// 剔除不存在的列
	for (auto iter = want_rename_fields.begin(); iter != want_rename_fields.end(); ) {
		bool flag = false;
		for (auto iter2 = table_fields.begin(); iter2 != table_fields.end(); ++iter2) {
			if (*iter2 == iter->first) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			want_rename_fields.erase(iter++);
		}
		else {
			++iter;
		}
	}

	for (auto iter = want_rename_fields.begin(); iter != want_rename_fields.end(); ++iter) {
		std::string sql = "ALTER TABLE " + table.table_name();
		sql += " CHANGE " + iter->first;
		sql += " " + iter->second->rename_to();
		sql += " " + GetFieldTypeDesc(*(iter->second));
		sql += ";";
		int ret = conn_ptr->Execute(sql.c_str(), sql.length());
		if (ret != 0) {
			LogError("Error: rename from " << iter->first << " to " << iter->second->rename_to() << " in table " << table.table_name() << " in database " << schema_name);
			return -1;
		}
		else {
			LogInfo("rename from " << iter->first << " to " << iter->second->rename_to() << " successfully in table " << table.table_name() << " in database " << schema_name);
		}
	}
	return 0;
}

int MysqlExecutor::ModifyTableColumn(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	std::vector<std::string> table_fields;
	if (0 != GetTableFields(conn_ptr, schema_name, table, table_fields)) {
		LogError("Error: fail to delete column in" << table.table_name() << " in table.table_name() in " << schema_name)
			return -1;
	}

	std::map<std::string, const dbtool::TableField*> want_modify_fields;
	for (auto iter = table.fields().begin(); iter != table.fields().end(); ++iter) {
		auto& field = *iter;
		if (field.has_modify_type()) {
			want_modify_fields[field.name()] = &field;
		}
	}

	// 剔除不存在的列
	for (auto iter = want_modify_fields.begin(); iter != want_modify_fields.end(); ) {
		bool flag = false;
		for (auto iter2 = table_fields.begin(); iter2 != table_fields.end(); ++iter2) {
			if (*iter2 == iter->first) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			want_modify_fields.erase(iter++);
		}
		else {
			++iter;
		}
	}

	for (auto iter = want_modify_fields.begin(); iter != want_modify_fields.end(); ++iter) {
		std::string sql = "ALTER TABLE " + table.table_name();
		sql += " MODIFY " + iter->first;
		sql += " " + GetFieldModifyTypeDesc(*(iter->second));
		sql += ";";
		int ret = conn_ptr->Execute(sql.c_str(), sql.length());
		if (ret != 0) {
			LogError("Error: fail to modify type in field " << iter->first << " in table " << table.table_name() << " in database " << schema_name);
			return -1;
		}
		else {
			LogInfo("modify type in field " << iter->first << " successfully in table " << table.table_name() << " in database " << schema_name);
		}
	}
	return 0;
}

int MysqlExecutor::DeleteTableColumn(SqlConnectionPtr conn_ptr, const std::string& schema_name, const dbtool::MysqlTable& table) {
	std::vector<std::string> table_fields;
	if (0 != GetTableFields(conn_ptr, schema_name, table, table_fields)) {
		LogError("Error: fail to delete column in" << table.table_name() << " in table.table_name() in " << schema_name)
		return -1;
	}

	std::vector<std::string> want_del_fields;
	for (auto iter = table.fields().begin(); iter != table.fields().end(); ++iter) {
		auto& field = *iter;
		if (field.is_delete()) {
			want_del_fields.push_back(field.name());
		}
	}

	// 剔除不存在的列
	for (auto iter = want_del_fields.begin(); iter != want_del_fields.end();) {
		bool flag = false;
		for (auto iter2 = table_fields.begin(); iter2 != table_fields.end(); ++iter2) {
			if (*iter2 == *iter) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			iter = want_del_fields.erase(iter);
		}
		else {
			++iter;
		}
	}

	std::string sql;
	std::string col_str;
	for (auto iter = want_del_fields.begin(); iter != want_del_fields.end(); ++iter) {
		if (sql.empty()) {
			sql = "ALTER TABLE " + table.table_name();
			col_str += *iter;
		}
		else {
			sql += ", ";
			col_str += ", " + *iter;
		}
		sql += " DROP COLUMN " + *iter;
	}

	if (sql.size()) {
		sql += ";";
		int ret = conn_ptr->Execute(sql.c_str(), sql.length());
		if (ret == 0) {
			LogInfo("delete column " << col_str << " successfully in " << table.table_name() << " in " << schema_name);
		}
		else {
			LogInfo("Error: fail to delete column " << col_str << " in table.table_name() in " << schema_name);
			LogInfo(conn_ptr->GetErrorMsg());
			return -1;
		}
	}
	return 0;
}

std::string MysqlExecutor::_GetFieldTypeDesc(const dbtool::TableField& field, dbtool::MysqlFieldType type) {
	std::string desc;
	switch (type) {
	case dbtool::E_FieldType_TinyInt:
		desc += "TINYINT";
		break;
	case dbtool::E_FieldType_Int:
		desc += "INT";
		break;
	case dbtool::E_FieldType_UInt:
		desc += "INT UNSIGNED";
		break;
	case dbtool::E_FieldType_BigInt:
		desc += "BIGINT";
		break;
	case dbtool::E_FieldType_Double:
		desc += "DOUBLE";
		break;
	case dbtool::E_FieldType_Varchar:
		desc += "VARCHAR(";
		desc += std::to_string(field.varchar_len());
		desc += ")";
		break;
	case dbtool::E_FieldType_Blob:
		desc += "BLOB";
		break;
	case dbtool::E_FieldType_MediumBlob:
		desc += "MEDIUMBLOB";
		break;
	case dbtool::E_FieldType_Text:
		desc += "TEXT";
		break;
	case dbtool::E_FieldType_Date:
		desc += "DATE";
		break;
	case dbtool::E_FieldType_Time:
		desc += "TIME";
		break;
	case dbtool::E_FieldType_TimeStamp:
		desc += "TIMESTAMP";
		break;
	default:
		break;
	}
	return desc;
}