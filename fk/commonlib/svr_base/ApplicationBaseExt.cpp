#include "commonlib/svr_base/ApplicationBase.h"
#include "protolib/src/svr_base.pb.h"
#include "slience/base/logger.hpp"
#include "commonlib/svr_base/ApplicationFunc.hpp"
#include "commonlib/transaction/transaction_mgr.h"

int ApplicationBase::Run() {
	base::slist<TcpSocketMsg*> tmp_tcp_socket_msg_list;
	base::slist<TcpSocketMsg*> tmp_tcp_socket_msg_list2;
	base::slist<TcpConnectorMsg*> tmp_tcp_connector_msg_list;
	base::slist<TcpConnectorMsg*> tmp_tcp_connector_msg_list2;

	auto &tcp_socket_fd_index = _tcp_socket_container.get<tag_socket_context_fd>();
	auto &tcp_conn_fd_index = _tcp_connector_container.get<tag_socket_context_fd>();

	while (!gAppExist) {
		base::timestamp t_now;
		if (t_now.millisecond() > _now.millisecond()) {
			_now = t_now;
			TransactionMgr::Update(_now);
			OnTick(_now);
			CheckTcpSocketExpire(_now);
		}
		if (CheckReload()) {
			LogInfo("reload begin.................");
			OnReload();
			LogInfo("reload end....................");
		}

		// swap message
		if (_tcp_socket_msg_list.size() || _tcp_connector_msg_list.size()) {
			_msg_lock.lock();
			tmp_tcp_socket_msg_list.swap(_tcp_socket_msg_list);
			tmp_tcp_connector_msg_list.swap(_tcp_connector_msg_list);
			_msg_lock.unlock();
		}

		// process connector msglist
		while (tmp_tcp_connector_msg_list.size()) {
			TcpConnectorMsg* pmsg = tmp_tcp_connector_msg_list.front();
			if (pmsg->type == M_SOCKET_DATA) {
				base::s_int64_t fd = M_TCP_CONNECTOR_FD_FLAG | pmsg->ptr->GetFd();
				auto tmp_iter = tcp_conn_fd_index.find(fd);
				if (tmp_iter != tcp_conn_fd_index.end()) {
					tcp_conn_fd_index.modify(tmp_iter, FuncModifySocketContext(tmp_iter->msgcount + 1, GetNow().second()));
					AppHeadFrame& pFrame = *(AppHeadFrame*)pmsg->buf.Data();
					const char* data = (const char*)pmsg->buf.Data() + sizeof(AppHeadFrame);
					OnProc(fd, pFrame, data, pFrame.get_cmd_length());
				}
				else {
					LogError(fd << " fd not found in _tcp_connector_container");
				}
			}
			else if (pmsg->type == M_SOCKET_IN) {
				OnConnection(pmsg->ptr, pmsg->error);
			}
			else if (pmsg->type == M_SOCKET_OUT) {
				OnDisConnection(pmsg->ptr);
			}
			else {
				assert(0);
			}
			tmp_tcp_connector_msg_list.pop_front();
			tmp_tcp_connector_msg_list2.push_back(pmsg);
		}

		// process tcp socket msg list
		while (tmp_tcp_socket_msg_list.size()) {
			TcpSocketMsg* pmsg = tmp_tcp_socket_msg_list.front();
			if (pmsg->type == M_SOCKET_DATA) {
				base::s_int64_t fd = M_TCP_FD_FLAG | pmsg->ptr->GetFd();
				auto tmp_iter = tcp_socket_fd_index.find(fd);
				if (tmp_iter != tcp_socket_fd_index.end()) {
					tcp_socket_fd_index.modify(tmp_iter, FuncModifySocketContext(tmp_iter->msgcount + 1, GetNow().second()));
					AppHeadFrame& pFrame = *(AppHeadFrame*)pmsg->buf.Data();
					const char* data = (const char*)pmsg->buf.Data() + sizeof(AppHeadFrame);
					OnProc(fd, pFrame, data, pFrame.get_cmd_length());
				}
				else {
					LogError(fd << " fd not found in _tcp_socket_container");
				}
			}
			else if (pmsg->type == M_SOCKET_IN) {
				OnConnection(pmsg->ptr);
			}
			else if (pmsg->type == M_SOCKET_OUT) {
				OnDisConnection(pmsg->ptr);
			}
			else {
				assert(0);
			}
			tmp_tcp_socket_msg_list.pop_front();
			tmp_tcp_socket_msg_list2.push_back(pmsg);
		}

		// recycle msg buffer
		if (tmp_tcp_connector_msg_list2.size() || tmp_tcp_socket_msg_list2.size()) {
			_msg_lock.lock();
			// tmp_tcp_connector_msg_list2
			if ((tmp_tcp_connector_msg_list2.size() + _tcp_connector_msg_list2.size()) <= _msg_cache_size) {
				_tcp_connector_msg_list2.join(tmp_tcp_connector_msg_list2);
			}
			else {
				while (tmp_tcp_connector_msg_list2.size()) {
					if (_tcp_connector_msg_list2.size() < _msg_cache_size) {
						_tcp_connector_msg_list2.push_back(tmp_tcp_connector_msg_list2.front());
					}
					else {
						delete tmp_tcp_connector_msg_list2.front();
					}
					tmp_tcp_connector_msg_list2.pop_front();
				}

			}
			// tmp_tcp_socket_msg_list2
			if ((tmp_tcp_socket_msg_list2.size() + _tcp_socket_msg_list2.size()) <= _msg_cache_size) {
				_tcp_socket_msg_list2.join(tmp_tcp_socket_msg_list2);
			}
			else {
				while (tmp_tcp_socket_msg_list2.size()) {
					if (_tcp_socket_msg_list2.size() < _msg_cache_size) {
						_tcp_socket_msg_list2.push_back(tmp_tcp_socket_msg_list2.front());
					}
					else {
						delete tmp_tcp_socket_msg_list2.front();
					}
					tmp_tcp_socket_msg_list2.pop_front();
				}
			}
			_msg_lock.unlock();
		}
		else {
			Sleep(1);
		}
	}
	OnExit();
	LogInfo("application exit..................");
	return 0;
}

void ApplicationBase::OnConnected(netiolib::TcpSocketPtr& clisock) {
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

void ApplicationBase::OnConnected(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) {
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

void ApplicationBase::OnDisconnected(netiolib::TcpSocketPtr& clisock) {
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

void ApplicationBase::OnDisconnected(netiolib::TcpConnectorPtr& clisock) {
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

void ApplicationBase::OnReceiveData(netiolib::TcpSocketPtr& clisock, SocketLib::Buffer& buffer) {
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

void ApplicationBase::OnReceiveData(netiolib::TcpConnectorPtr& clisock, SocketLib::Buffer& buffer) {
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