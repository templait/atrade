#include "timeintervalcombobox.h"
#include <tools.h>

TimeIntervalComboBox::TimeIntervalComboBox(QWidget *parent) : QComboBox(parent)
{
	for(int i = IntervalM1; i!=IntervalUnknown; i++)
	{	addItem(intervalToString(static_cast<ETimeInterval>(i)), i);	}
}

void TimeIntervalComboBox::setTimeInterval(ETimeInterval timeInterval)
{
	for(int i=0; i<count(); i++)
	{
		if(itemData(i) == timeInterval)
		{
			setCurrentIndex(i);
			break;
		}
	}
}

ETimeInterval TimeIntervalComboBox::timeInterval() const
{
	return static_cast<ETimeInterval>(currentData().toInt());
}
