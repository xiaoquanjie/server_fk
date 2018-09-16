#ifndef M_SVR_NET_HANDLER_INCLUDE
#define M_SVR_NET_HANDLER_INCLUDE

#include "commonlib/net_handler/net_handler.h"

enum ConnType {
	Enum_ConnType_Router = 1,
};

struct ConnInfo {
	int conn_type;
	int serial_num;
	int port;
	char ip[65];
};

class SvrNetIoHandler : public NetIoHandler {
public:
	bool ConnectOne(const std::string& addr, SocketLib::s_uint16_t port, int conn_type, int serial_num);

	void ConnectOneHttp(const std::string& addr, SocketLib::s_uint16_t port, int conn_type, int serial_num);

	void OnConnection(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) override;

private:
	void _ConnectOne(ConnInfo* info);
};

#ifndef SvrNetIoHandlerSgl
#define SvrNetIoHandlerSgl base::singleton<SvrNetIoHandler>::mutable_instance()
#endif

#endif