package main

import (
	"common/logger"
	"common/sgl"
	"flag"
)

func init()  {
	flag.Parse()
}

func main() {
	singleton := sgl.Singleton{}
	if !singleton.Lock() {
		logger.Fatal("depoly has already runing")
	} else {
		logger.Info("......")
	}
	logger.Stop()
	singleton.Unlock()
}

