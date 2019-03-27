#include "test/test.h"

int main(int argc, char* argv[]) {
	TestAppSgl::mutable_instance().Init(argc, argv);
	TestAppSgl::mutable_instance().Run();
	return 0;
}