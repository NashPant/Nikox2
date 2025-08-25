#include "CacheMemory.h"

namespace TP
{
	template <typename T>
	std::shared_ptr<const T> CacheMemory<T>::null_ = nullptr;

	/*template <typename T>
	TPSmartPointer<typename CacheMemory<T>::cachememorymap_t> CacheMemory<T>::myCacheMap = nullptr;*/

}