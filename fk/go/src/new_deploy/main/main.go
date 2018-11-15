package main

import (
	"common/logger"
	"flag"
	"github.com/golang/glog"
	"github.com/golang/protobuf/proto"
	"new_deploy/proto"
)

func init()  {
	flag.Parse()
}

func main() {
	deploy := protocol.Deploy{}
	deploy.ListenPort = proto.Int32(54)
	deploy.DstRootPath = proto.String("this is a test")
	logger.Info(deploy.String())
	glog.Exit()
}

