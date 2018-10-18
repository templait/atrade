#include "tools.h"

qint64 secsInInterval(ETimeInterval interval)
{
	qint64 rv = -1;
	switch(interval)
	{
	case IntervalM1:
		rv = 60;
		break;
	case IntervalM5:
		rv = 60*5;
		break;
	case IntervalM10:
		rv = 60*10;
		break;
	case IntervalM15:
		rv = 60*15;
		break;
	case IntervalM20:
		rv = 60*20;
		break;
	case IntervalM30:
		rv = 60*30;
		break;
	case IntervalH1:
		rv = 60*60;
		break;
	case IntervalH2:
		rv = 60*60*2;
		break;
	case IntervalH4:
		rv = 60*60*4;
		break;
	case IntervalD1:
		rv = 60*60*24;
		break;
	case IntervalW1:
		rv = 60*60*24*7;
		break;
	case IntervalMN1:
		rv = 60*60*24*DAYS_IN_YEAR/12;
		break;
	default:
		rv = -2;
	}
	return rv;
}
