#include "dbtool/mysql_executor.h"
#include "dbtool/dbtool.pb.h"
#include "slience/base/logger.hpp"
#include "slience/base/compatibility.hpp"
#include "commonlib/svr_base/server_cfg.h"
#include <set>
#include <map>

bool CheckMysqlSyntax(ServerCfg<dbtool::MysqlSchemaConf>& cfg) {
	for (int idx_schema = 0; idx_schema < cfg.Data().mysql_schemas_size(); ++idx_schema) {
		auto& schema = *(cfg.Data().mutable_mysql_schemas(idx_schema));
		for (int idx_table = 0; idx_table < schema.tables_size(); ++idx_table) {
			auto& table = *(schema.mutable_tables(idx_table));
			std::map<std::string, const dbtool::TableField*> field_map;
			// �ռ�field
			for (int idx_field = 0; idx_field < table.fields_size(); ++idx_field) {
				auto& field = table.fields(idx_field);
				if (field_map.find(field.name()) != field_map.end()) {
					LogError("Error: duplicate field: " << field.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
				field_map[field.name()] = &field;

				// ����ֶ����Ƿ��ͻ
				int tmp = 0;
				if (field.has_add_col_after()) {
					tmp++;
				}
				if (field.has_rename_from()) {
					tmp++;
				}
				if (field.has_modify_type()) {
					tmp++;
				}

				if (tmp >= 2) {
					// �������߲���ͬʱ��������������
					LogError("Error: add_col_after��rename_from��modify_type must be exclusive in field: " << field.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				if (field.has_rename_from()) {
					if (field.rename_from() == field.name()) {
						LogError("Error: rename_from is equal to name in field: " << field.name() << " in table: " << table.table_name()
							<< " in schema: " << schema.schema_name());
						return false;
					}
				}

				if (!field.has_type()) {
					LogError("Error: need has type in field: " << field.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
				if (field.type() < dbtool::E_FieldType_TinyInt
					|| field.type() > dbtool::E_FieldType_TimeStamp) {
					LogError("Error: illegal field type in field: " << field.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				// ����auto_incr�Ļ������ͱ���������
				if (field.has_auto_incr()) {
					switch (field.type()) {
					case dbtool::E_FieldType_TinyInt:
					case dbtool::E_FieldType_Int:
					case dbtool::E_FieldType_UInt:
					case dbtool::E_FieldType_BigInt:
					case dbtool::E_FieldType_Double:
						break;
					default:
						LogError("Error: auto_incr column must be integer type in field: " << field.name() << " in table: " << table.table_name()
							<< " in schema: " << schema.schema_name());
						return false;
					}
				}
			}

			std::map<std::string, dbtool::TableKey*> key_map;
			int primary_key_cnt = 0;
			// �ռ�keys
			for (int idx_key = 0; idx_key < table.keys_size(); ++idx_key) {
				auto& key = *(table.mutable_keys(idx_key));
				if (key_map.find(key.name()) != key_map.end()) {
					LogError("Error: duplicate key: " << key.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				key_map[key.name()] = &key;
				if (!key.has_type()) {
					LogError("Error: need have type in key: " << key.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
				if (key.type() < dbtool::E_KeyType_Primary
					|| key.type() > dbtool::E_KeyType_Unique) {
					LogError("Error: illegal type in key: " << key.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				if (key.type() == dbtool::E_KeyType_Primary) {
					key.set_name("PRIMARY");
					primary_key_cnt++;
				}

				// ���ܴ��ڶ������
				if (primary_key_cnt > 1) {
					LogError("Error: can't have over one primary key in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				// ��Ҫ��������
				if (key.fields_size() == 0) {
					LogError("Error: field can't be empty in key: " << key.name() << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}

				// ���ڲ������������ǺϷ�������
				std::set<std::string> key_field_set;
				for (int idx_key_field = 0; idx_key_field < key.fields_size(); ++idx_key_field) {
					auto key_field = key.fields(idx_key_field);
					if (field_map.find(key_field) == field_map.end()) {
						LogError("Error: key_field: " << key_field << " is illegal in key: " << key.name() << " in table: " << table.table_name()
							<< " in schema: " << schema.schema_name());
						return false;
					}

					if (!key_field_set.insert(key_field).second) {
						LogError("Error: duplicate key_field: " << key_field << " in key: " << key.name() << " in table: " << table.table_name()
							<< " in schema: " << schema.schema_name());
						return false;
					}
				}
			}
		
			// �����ӵ�field�����Ǽ�
			for (auto iter_field = field_map.begin(); iter_field != field_map.end(); ++iter_field) {
				if (!iter_field->second->has_auto_incr()) {
					continue;
				}
				bool flag = false;
				for (auto iter_key = key_map.begin(); iter_key != key_map.end(); ++iter_key) {
					for (int idx_key_field = 0; idx_key_field < iter_key->second->fields_size(); ++idx_key_field) {
						auto key_field = iter_key->second->fields(idx_key_field);
						if (key_field == iter_field->first) {
							flag = true;
							break;
						}
					}
				}
				if (!flag) {
					LogError("Error: auto_incr column has to be key in field: " << iter_field->first << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
			}

			// add_col_after �������field
			for (auto iter_field = field_map.begin(); iter_field != field_map.end(); ++iter_field) {
				if (!iter_field->second->has_add_col_after()) {
					continue;
				}
				if (iter_field->second->add_col_after() == iter_field->first) {
					LogError("Error: field name conflict with add_col_after in field: " << iter_field->first << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
				if (field_map.find(iter_field->second->add_col_after()) == field_map.end()) {
					LogError("Error: add_col_after: " << iter_field->second->add_col_after() << " column is illegal in field: " << iter_field->first << " in table: " << table.table_name()
						<< " in schema: " << schema.schema_name());
					return false;
				}
			}
}
	}
	
	return true;
}

int main(int argc, char* argv[]) {
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
				LogError("Error: parse file: " << argv[1] << " fail");
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

		// ����﷨
		for (auto iter = SchemaCfgVec.begin(); iter != SchemaCfgVec.end(); ++iter) {
			if (!CheckMysqlSyntax(*iter)) {
				flag = false;
				break;
			}
		}

		if (!flag) {
			break;
		}

		// ִ��
		MysqlExecutor executor;
		for (auto iter = SchemaCfgVec.begin(); iter != SchemaCfgVec.end(); ++iter) {
			auto& cfg = iter->Data();
			if (0 != executor.Execute(cfg)) {
				flag = false;
				break;
			}
		}

		if (flag) {
			LogInfo("dbtool finish successfully");
		}
		else {
			LogInfo("Error: dbtool fail to finish");
		}

	} while (false);

	Sleep(1000);
	StopLogger();
	return 0;
}