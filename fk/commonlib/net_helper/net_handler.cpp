#include "commonlib/net_helper/net_handler.h"
#include "slience/base/logger.hpp"
#include "protolib/src/cmd.pb.h"
#include "protolib/src/svr_base.pb.h"

NetIoHandler::NetIoHandler(base::timestamp& now, callback_type callback) 
	: _now(now) {
	_msg_cache_size = 5000;
	_callback = callback;
}

int NetIoHandler::Update() {
	return 0;
}

void NetIoHandler::OnTick() {
	CheckTcpSocketExpire();
}

const base::timestamp& NetIoHandler::GetNow()const {
	return _now;
}

void NetIoHandler::CheckTcpSocketExpire() {
	if (GetNow().second() - _last_check_time.second() >= M_EXPIRE_CHECK_INTERVAL) {
		// ten second
		auto &tt_index = _tcp_socket_container.get<tag_socket_context_active>();
		for (auto iter = tt_index.begin(); iter != tt_index.end(); ++iter) {
			if ((GetNow().second() - iter->tt) >= M_EXPIRE_INTERVAL
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
		_last_check_time = GetNow();
	}
}

void NetIoHandler::OnConnection(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) {

}

void NetIoHandler::OnConnection(netiolib::TcpSocketPtr& clisock) {
	base::s_int64_t fd = M_TCP_FD_FLAG | clisock->GetFd();
	TcpSocketContext context;
	context.svr_type = -1;
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
		_callback(fd, frame, str.c_str(), str.size());
	}
}

void NetIoHandler::OnDisConnection(netiolib::TcpConnectorPtr& clisock) {

}

void NetIoHandler::OnDisConnection(netiolib::TcpSocketPtr& clisock) {

}

///////////////////////////////////////////////////////////////////////////////

void NetIoHandler::OnConnected(netiolib::TcpSocketPtr& clisock) {
	base::ScopedLock scoped(_msg_lock);
	TcpSocketMsg* pMessage = 0;
	if (_tcp_socket_msg_list2.size() > 0) {
		pMessage = _tcp_socket_msg_list2.front();
		_tcp_socket_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpSocketMsg;
	}
	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_IN;
	_tcp_socket_msg_list.push_back(pMessage);
}

void NetIoHandler::OnConnected(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) {
	base::ScopedLock scoped(_msg_lock);
	TcpConnectorMsg* pMessage = 0;
	if (_tcp_connector_msg_list2.size() > 0) {
		pMessage = _tcp_connector_msg_list2.front();
		_tcp_connector_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpConnectorMsg;
	}

	pMessage->error = error;
	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_IN;
	_tcp_connector_msg_list.push_back(pMessage);
}

void NetIoHandler::OnDisconnected(netiolib::TcpSocketPtr& clisock) {
	base::ScopedLock scoped(_msg_lock);
	TcpSocketMsg* pMessage = 0;
	if (_tcp_socket_msg_list2.size() > 0) {
		pMessage = _tcp_socket_msg_list2.front();
		_tcp_socket_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpSocketMsg;
	}
	;
	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_OUT;
	_tcp_socket_msg_list.push_back(pMessage);
}

void NetIoHandler::OnDisconnected(netiolib::TcpConnectorPtr& clisock) {
	base::ScopedLock scoped(_msg_lock);
	TcpConnectorMsg* pMessage = 0;
	if (_tcp_connector_msg_list2.size() > 0) {
		pMessage = _tcp_connector_msg_list2.front();
		_tcp_connector_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpConnectorMsg;
	}

	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_OUT;
	_tcp_connector_msg_list.push_back(pMessage);
}

void NetIoHandler::OnReceiveData(netiolib::TcpSocketPtr& clisock, SocketLib::Buffer& buffer) {
	base::ScopedLock scoped(_msg_lock);
	if (_tcp_socket_msg_list.size() >= M_MAX_MESSAGE_LIST) {
		// message list is too many
		LogError("tcp_socket_msg_list is too many, new message will be dropped");
		return;
	}

	TcpSocketMsg* pMessage = 0;
	if (_tcp_socket_msg_list2.size() > 0) {
		pMessage = _tcp_socket_msg_list2.front();
		_tcp_socket_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpSocketMsg;
	}

	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_DATA;
	pMessage->buf.Write(buffer.Data(), buffer.Length());
	_tcp_socket_msg_list.push_back(pMessage);
}

void NetIoHandler::OnReceiveData(netiolib::TcpConnectorPtr& clisock, SocketLib::Buffer& buffer) {
	base::ScopedLock scoped(_msg_lock);
	if (_tcp_connector_msg_list.size() >= M_MAX_MESSAGE_LIST) {
		// message list is too many
		LogError("tcp_connector_msg_list is too many, new message will be dropped");
		return;
	}

	TcpConnectorMsg* pMessage = 0;
	if (_tcp_connector_msg_list2.size() > 0) {
		pMessage = _tcp_connector_msg_list2.front();
		_tcp_connector_msg_list2.pop_front();
	}
	else {
		pMessage = new TcpConnectorMsg;
	}

	pMessage->ptr = clisock;
	pMessage->type = M_SOCKET_DATA;
	pMessage->buf.Write(buffer.Data(), buffer.Length());
	_tcp_connector_msg_list.push_back(pMessage);
}