package sgl

type Singleton struct {
	id uintptr
}

func (s *Singleton) Lock() bool {
	id, ok := lockSgl()
	if ok {
		s.id = id
	}
	return ok
}

func (s *Singleton) Unlock() {
	if s.id != 0 {
		unlockSgl(s.id)
	}
}