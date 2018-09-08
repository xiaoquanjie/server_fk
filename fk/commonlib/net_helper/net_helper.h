#ifndef M_NET_HELPER_INCLUDE
#define M_NET_HELPER_INCLUDE

#include "commonlib/svr_base/svrbase.h"
#include "google/protobuf/message.h"
#include <unordered_map>
#include "slience/base/compatibility.hpp"

// 最大消息队列
#ifndef M_MAX_MESSAGE_LIST
#define M_MAX_MESSAGE_LIST (5000)
#endif

// expire检查间隔
#ifndef M_EXPIRE_CHECK_INTERVAL
#define M_EXPIRE_CHECK_INTERVAL (15)
#endif

// expire时长
#ifndef M_EXPIRE_INTERVAL
#define M_EXPIRE_INTERVAL (30)
#endif

class NetHandler : public netiolib::NetIo {
public:
	typedef m_function_t<int(base::s_int64_t fd, const AppHeadFrame& frame, const char* data, base::s_uint32_t data_len)>
		callback_type;

	NetHandler(base::timestamp& now, callback_type callback);

	int Update();

	void OnTick();

	const base::timestamp& GetNow()const;

	void SendDataByFd(base::s_int64_t fd, const char* data, base::s_int32_t len);

	void SendDataByInstId(int instid, const char* data, base::s_int32_t len);

	void SendData(const AppHeadFrame& frame, const google::protobuf::Message& message);

	void RegisterServer(int server_type, int instance_id, base::s_int64_t fd);

	virtual void OnConnection(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error);

	virtual void OnConnection(netiolib::TcpSocketPtr& clisock);

	virtual void OnDisConnection(netiolib::TcpConnectorPtr& clisock);

	virtual void OnDisConnection(netiolib::TcpSocketPtr& clisock);

	virtual void CheckTcpSocketExpire();

protected:
	void OnConnected(netiolib::TcpSocketPtr& clisock) override;

	void OnConnected(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) override;

	void OnDisconnected(netiolib::TcpSocketPtr& clisock) override;

	void OnDisconnected(netiolib::TcpConnectorPtr& clisock) override;

	void OnReceiveData(netiolib::TcpSocketPtr& clisock, SocketLib::Buffer& buffer) override;

	void OnReceiveData(netiolib::TcpConnectorPtr& clisock, SocketLib::Buffer& buffer) override;

protected:
	void OnConnected(netiolib::HttpSocketPtr& clisock) override {

	}

	void OnConnected(netiolib::HttpConnectorPtr& clisock, SocketLib::SocketError error) override {

	}

	void OnDisconnected(netiolib::HttpSocketPtr& clisock) override {

	}

	void OnDisconnected(netiolib::HttpConnectorPtr& clisock) override {

	}

	void OnReceiveData(netiolib::HttpSocketPtr& clisock, netiolib::HttpSvrRecvMsg& httpmsg) override {

	}
	
	void OnReceiveData(netiolib::HttpConnectorPtr& clisock, netiolib::HttpCliRecvMsg& httpmsg) override {

	}

protected:
	int _msg_cache_size;
	base::timestamp& _now;

	// callback
	callback_type _callback;

	// send buff
	base::Buffer _snd_buff;

	// message list
	base::MutexLock _msg_lock;
	base::slist<TcpSocketMsg*> _tcp_socket_msg_list;
	base::slist<TcpSocketMsg*> _tcp_socket_msg_list2;
	base::slist<TcpConnectorMsg*> _tcp_connector_msg_list;
	base::slist<TcpConnectorMsg*> _tcp_connector_msg_list2;

	// socket container
	TcpSocketContextContainer _tcp_socket_container;
	TcpConnectorContextContainer _tcp_connector_container;

	// instance id map
	std::unordered_map<int, base::s_int64_t> _instid_fd_map;
};

/////////////////////////////////////////////////////////////////////////////////////////

class NetHelper {
public:
	static int Init(base::timestamp& now, NetHandler::callback_type callback, int svr_thread_cnt);

	static void Stop();

	static int Update();

	static void OnTick();

	static bool ListenOne(const std::string& addr, SocketLib::s_uint16_t port);

	static bool ListenOneHttp(const std::string& addr, SocketLib::s_uint16_t port);

	static void ConnectOne(const std::string& addr, SocketLib::s_uint16_t port, void* data);

	static void ConnectOneHttp(const std::string& addr, SocketLib::s_uint16_t port, void* data);

	static SocketLib::SocketError GetLastError();

	static void RegisterServer(int server_type, int instance_id, base::s_int64_t fd);

	static void SendDataByInstId(int instid, const char* data, base::s_int32_t len);

	static void SendData(const AppHeadFrame& frame, const google::protobuf::Message& message);

protected:
	static NetHandler& GetNetHandler();

	static void SetNetHandler(NetHandler* handler);

private:
	static NetHandler* _net_handler;
};

#endif