#ifndef M_SVR_NET_HANDLER_INCLUDE
#define M_SVR_NET_HANDLER_INCLUDE

#include "commonlib/net_helper/net_handler.h"

class SvrNetIoHandler : public NetIoHandler {
public:
	void ConnectOne(const std::string& addr, SocketLib::s_uint16_t port, void* data);

	void ConnectOneHttp(const std::string& addr, SocketLib::s_uint16_t port, void* data);
};

#ifndef SvrNetIoHandlerSgl
#define SvrNetIoHandlerSgl base::singleton<SvrNetIoHandler>::mutable_instance()
#endif

#endif