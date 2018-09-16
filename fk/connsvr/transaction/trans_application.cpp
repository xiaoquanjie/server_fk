#include "protolib/src/svr_base.pb.h"
#include "protolib/src/cmd.pb.h"
#include "commonlib/transaction/base_transaction.h"
#include "commonlib/transaction/transaction_mgr.h"
#include "commonlib/net_handler/router_mgr.h"
#include "commonlib/net_handler/svr_net_handler.h"
#include "connsvr/conn_svr.h"

class TransClientIn 
	: public BaseTransaction<TransClientIn, proto::SocketClientIn> {
public:
	TransClientIn(unsigned int cmd) : BaseTransaction(cmd) {}

	int OnRequest(proto::SocketClientIn& request) {
		if (M_CHECK_IS_TCP_CONNECTOR_FD(fd())) {
			netiolib::TcpConnectorPtr ptr = SvrNetIoHandlerSgl.GetConnectorPtr(fd());
			if (ptr) {
				ConnInfo* pinfo = (ConnInfo*)ptr->GetExtData();
				if (pinfo && pinfo->conn_type == Enum_ConnType_Router) {
					SendRegistCmd();
					RouterMgrSgl.AddRouter(pinfo->ip, pinfo->port, pinfo->serial_num,
						fd());
				}
			}
		}
		return 0;
	}

	void SendRegistCmd() {
		proto::RegisterServerReq request;
		proto::RegisterServerRsp respond;
		request.set_server_type(ConnApplicationSgl.ServerType());
		request.set_instance_id(ConnApplicationSgl.InstanceId());
		//int ret = SendMessgae(proto::CMD::CMD_REGISTER_SERVER_REQ, )
	}
};

REGISTER_TRANSACTION(CMD_SOCKET_CLIENT_IN, TransClientIn);

///////////////////////////////////////////////////////////////////////////

class TransClientOut
	: public BaseTransaction<TransClientOut, proto::SocketClientOut> {
public:
	TransClientOut(unsigned int cmd) : BaseTransaction(cmd) {}

	int OnRequest(proto::SocketClientOut& request) {
		if (M_CHECK_IS_TCP_CONNECTOR_FD(fd())) {
			netiolib::TcpConnectorPtr ptr = SvrNetIoHandlerSgl.GetConnectorPtr(fd());
			if (ptr) {
				ConnInfo* pinfo = (ConnInfo*)ptr->GetExtData();
				if (pinfo && pinfo->conn_type == Enum_ConnType_Router) {
					if (!RouterMgrSgl.ExistRouter(pinfo->ip, pinfo->port, pinfo->serial_num)) {
						RouterMgrSgl.DelRouter(pinfo->ip, pinfo->port, pinfo->serial_num, fd());
					}
					else {
						// жипB
						SvrNetIoHandlerSgl.ConnectOne(pinfo->ip, pinfo->port,
							pinfo->conn_type, pinfo->serial_num);
					}
				}
			}
		}
		return 0;
	}
};

REGISTER_TRANSACTION(CMD_SOCKET_CLIENT_OUT, TransClientOut);

///////////////////////////////////////////////////////////////////////////

class TransRegisterServer 
	: public BaseTransaction<TransRegisterServer, proto::RegisterServerReq, proto::RegisterServerRsp> {
public:
	TransRegisterServer(unsigned int cmd) : BaseTransaction(cmd) {}

	int OnRequest(proto::RegisterServerReq& request, proto::RegisterServerRsp& respond) {
		
		return 0;
	}
};

REGISTER_TRANSACTION(CMD_REGISTER_SERVER_REQ, TransRegisterServer);