#pragma once
#include "stdafx.h"

template <class T>
class DynArray
{
public:
	const dword DEFAULT_SIZE = 1024;
	DynArray(float growRatio);
	~DynArray();
	T &operator[] (dword index);
private:
	dword m_dwSize;
	T* m_pData;
	float m_flGrowRatio;
	bool Grow();
};

template <class T>
DynArray<T>::DynArray(float growRatio) : m_pData(nullptr), m_dwSize(DEFAULT_SIZE), m_flGrowRatio(growRatio)
{
	this->m_pData = new T[this->m_dwSize]();
}

template <class T>
DynArray<T>::~DynArray()
{
	delete[] this->m_pData;
}

template <class T>
bool DynArray<T>::Grow()
{
	dword newSize = (dword)(this->m_dwSize * this->m_flGrowRatio);
	T* newData = nullptr;
	try
	{
		newData = new T[newSize]();
	}
	catch (std::bad_alloc &ba)
	{
#ifdef DEBUG
		std::cout << "[DynArray] CRITICAL: Failed to allocate " << newSize << " bytes" << std::endl;
#endif
		return false;
	}
	
	errno_t error = memcpy_s(newData, newSize * sizeof(T), this->m_pData, this->m_dwSize * sizeof(T));
	
	if (error != 0)
	{
		std::cout << "[DynArray] CRITICAL: Failed to copy memory from " <<
			std::hex << std::setfill('0') << std::setw(8) << (dword)this->m_pData <<
			" (" << std::dec << std::setw(0) << (this->m_dwSize + sizeof(T)) << "bytes) to " <<
			std::hex << std::setfill('0') << std::setw(8) << (dword)newData <<
			" (" << std::dec << std::setw(0) << (newSize + sizeof(T)) << "bytes), error-code: " << error << std::endl;
		delete[] newData;
		return false;
	}

#ifdef DEBUG
	std::cout << "[DynArray] Grown to " << (newSize * sizeof(T)) << " bytes" << std::endl;
#endif

	delete[] m_pData;
	this->m_pData = newData;
	this->m_dwSize = newSize;
	return true;
}

template <class T>
T& DynArray<T>::operator[] (dword index)
{
	while (index > this->m_dwSize - 1)
	{
		if (!this->Grow())
			return this->m_pData[this->m_dwSize - 1];
	}
	return this->m_pData[index];
}