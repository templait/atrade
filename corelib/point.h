#pragma once

#include <QDateTime>

class Point
{
public:
	Point(double value, const QDateTime & time);
	Point(const QDateTime & time);
	Point();

	bool isNull() const;
	bool isNaN() const;
	double value() const;
	const QDateTime& time() const;
private:
	double mValue;
	QDateTime mTime;
};
