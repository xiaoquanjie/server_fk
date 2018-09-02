#include "commonlib/svr_base/ApplicationBase.h"
#include "slience/base/string_util.hpp"
#include "slience/base/logger.hpp"
#include "commonlib/svr_base/getopt.hpp"
#include "commonlib/svr_base/ApplicationFunc.hpp"

#include "mysql_wrapper/mysql_wrapper.h"
#include "protolib/src/svr_base.pb.h"
#include "protolib/src/cmd.pb.h"
#include "commonlib/transaction/transaction_mgr.h"

/////////////////////////////////////////////////////////////

ApplicationBase::ApplicationBase() {
	_log_level = base::logger::LOG_LEVEL_TRACE;
	_log_withpid = 0;
	_daemon = 0;
	_msg_cache_size = 5000;
	_svr_thread_cnt = 4;
}

ApplicationBase::~ApplicationBase() {

}

int ApplicationBase::Init(int argc, char** argv) {
	int ret = 0;
	do {
		// set core file unlimit
		CoreFileUnlimit();

		ret = ParseOpt(argc, argv);
		if (0 != ret)
			break;

		// set work dir
		SetWorkDir(_workdir);

		// can't start two process
		ret = KillExist(_pid_file);
		if (ret != 0) {
			printf("can't kill old process\n");
			exit(0);
		}

		// set daemon
		if (_daemon == 1) {
			Daemon();
		}

		// init signal
		InitSigHandler();

		// init log
		SetLogFileName(_log_file, (bool)_log_withpid);
		SetLogLevel(_log_level);

		ret = WritePid(_pid_file);
		if (0 != ret) {
			break;
		}

		ret = OnReload();
		if (ret != 0) {
			LogError("reload error........");
			break;
		}

		// start network thread
		if (_svr_thread_cnt > 0 && _svr_thread_cnt < 100) {
			Start(_svr_thread_cnt, false);
		}

		ret = OnInit();
		if (ret != 0) {
			LogError("application init error.....");
			break;
		}

		// init transaction manager
		TransactionMgr::Init();

	} while (false);
	  
	if (0 == ret) {
		LogInfo("application param : workdir=" << GetWorkDir());
		LogInfo("application param : appname=" << _appname);
		LogInfo("application param : log_file=" << _log_file);
		LogInfo("application param : log_level=" << _log_level);
		LogInfo("application start successfully");
	}
	else {
		LogError("application start fail");
		// stop network 
		Stop();
		exit(-1);
	}
	return ret;
}

const std::string& ApplicationBase::ConfigFilePath()const {
	return _conf_file;
}

const std::string& ApplicationBase::AppName()const {
	return _appname;
}

const std::string& ApplicationBase::PidFile()const {
	return _pid_file;
}

const base::timestamp& ApplicationBase::GetNow()const {
	return _now;
}

void ApplicationBase::RegisterServer(int server_type, int instance_id, base::s_int64_t fd) {
	if (M_CHECK_IS_TCP_FD(fd)) {
		int real_fd = M_GET_TCP_FD(fd);
		auto &fd_idx = _tcp_socket_container.get<tag_socket_context_fd>();
		auto iter = fd_idx.find(real_fd);
		if (iter != fd_idx.end()) {
			fd_idx.modify(iter, FuncAddSocketContextInstId(instance_id));
			_instid_fd_map[instance_id] = fd;
			LogInfo("register {server_type: " << server_type << " instance_id: " << instance_id << "}");
		}
	}
	else if (M_CHECK_IS_TCP_CONNECTOR_FD(fd)) {
		int real_fd = M_GET_TCP_FD(fd);
		real_fd = M_GET_TCP_CONNECTOR_FD(fd);
		auto &fd_idx = _tcp_connector_container.get<tag_socket_context_fd>();
		auto iter = fd_idx.find(real_fd);
		if (iter != fd_idx.end()) {
			fd_idx.modify(iter, FuncAddSocketContextInstId(instance_id));
			_instid_fd_map[instance_id] = fd;
			LogInfo("register {server_type: " << server_type << " instance_id: " << instance_id << "}");
		}
	}
}

void ApplicationBase::SendNetWorkData(int instid, const char* data, base::s_int32_t len) {
	auto fd_iter = _instid_fd_map.find(instid);
	if (fd_iter == _instid_fd_map.end()) {
		LogError("instid: " << instid << " connection is broken");
		return;
	}
	int real_fd = 0;
	base::s_int64_t fd = fd_iter->second;

	if (M_CHECK_IS_TCP_FD(fd)) {
		real_fd = M_GET_TCP_FD(fd);
		auto &fd_idx = _tcp_socket_container.get<tag_socket_context_fd>();
		auto iter = fd_idx.find(real_fd);
		if (iter != fd_idx.end()) {
			iter->ptr->Send(data, len);
		}
		else {
			LogError("instid: " << instid << " this is a big bug");
		}
	}
	else if (M_CHECK_IS_TCP_CONNECTOR_FD(fd)) {
		real_fd = M_GET_TCP_CONNECTOR_FD(fd);
		auto &fd_idx = _tcp_connector_container.get<tag_socket_context_fd>();
		auto iter = fd_idx.find(real_fd);
		if (iter != fd_idx.end()) {
			iter->ptr->Send(data, len);
		}
	}
}

int ApplicationBase::OnTick(const base::timestamp& now) {
	return 0;
}


int ApplicationBase::ParseOpt(int argc, char** argv) {
	_workdir = base::StringUtil::directory(argv[0]);
	_appname = base::StringUtil::basename(argv[0]);
	_appname = base::StringUtil::remove_from_end(_appname, ".exe");
	_pid_file = _appname + ".pid";

	if (argc < 2) {
		Usage();
		exit(0);
	}

	static int opt_char = 0;
	static base::Option long_option[] = {
		{"config_file", 1, &opt_char, 'C'},
		{"help", 0, 0, 'h'},
		{"daemon", 0, 0, 'D'},
		{"log_file", 1, &opt_char, 'L'},
		{"log_level", 1, &opt_char, 'l'},
		{"log_withpid", 1, &opt_char, 'p'},
		{"thread_cnt", 1, &opt_char, 'T'}
	};

	int opt_idx = 0;
	int opt;
	while (-1 != (opt = base::GetOptLong(argc, argv, "DHh", long_option, &opt_idx))) {
		switch (opt) {
		case 0:
			switch (*long_option[opt_idx].flag) {
			case 'C':
				_conf_file = base::GetOptArg();
				break;
			case 'L':
				_log_file = base::GetOptArg();
				break;
			case 'l':
				_log_level = base::GetOptArgI();
				break;
			case 'p':
				_log_withpid = base::GetOptArgI();
				break;
			case 'T':
				_svr_thread_cnt = base::GetOptArgI();
				break;
			}
			break;
		case 'D':
			_daemon = 1;
			break;
		case 'H':
		case 'h':
		case '?':
		default:
			Usage();
			exit(0);
			break;
		}
	}
	
	if (_log_file.empty()) {
		_log_file = _appname;
	}

	return 0;
}

void ApplicationBase::Usage()const {
	printf("Usage:\n");
	printf("required arguments:\n");
	printf("--config_file		the config file path\n");

	printf("\noptional arguments:\n");
	printf("--log_file		the log file path\n");
	printf("--log_level		the log level\n");
}

bool ApplicationBase::CheckReload() {
	static base::timestamp last_check_time;
	if ((GetNow().second() - last_check_time.second()) > 2) {
		last_check_time = GetNow();

		static const char* filename = "_reload_";
		FILE *fp = myfopen(filename, "r", _SH_DENYNO);
		if (fp) {
			fclose(fp);
			remove(filename);
			return true;
		}
	}
	return false;
}

void ApplicationBase::CheckTcpSocketExpire(const base::timestamp& now) {
	static base::timestamp last_check_time;
	if (now.second() - last_check_time.second() >= M_EXPIRE_CHECK_INTERVAL) {
		// ten second
		auto &tt_index = _tcp_socket_container.get<tag_socket_context_active>();
		for (auto iter = tt_index.begin(); iter != tt_index.end(); ++iter) {
			if ((now.second() - iter->tt) >= M_EXPIRE_INTERVAL
				&& iter->ptr->IsConnected()) {
				base::s_int64_t fd = M_TCP_FD_FLAG | iter->ptr->GetFd();
				LogInfo("connection expire been closed, remote_ip: " << iter->ptr->RemoteEndpoint().Address()
					<< " fd: " << fd);
				iter->ptr->Close();
			}
			else {
				break;
			}
		}
		last_check_time = now;
	}
}

void ApplicationBase::OnConnection(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) {
	if (!error) {
		// connect success
		base::s_int64_t fd = M_TCP_CONNECTOR_FD_FLAG | clisock->GetFd();
		TcpConnectorContext context;
		context.instid = 0;
		context.fd = fd;
		context.ptr = clisock;
		context.msgcount = 0;
		context.tt = GetNow().second();

		auto &fd_index = _tcp_connector_container.get<tag_socket_context_fd>();
		if (!fd_index.insert(context).second) {
			LogError("new connection insert fail, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());
			// close socket
			clisock->Close();
		}
		else {
			LogInfo("new connection insert success, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());
			proto::SocketClientIn client_in;
			std::string str = client_in.SerializeAsString();
			AppHeadFrame frame;
			frame.set_cmd(proto::CMD::CMD_SOCKET_CLIENT_IN);
			OnProc(fd, frame, str.c_str(), str.size());
		}
	}
}

void ApplicationBase::OnConnection(netiolib::TcpSocketPtr& clisock) {
	base::s_int64_t fd = M_TCP_FD_FLAG | clisock->GetFd();
	TcpSocketContext context;
	context.instid = 0;
	context.fd = fd;
	context.ptr = clisock;
	context.msgcount = 0;
	context.tt = GetNow().second();

	auto &fd_index = _tcp_socket_container.get<tag_socket_context_fd>();
	if (!fd_index.insert(context).second) {
		LogError("new connection insert fail, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());
		// close socket
		clisock->Close();
	}
	else {
		LogInfo("new connection insert success, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());

		proto::SocketClientIn client_in;
		std::string str = client_in.SerializeAsString();
		AppHeadFrame frame;
		frame.set_cmd(proto::CMD::CMD_SOCKET_CLIENT_IN);
		OnProc(fd, frame, str.c_str(), str.size());
	}
}

void ApplicationBase::OnDisConnection(netiolib::TcpConnectorPtr& clisock) {
	base::s_int64_t fd = M_TCP_CONNECTOR_FD_FLAG | clisock->GetFd();
	auto &fd_index = _tcp_connector_container.get<tag_socket_context_fd>();
	if (0 == fd_index.erase(fd)) {
		LogError("fd: " << fd << " not exist, this is a big bug!!!!!!!!!!!!!!!!!");
	}
	else {
		proto::SocketClientOut client_out;
		std::string str = client_out.SerializeAsString();
		AppHeadFrame frame;
		frame.set_cmd(proto::CMD::CMD_SOCKET_CLIENT_OUT);

		LogInfo("connection broken, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());
		OnProc(fd, frame, str.c_str(), str.size());
	}
}

void ApplicationBase::OnDisConnection(netiolib::TcpSocketPtr& clisock) {
	base::s_int64_t fd = M_TCP_FD_FLAG | clisock->GetFd();
	auto &fd_index = _tcp_socket_container.get<tag_socket_context_fd>();
	auto iter = fd_index.find(fd);
	if (iter == fd_index.end()) {
		LogError("fd: " << fd << " not exist, this is a big bug!!!!!!!!!!!!!!!!!");
	}
	else {
		int instid = iter->instid;
		_instid_fd_map.erase(instid);
		fd_index.erase(iter);
		proto::SocketClientOut client_out;
		std::string str = client_out.SerializeAsString();
		AppHeadFrame frame;
		frame.set_dst_inst_id(instid);
		frame.set_cmd(proto::CMD::CMD_SOCKET_CLIENT_OUT);

		LogInfo("connection broken, remote_ip: " << clisock->RemoteEndpoint().Address() << " fd: " << fd << " time: " << GetNow().format_ymd_hms());
		OnProc(fd, frame, str.c_str(), str.size());
	}
}