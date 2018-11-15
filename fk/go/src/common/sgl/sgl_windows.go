package sgl

import (
	"syscall"
	"unsafe"
)

var(
	nameMutex = "Sgl_ShaMem"
	kernel = syscall.NewLazyDLL("kernel32.dll")
)

func lockSgl() (uintptr, bool) {
	id, _, err := kernel.NewProc("CreateMutexA").Call(0, 1, uintptr(unsafe.Pointer(&nameMutex)))
	if err.Error() != "The operation completed successfully." {
		return 0, false
	}
	return id, true
}

func unlockSgl(id uintptr) {
	syscall.CloseHandle(syscall.Handle(id))
}