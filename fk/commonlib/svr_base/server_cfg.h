#ifndef M_SVR_BASE_SERVER_CFG_INCLUDE
#define M_SVR_BASE_SERVER_CFG_INCLUDE

#include "google/protobuf/text_format.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "slience/base/logger.hpp"
#include "slience/base/compatibility.hpp"

template<typename CfgType>
class ServerCfg {
public:
	int Parse(const char* file_path) {
		int fd = myfopen(file_path, "rt", _SH_DENYRW);
		if (fd < 0) {
			LogError("open server cfg file failed, file_path=" << file_path);
			return -1;
		}

		google::protobuf::io::FileInputStream fileInput(fd);
		if (!google::protobuf::TextFormat::Parse(&fileInput, &_cfg)) {
			LogError(0, 0, "google::protobuf::TextFormat::Parse failed, file_path=" << file_path);
			fileInput.Close();
			return -2;
		}
		fileInput.Close();
		return 0;
	}

	const CfgType& Data() const {
		return _cfg;
	}

private:
	CfgType _cfg;
};

#endif