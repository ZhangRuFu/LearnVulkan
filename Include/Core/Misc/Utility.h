#pragma once
template<class T>
inline T* Stride(T* p, size_t offset)
{
	return reinterpret_cast<T*>((char*)p + offset);
}