#!/bin/sh

./protoc  --proto_path=../protolib/proto --cpp_out=../protolib/src/ routersvr_config.proto
./protoc  --proto_path=../protolib/proto --cpp_out=../protolib/src/ cmd.proto
./protoc  --proto_path=../protolib/proto --cpp_out=../protolib/src/ comm_conf.proto
./protoc  --proto_path=../protolib/proto --cpp_out=../protolib/src/ router.proto
./protoc  --proto_path=../protolib/proto --cpp_out=../protolib/src/ svr_base.proto
