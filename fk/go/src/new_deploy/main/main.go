package main

import (
	"common/logger"
	"common/sgl"
	"flag"
	"time"
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
	time.Sleep(10000*time.Second)
	logger.Stop()
	singleton.Unlock()
}

