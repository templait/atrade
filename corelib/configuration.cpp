#include "configuration.h"

Parameter::Parameter(const QVariant &value, const QString &title)
	: mValue(value)
	, mTitle(title)
{

}

void Parameter::setTitle(const QString &title)
{
	mTitle = title;
}

const QString &Parameter::title() const
{
	return mTitle;
}

void Parameter::setValue(const QVariant &value)
{
	mValue = value;
}

const QVariant &Parameter::value() const
{
	return mValue;
}

bool Parameter::operator==(const Parameter &other) const
{
	return mTitle==other.mTitle && mValue==other.mValue;
}
