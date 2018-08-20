#ifndef M_BASE_OBJECT_POOL_INCLUDE
#define M_BASE_OBJECT_POOL_INCLUDE

#include "slience/base/config.hpp"
#include "slience/base/mutexlock.hpp"

M_BASE_NAMESPACE_BEGIN

template<typename T, typename LockMode = base::FakeLock>
class ObjectPool {
public:
	ObjectPool() {
		_numalloc = 0;
		_elemsize = (sizeof(T) > sizeof(T*)) ? sizeof(T) : sizeof(T*);
		_listhead = NULL;
	}

	~ObjectPool() {
		while (_listhead) {
			T* ret = _listhead;
			_listhead = *(reinterpret_cast<T**>(_listhead));
			::free(ret);
		}
	}

	int GetCount()const {
		return _numalloc;
	}

	void SnapShot(int lv, unsigned int verbose)const;

	T *Alloc() {
		T* ret = _alloc();
		return new(ret)T();
	}

	template<class P>
	T *Alloc(const P& p) {
		T* ret = _alloc();
		return new(ret)T(p);
	}

	template<class P1, class P2>
	T *Alloc(const P1& p1, const P2& p2) {
		T* ret = _alloc();
		return new(ret)T(p1, p2);
	}

	template<class P1, class P2, class P3>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3);
	}

	template<class P1, class P2, class P3, class P4>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4);
	}

	template<class P1, class P2, class P3, class P4, class P5>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10, const P11& p11) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}

	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11, class P12>
	T *Alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10, const P11& p11, const P12& p12) {
		T* ret = _alloc();
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}

	void Dealloc(T* elem) {
		base::ScopedLock lock(_lock);
		elem->~T();
		memset(elem, 0xfe, _elemsize);
		_numalloc--;
		*(reinterpret_cast<T**>(elem)) = _listhead;
		_listhead = elem;
	}

protected:
	T * _alloc() {
		base::ScopedLock lock(_lock);
		T* ret = 0;
		_numalloc++;
		if (_listhead == NULL) {
			ret = (T*)malloc(_elemsize);
		}
		else {
			ret = _listhead;
			_listhead = *(reinterpret_cast<T**>(_listhead));
		}
		memset(ret, 0xfe, _elemsize);
		return ret;
	}

protected:
	int		  _numalloc; ///< number of elements currently allocated through this ClassPool
	size_t    _elemsize; ///< the size of each element, or the size of a pointer, whichever is greater
	T *	      _listhead; ///< a pointer to a linked list of freed elements for reuse
	LockMode  _lock;
};

M_BASE_NAMESPACE_END
#endif