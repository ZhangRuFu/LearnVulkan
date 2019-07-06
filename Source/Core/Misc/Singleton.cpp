#include "Core\Misc\Singleton.h"

template<class T> 
T* Singleton<T>::m_instance = nullptr;