#ifndef M_NETIO_TCP_STREAM_SOCKET_INCLUDE
#define M_NETIO_TCP_STREAM_SOCKET_INCLUDE

#include "slience/netio/config.hpp"
M_NETIO_NAMESPACE_BEGIN

template<typename T, typename SocketType>
class TcpStreamSocket : public TcpBaseSocket<T, SocketType> {
protected:
	struct _readerinfo_ {
		SocketLib::s_byte_t* readbuf;
		SocketLib::Buffer msgbuffer;
		PacketHeader curheader;

		_readerinfo_();
		~_readerinfo_();
	};

	_readerinfo_ _reader;

	void _ReadHandler(SocketLib::s_uint32_t tran_byte, SocketLib::SocketError error);

	// 裁减出数据包，返回false意味着数据包有错
	bool _CutMsgPack(SocketLib::s_byte_t* buf, SocketLib::s_uint32_t tran_byte);

	void _TryRecvData();

public:
	TcpStreamSocket(NetIo& netio);

	bool SendPacket(const SocketLib::s_byte_t* data, SocketLib::s_uint32_t len);

	template<typename MsgHeadType>
	bool SendPacket(const MsgHeadType& head, const SocketLib::s_byte_t* data,
		SocketLib::s_uint32_t len);
};

template<typename T, typename SocketType>
TcpStreamSocket<T, SocketType>::_readerinfo_::_readerinfo_() {
	g_memset(&curheader, 0, sizeof(curheader));
	readbuf = new SocketLib::s_byte_t[M_SOCKET_READ_SIZE];
	g_memset(readbuf, 0, M_SOCKET_READ_SIZE);
}

template<typename T, typename SocketType>
TcpStreamSocket<T, SocketType>::_readerinfo_::~_readerinfo_() {
	delete[]readbuf;
}

template<typename T, typename SocketType>
void TcpStreamSocket<T, SocketType>::_ReadHandler(SocketLib::s_uint32_t tran_byte, SocketLib::SocketError error) {
	do {
		// 出错关闭连接
		if (error) {
			M_NETIO_LOGGER("read handler happend error:" << M_ERROR_DESC_STR(error));
			break;
		}
		// 对方关闭写
		if (tran_byte <= 0)
			break;

		// 我方post了关闭
		if (this->_flag != E_STATE_START)
			break;

		if (_CutMsgPack(_reader.readbuf, tran_byte)) {
			_TryRecvData();
			return;
		}
		else {
			// 数据检查出错，主动断开连接
			this->_socket->Shutdown(SocketLib::E_Shutdown_RD, error);
		}
	} while (false);

	this->_PostClose();
}

template<typename T, typename SocketType>
bool TcpStreamSocket<T, SocketType>::_CutMsgPack(SocketLib::s_byte_t* buf, SocketLib::s_uint32_t tran_byte) {
	// 减少内存拷贝是此函数的设计关键
	SocketLib::s_uint32_t hdrlen = (SocketLib::s_uint32_t)sizeof(PacketHeader);
	shard_ptr_t<T> ref;
	do
	{
		/*SocketLib::s_uint32_t datalen = _reader.msgbuffer.Length() + tran_byte;
		if (datalen < hdrlen) {
			_reader.msgbuffer.Write(buf, tran_byte);
			break;
		}*/
		

		// 算出头部长度
		SocketLib::s_uint32_t datalen = _reader.msgbuffer.Length();
		if (_reader.curheader.size == 0) {
			if (tran_byte + datalen < hdrlen) {
				_reader.msgbuffer.Write(buf, tran_byte);
				return true;
			}
			else if (datalen >= hdrlen) {
				_reader.msgbuffer.Read(_reader.curheader);
			}
			else {
				_reader.msgbuffer.Write(buf, hdrlen - datalen);
				buf += (hdrlen - datalen);
				tran_byte -= (hdrlen - datalen);
				_reader.msgbuffer.Read(_reader.curheader);
			}

			// convert byte order
			_reader.curheader.n2h();

			// check
			if (_reader.curheader.size > (M_SOCKET_PACK_SIZE /*- hdrlen*/))
				return false;
		}

		// copy body data
		datalen = _reader.msgbuffer.Length();
		if (tran_byte + datalen < _reader.curheader.size) {
			_reader.msgbuffer.Write(buf, tran_byte);
			return true;
		}
		else {
			_reader.msgbuffer.Write(buf, _reader.curheader.size - datalen);
			buf += (_reader.curheader.size - datalen);
			tran_byte -= (_reader.curheader.size - datalen);

			// notify
			_reader.curheader.size = 0;
			if (!ref)
				ref = this->shared_from_this();
			this->_netio.OnReceiveData(ref, _reader.msgbuffer);
			_reader.msgbuffer.Clear();
		}
	} while (true);
	return true;
}

template<typename T, typename SocketType>
void TcpStreamSocket<T, SocketType>::_TryRecvData() {
	SocketLib::SocketError error;
	this->_socket->AsyncRecvSome(m_bind_t(&TcpStreamSocket::_ReadHandler, this->shared_from_this(), placeholder_1, placeholder_2)
		, _reader.readbuf, M_SOCKET_READ_SIZE, error);
	if (error)
		this->_PostClose();
}

template<typename T, typename SocketType>
TcpStreamSocket<T, SocketType>::TcpStreamSocket(NetIo& netio)
	:TcpBaseSocket<T, SocketType>(netio) {
}

template<typename T, typename SocketType>
bool TcpStreamSocket<T, SocketType>::SendPacket(const SocketLib::s_byte_t* data, 
	SocketLib::s_uint32_t len) {
	SocketLib::ScopedLock scoped_w(_writer.lock);
	if (!_CheckCanSend(len + sizeof(PacketHeader))) {
		return false;
	}

	PacketHeader hdr;
	hdr.size = len;
	hdr.timestamp = 0xFCFCFCFC;
	hdr.h2n();
	_writer.msgbuffer2.Write(hdr);
	_writer.msgbuffer2.Write((void*)data, len);
	_TrySendData();
	return true;
}

template<typename T, typename SocketType>
template<typename MsgHeadType>
bool TcpStreamSocket<T, SocketType>::SendPacket(const MsgHeadType& head, 
	const SocketLib::s_byte_t* data, SocketLib::s_uint32_t len) {
	SocketLib::ScopedLock scoped_w(_writer.lock);
	if (!_CheckCanSend(len + sizeof(PacketHeader) + sizeof(MsgHeadType))) {
		return false;
	}

	PacketHeader hdr;
	hdr.size = len + sizeof(MsgHeadType);
	hdr.timestamp = 0xFCFCFCFC;
	hdr.h2n();
	_writer.msgbuffer2.Write(hdr);
	_writer.msgbuffer2.Write(head);
	_writer.msgbuffer2.Write((void*)data, len);
	_TrySendData();
	return true;
}

M_NETIO_NAMESPACE_END
#endif