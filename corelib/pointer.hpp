#pragma once

#include <QMap>

template<class T>
class Pointer
{
public:
	Pointer(T* pointer);
	Pointer(const Pointer & other);
	~Pointer();
private:
	T* mPointer;
	static QMap<T*,uint> mCounter;
};

template<class T>
QMap<T*,uint>  Pointer<T>::mCounter;

template<class T>
Pointer<T>::Pointer(T *pointer) :mPointer(pointer)
{
	mCounter[mPointer]=1;
}

template<class T>
Pointer<T>::Pointer(const Pointer &other) : mPointer(other.mPointer)
{
	mCounter[mPointer]++;
}

template<class T>
Pointer<T>::~Pointer()
{
	mCounter[mPointer]--;
	if(mCounter[mPointer]==0)
	{
		delete mPointer;
	}
	//qDebug() << mCounter[mPointer];
}
