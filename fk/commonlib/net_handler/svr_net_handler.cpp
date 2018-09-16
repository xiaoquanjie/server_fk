#include "commonlib/net_handler/svr_net_handler.h"
#include "commonlib/net_handler/router_mgr.h"

ConnInfo* MakeConnInfo(const std::string& addr, SocketLib::s_uint16_t port,
	int conn_type, int serial_num) {
	ConnInfo* pinfo = (ConnInfo*)malloc(sizeof(ConnInfo));
	pinfo->conn_type = conn_type;
	pinfo->serial_num = serial_num;
	pinfo->port = port;
	memcpy(pinfo->ip, addr.c_str(), addr.length());
	return pinfo;
}

bool SvrNetIoHandler::ConnectOne(const std::string& addr, SocketLib::s_uint16_t port, 
	int conn_type, int serial_num) {
	ConnInfo* pinfo = MakeConnInfo(addr, port, conn_type, serial_num);
	SocketLib::SocketError error;
	netiolib::TcpConnectorPtr connector(new netiolib::TcpConnector(*this));
	connector->SetExtData(pinfo);
	SocketLib::Tcp::EndPoint ep(SocketLib::AddressV4(addr), port);
	connector->AsyncConnect(ep, error);
	return true;
}

void SvrNetIoHandler::ConnectOneHttp(const std::string& addr, SocketLib::s_uint16_t port, 
	int conn_type, int serial_num) {
	try {
		ConnInfo* pinfo = MakeConnInfo(addr, port, conn_type, serial_num);
		SocketLib::Tcp::EndPoint ep(SocketLib::AddressV4(addr), port);
		netiolib::HttpConnectorPtr connector(new netiolib::HttpConnector(*this));
		connector->SetExtData(pinfo);
		connector->AsyncConnect(ep);
		connector.reset();
	}
	catch (...) {
	}
}

void SvrNetIoHandler::OnConnection(netiolib::TcpConnectorPtr& clisock, SocketLib::SocketError error) {
	if (!error) {
		// Ä¬ÕJÊÇÖØßB
		ConnInfo* pinfo = (ConnInfo*)clisock->GetExtData();
		ConnInfo* new_info = MakeConnInfo(pinfo->ip, pinfo->port, pinfo->conn_type, pinfo->serial_num);
		_ConnectOne(pinfo);
	}
	else {
		NetIoHandler::OnConnection(clisock, error);
	}
}

void SvrNetIoHandler::_ConnectOne(ConnInfo* info) {
	SocketLib::SocketError error;
	netiolib::TcpConnectorPtr connector(new netiolib::TcpConnector(*this));
	connector->SetExtData(info);
	SocketLib::Tcp::EndPoint ep(SocketLib::AddressV4(info->ip), info->port);
	connector->AsyncConnect(ep, error);
}