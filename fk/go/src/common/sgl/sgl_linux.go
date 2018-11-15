package sgl

import (
	"fmt"
	"syscall"
)

var(
	nameMutex = "Sgl_ShaMem"
)

const (
	IPC_RMID   = 0
	IPC_CREAT  = 00001000
	IPC_EXCL   = 00002000
	IPC_NOWAIT = 00004000
)

func lockSgl() (uintptr, bool) {
	id, _, err := syscall.Syscall6(syscall.SYS_SHMGET, uintptr(unsafe.Pointer(&nameMutex)), 1, IPC_CREAT|IPC_EXCL, 0, 0, 0)
	if err != nil {
		fmt.Println(err)
		return 0, false
	}
	return id, true
}

func unlockSgl(id uintptr) {
	syscall.Syscall6(syscall.SYS_SHMCTL, id, IPC_RMID, 0, 0, 0, 0)
}