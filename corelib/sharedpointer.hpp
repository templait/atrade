#pragma once

#include <QMap>

template<class T>
class SharedPointer
{
public:
	typedef T BaseType;
	typedef std::function<void (int)> DeleteCallback;
	SharedPointer();
	SharedPointer(T* pointer,  DeleteCallback onDelete);
	SharedPointer(const SharedPointer & other);
	~SharedPointer();

	bool isNull() const;

	const T* operator->() const;
	T* operator->();

	const T* operator&() const;
	T* operator&();

	bool operator==(const SharedPointer& other) const;
	SharedPointer& operator=(const SharedPointer& other);
private:
	void increment();
	void decrement();
	T* mPointer;
	DeleteCallback mOnDelete;
	static QMap<T*,uint> mCounter;
};

template<class T>
QMap<T*,uint>  SharedPointer<T>::mCounter;

template<class T>
SharedPointer<T>::SharedPointer() : mPointer(0), mOnDelete([](int){}){}

template<class T>
SharedPointer<T>::SharedPointer(T *pointer,  DeleteCallback onDelete) :mPointer(pointer), mOnDelete(onDelete)
{
	mCounter[mPointer]=1;
}

template<class T>
SharedPointer<T>::SharedPointer(const SharedPointer &other) : mPointer(other.mPointer), mOnDelete(other.mOnDelete)
{
	increment();
}

template<class T>
inline const T *SharedPointer<T>::operator->() const
{
	return mPointer;
}

template<class T>
inline T *SharedPointer<T>::operator->()
{
	return mPointer;
}

template<class T>
inline const T *SharedPointer<T>::operator&() const
{
	return mPointer;
}

template<class T>
inline T *SharedPointer<T>::operator&()
{
	return mPointer;
}

template<class T>
bool SharedPointer<T>::operator==(const SharedPointer &other) const
{
	return mPointer == other.mPointer;
}

template<class T>
inline SharedPointer<T> &SharedPointer<T>::operator=(const SharedPointer<T> &other)
{
	decrement();
	mPointer = other.mPointer;
	mOnDelete = other.mOnDelete;
	increment();
	return *this;
}

template<class T>
inline void SharedPointer<T>::increment()
{
	if(!isNull())
	{	mCounter[mPointer]++;	}
}

template<class T>
inline void SharedPointer<T>::decrement()
{
	if(!isNull())
	{	mCounter[mPointer]--;	}
}

template<class T>
SharedPointer<T>::~SharedPointer()
{
	decrement();
	if(mCounter[mPointer]==0)
	{
		mCounter.remove(mPointer);
		delete mPointer;
	}
	mOnDelete(mCounter[mPointer]);
}

template<class T>
inline bool SharedPointer<T>::isNull() const
{
	return mPointer==0;
}

template<class T>
uint qHash(const SharedPointer<T> &pt, uint)
{
	return reinterpret_cast<std::uintptr_t>(&pt);
}
