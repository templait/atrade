#include "bdatasource.h"

class BDataSource::const_iterator : public std::iterator<std::random_access_iterator_tag, Candle>
{
	friend class BDataSource;
public:
	inline bool operator==(const const_iterator& other) const	{return mPos==other.mPos;}
	inline bool operator<(const const_iterator& other) const	{return mPos<other.mPos;}
	inline bool operator>(const const_iterator& other) const	{return mPos>other.mPos;}
	inline bool operator<=(const const_iterator& other) const	{return mPos<=other.mPos;}
	inline bool operator>=(const const_iterator& other) const	{return mPos>=other.mPos;}
	inline const_iterator& operator++() {mPos++; return *this;}
	inline const_iterator& operator--() {mPos--; return *this;}
	inline const Candle& operator*() const	{return *mDataSource->at(mPos);}
	inline const Candle* operator->() const	{return mDataSource->at(mPos);}
	inline difference_type operator-(const const_iterator& other) const	{return mPos-other.mPos;}
	inline const_iterator& operator+=(difference_type val){mPos+=val; return *this;}

private:
	const_iterator(const BDataSource * dataSource, int pos) : mDataSource(dataSource), mPos(pos){}

	const BDataSource *mDataSource;
	int mPos;
};

BDataSource::BDataSource(QObject *parent) : QObject(parent)
{

}

const Candle &BDataSource::operator[](int index) const
{
	return *at(index);
}

const Candle &BDataSource::first() const
{
	return *at(0);
}

const Candle &BDataSource::last() const
{
	return *at(size()-1);
}

BDataSource::const_iterator BDataSource::begin() const
{
	return const_iterator(this, 0);
}

BDataSource::const_iterator BDataSource::end() const
{
	return const_iterator(this, size());
}

QList<const Candle*> BDataSource::getTimeRange(const TimeRange& range) const
{
	QList<const Candle*> rv;

	const_iterator end_it   = std::upper_bound(begin(), end(), range.second,[](const QDateTime& t, const Candle& candle){return t<candle.time();});
	const_iterator start_it = std::lower_bound(begin(), end(), range.first, [](const Candle& candle, const QDateTime& t){return candle.time()<t;});

	while(start_it < end_it)
	{
		rv << start_it.operator->();
		++start_it;
	}

	return rv;
}

