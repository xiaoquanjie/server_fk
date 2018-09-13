#include "commonlib/net_helper/svr_net_handler.h"

void SvrNetIoHandler::ConnectOne(const std::string& addr, SocketLib::s_uint16_t port, void* data) {
	SocketLib::SocketError error;
	netiolib::TcpConnectorPtr connector(new netiolib::TcpConnector(*this));
	connector->SetExtData(data);
	SocketLib::Tcp::EndPoint ep(SocketLib::AddressV4(addr), port);
	connector->AsyncConnect(ep, error);
	connector.reset();
}

void SvrNetIoHandler::ConnectOneHttp(const std::string& addr, SocketLib::s_uint16_t port, void* data) {
	try {
		SocketLib::Tcp::EndPoint ep(SocketLib::AddressV4(addr), port);
		netiolib::HttpConnectorPtr connector(new netiolib::HttpConnector(*this));
		connector->SetExtData(data);
		connector->AsyncConnect(ep);
		connector.reset();
	}
	catch (...) {
	}
}