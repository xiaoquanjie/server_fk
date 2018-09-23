#ifndef M_NETIO_SYNC_CONNECTOR_INCLUDE
#define M_NETIO_SYNC_CONNECTOR_INCLUDE

#include "slience/netio/config.hpp"
M_NETIO_NAMESPACE_BEGIN

// 同步connector
class SyncConnector {
public:
	SyncConnector();

	~SyncConnector();

	bool Connect(const SocketLib::Tcp::EndPoint& ep, SocketLib::s_uint32_t timeo_sec = -1);

	bool Connect(const std::string& addr, SocketLib::s_uint16_t port, SocketLib::s_uint32_t timeo_sec = -1);

	const SocketLib::Tcp::EndPoint& LocalEndpoint()const;

	const SocketLib::Tcp::EndPoint& RemoteEndpoint()const;

	// 调用这个函数不意味着连接立即断开，会等所有的未处理的数据处理完就会断开
	void Close();

	bool Send(const SocketLib::s_byte_t* data, SocketLib::s_uint32_t len);

	bool IsConnected()const;

	SocketLib::Buffer* Recv();

	void SetTimeOut(SocketLib::s_uint32_t timeo);

protected:
	SocketLib::s_uint32_t _LocalEndian()const;

	SocketLib::Buffer* _CutMsgPack(SocketLib::s_byte_t* buf, SocketLib::s_uint32_t& tran_byte);

protected:
	SocketLib::IoService _ioservice;
	SocketLib::TcpConnector<SocketLib::IoService>* _socket;

	// endpoint
	SocketLib::Tcp::EndPoint _localep;
	SocketLib::Tcp::EndPoint _remoteep;

	PacketHeader _curheader;
	SocketLib::s_byte_t*  _readbuf;
	SocketLib::s_uint32_t _readsize;

	SocketLib::Buffer _sndbuffer;
	SocketLib::Buffer _rcvbuffer;
	// 状态标志
	unsigned short _flag;
};


M_NETIO_NAMESPACE_END
#endif