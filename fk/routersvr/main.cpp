#include "routersvr/router_svr.h"
#include <iostream>
#include "protolib/src/routersvr_config.pb.h"
using namespace std;

#include "tinyxml/tinyxml.h"

int main(int argc, char* argv[]) {

	TiXmlDocument doc;
	TiXmlElement * element = new TiXmlElement("Hello");
	doc.LinkEndChild(element);
	doc.Print();
	return 0;

	RouterAppSgl::mutable_instance().Init(argc, argv);
	RouterAppSgl::mutable_instance().Run();
	return 0;
}