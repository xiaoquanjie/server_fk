#include "redis_helper.h"

ExpireRedisCmd::ExpireRedisCmd(const char* key, time_t expire) {
	cmd = "EXPIRE ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(expire));
}

DelRedisCmd::DelRedisCmd(std::initializer_list<char*> l) {
	cmd = "DEL ";
	for (auto iter = l.begin(); iter != l.end(); ++iter) {
		cmd.append(*iter);
		cmd.append(" ");
	}
}

SetRedisCmd::SetRedisCmd(const char* key, const char* value) {
	cmd = "SET ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(value);
}

SetNxRedisCmd::SetNxRedisCmd(const char* key, const char* value) {
	cmd = "SETNX ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(value);
}

SetExRedisCmd::SetExRedisCmd(const char* key, const char* value, time_t expire) {
	cmd = "SETEX ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(expire));
	cmd.append(" ");
	cmd.append(value);
}

GetRedisCmd::GetRedisCmd(const char* key) {
	cmd = "GET ";
	cmd.append(key);
}

IncrbyRedisCmd::IncrbyRedisCmd(const char* key, int step) {
	cmd = "INCRBY ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(step));
}

DecrbyRedisCmd::DecrbyRedisCmd(const char* key, int step) {
	cmd = "DECRBY ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(step));
}

StrlenRedisCmd::StrlenRedisCmd(const char* key) {
	cmd = "STRLEN ";
	cmd.append(key);
}

AppendRedisCmd::AppendRedisCmd(const char* key, const char* value) {
	init(key, value, strlen(value));
}

AppendRedisCmd::AppendRedisCmd(const char* key, const char* value, unsigned int len) {
	init(key, value, len);
}

void AppendRedisCmd::init(const char* key, const char* value, unsigned int len) {
	cmd = "APPEND ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(value, len);
}

SetRangeRedisCmd::SetRangeRedisCmd(const char* key, int beg_idx, const char* value) {
	cmd = "SETRANGE ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(beg_idx));
	cmd.append(" ");
	cmd.append(value);
}

GetRangeRediCmd::GetRangeRediCmd(const char* key, int beg_idx, int end_idx) {
	cmd = "GETRANGE ";
	cmd.append(key);
	cmd.append(" ");
	cmd.append(std::to_string(beg_idx));
	cmd.append(" ");
	cmd.append(std::to_string(end_idx));
}

/////////////////////////////////////////////////////////////////////////////////////

RedisReplyParser::RedisReplyParser(redisReply* reply) {
	_reply = reply;
}

RedisReplyParser::~RedisReplyParser() {
	if (_reply) {
		freeReplyObject(_reply);
	}
}

void RedisReplyParser::GetInteger(long long& value) {
	RedisException error;
	do {
		if (!_reply) {
			error = RedisException(M_ERR_REDIS_REPLY_NULL);
			break;
		}
		if (_reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(_reply->str);
			break;
		}
		if (_reply->type != REDIS_REPLY_INTEGER) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		value = _reply->integer;
	} while (false);
	if (!error.Empty())
		throw error;
}

void RedisReplyParser::GetString(std::string& value) {
	RedisException error;
	do {
		if (!_reply) {
			error = RedisException(M_ERR_REDIS_REPLY_NULL);
			break;
		}
		if (_reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(_reply->str);
			break;
		}
		if (_reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		value.clear();
		value.append(_reply->str, _reply->len);
	} while (false);

	if (!error.Empty())
		throw error;
}

void RedisReplyParser::GetOk(bool& value) {
	RedisException error;
	do {
		if (!_reply) {
			error = RedisException(M_ERR_REDIS_REPLY_NULL);
			break;
		}
		if (_reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(_reply->str);
			break;
		}
		if (_reply->type != REDIS_REPLY_STATUS) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		if (strcasecmp(_reply->str, "OK") != 0) {
			value = false;
		}
		else {
			value = true;
		}
	} while (false);

	if (!error.Empty())
		throw error;
}
