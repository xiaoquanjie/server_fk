start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ routersvr_config.proto
start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ cmd.proto
start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ svr_base.proto
start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ router.proto
start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ comm_conf.proto
start ../bin/protoc.exe  --proto_path=../protolib/proto --cpp_out=../protolib/src/ connsvr_config.proto