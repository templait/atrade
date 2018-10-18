#pragma once

#include <types.h>
#include <iterator>

template<class T>
class Serial
{
	class const_iterator : public std::iterator<std::random_access_iterator_tag, T>
	{
		friend class Serial<T>;
		typedef typename std::iterator<std::random_access_iterator_tag, T>::difference_type DifT;
	public:
		inline bool operator==(const const_iterator& other) const	{return mPos==other.mPos && mSerial==other.mSerial;}
		inline bool operator!=(const const_iterator& other) const	{return ! operator == (other);}
		inline bool operator<(const const_iterator& other) const	{return mPos<other.mPos;}
		inline bool operator>(const const_iterator& other) const	{return mPos>other.mPos;}
		inline bool operator<=(const const_iterator& other) const	{return mPos<=other.mPos;}
		inline bool operator>=(const const_iterator& other) const	{return mPos>=other.mPos;}
		inline const_iterator& operator++()							{mPos++; return *this;}
		inline const_iterator& operator--()							{mPos--; return *this;}
		inline const_iterator operator-(DifT dif) const				{return const_iterator(mSerial, mPos-dif);}
		inline const_iterator operator+(DifT dif) const				{return const_iterator(mSerial, mPos+dif);}
		inline const T& operator*() const							{return *mSerial->at(mPos);}
		inline const T* operator->() const							{return mSerial->at(mPos);}
		inline DifT operator-(const const_iterator& other) const	{return mPos-other.mPos;}
		inline const_iterator& operator+=(DifT val)					{mPos+=val; return *this;}

	private:
		const_iterator(const Serial<T> * serial, int pos) : mSerial(serial), mPos(pos){}

		const Serial<T> *mSerial;
		int mPos;
	};

public:
	virtual int size() const = 0;
	virtual const T * at(int index) const = 0; //!< временная метка каждой последующей свечки должна быть больше предыдущей.

	const T& operator[](int index) const;
	const T& first() const;
	const T& last() const;
	const_iterator begin() const;
	const_iterator end() const;
	QList<const T *> getTimeRange(const TimeRange &range) const;

	virtual ~Serial(){}
};

template<class T> inline
const T &Serial<T>::operator[](int index) const
{
	return *at(index);
}

template<class T> inline
const T &Serial<T>::first() const
{
	return *at(0);
}

template<class T>
const T &Serial<T>::last() const
{
	return *at(size()-1);
}

template<class T> inline
typename Serial<T>::const_iterator Serial<T>::begin() const
{
	return const_iterator(this, 0);
}

template<class T> inline
typename Serial<T>::const_iterator Serial<T>::end() const
{
	return const_iterator(this, size());
}

template<class T>
QList<const T *> Serial<T>::getTimeRange(const TimeRange &range) const
{
	QList<const T*> rv;

	const_iterator end_it   = std::upper_bound(begin(), end(), range.second,[](const QDateTime& t, const T& val){return t<val.time();});
	const_iterator start_it = std::lower_bound(begin(), end(), range.first, [](const T& val, const QDateTime& t){return val.time()<t;});

	while(start_it < end_it)
	{
		rv << start_it.operator->();
		++start_it;
	}

	return rv;
}
