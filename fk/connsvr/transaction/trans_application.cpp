#include "protolib/src/svr_base.pb.h"
#include "protolib/src/cmd.pb.h"
#include "commonlib/transaction/base_transaction.h"
#include "commonlib/transaction/transaction_mgr.h"
#include "commonlib/net_helper/router_mgr.h"

class TransClientIn 
	: public BaseTransaction<TransClientIn, proto::SocketClientIn> {
public:
	TransClientIn(unsigned int cmd) : BaseTransaction(cmd) {}

	int OnRequest(proto::SocketClientIn& request) {
		return 0;
	}
};

REGISTER_TRANSACTION(CMD_SOCKET_CLIENT_IN, TransClientIn);

///////////////////////////////////////////////////////////////////////////

class TransClientOut
	: public BaseTransaction<TransClientOut, proto::SocketClientOut> {
public:
	TransClientOut(unsigned int cmd) : BaseTransaction(cmd) {}

	int OnRequest(proto::SocketClientOut& request) {
		//SeverInstanceMgrSgl.DelInstance(fd());
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