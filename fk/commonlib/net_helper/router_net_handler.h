#ifndef M_ROUTER_NET_HANDLER_INCLUDE
#define M_ROUTER_NET_HANDLER_INCLUDE

#include "commonlib/net_helper/net_handler.h"

class RouterNetHandler{
public:
	static int Init(base::timestamp& now, NetIoHandler::callback_type callback, int svr_thread_cnt);


	
};

#endif