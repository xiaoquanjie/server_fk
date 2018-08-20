#include "slience/base/object_pool.h"

M_BASE_NAMESPACE_BEGIN

template<typename T, typename LockMode>
ObjectPoolInfo<T, LockMode>	ObjectPool<T, LockMode>::_info;

M_BASE_NAMESPACE_END