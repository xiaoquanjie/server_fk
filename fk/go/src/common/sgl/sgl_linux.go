package sgl

import (
	"os"
	"syscall"
	"unsafe"
)

var(
	nameMutex = "Sgl_ShaMem"
	pathDeploy = "./_deploy_.tmp"
)

var file *os.File

const (
	IPC_RMID   = 0
	IPC_CREAT  = 00001000
	IPC_EXCL   = 00002000
	IPC_NOWAIT = 00004000
)

func lockSgl() (bool) {
	flag := os.O_RDWR | os.O_CREATE
	f, err := os.OpenFile(pathDeploy, flag, 0666)
	if err != nil {
		return false
	}
	if err = syscall.Flock(int(f.Fd()), syscall.LOCK_EX|syscall.LOCK_NB); err != nil {
		f.Close()
		return false
	}
	file = f
	return true
}

func unlockSgl() {
	syscall.Flock(file.Fd(), syscall.LOCK_UN)
	file.Close()
}

///////////////////////////////////////////////////////////////////////////////////////////

func lockSgl2() (uintptr, bool) {
	id, _, err := syscall.Syscall6(syscall.SYS_SHMGET, uintptr(unsafe.Pointer(&nameMutex)), 1, IPC_CREAT|IPC_EXCL, 0, 0, 0)
	if err.Error() != "errno 0" {
		return 0, false
	} else {
		go func(id uintptr) {
			chan_sig := make(chan os.Signal, 1)
			select {
			case s := <- chan_sig:
				switch s {
				case syscall.SIGQUIT:
					unlockSgl(id)
				case syscall.SIGINT:
					unlockSgl(id)
				case syscall.SIGKILL:
					unlockSgl(id)
				}
			}
		}(id)
	}
	return id, true
}

func unlockSgl2(id uintptr) {
	syscall.Syscall6(syscall.SYS_SHMCTL, id, IPC_RMID, 0, 0, 0, 0)
}