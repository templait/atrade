#include "log.h"

Log::LogFunction Log::mFunction = [](const QString&, int){};
QMap<Log::ELevel, QColor> Log::mColors = QMap<Log::ELevel, QColor>();

void Log::setLogFunction(Log::LogFunction function)
{
	mFunction = function;
}

void Log::setColor(const QColor &color, ELevel level)
{
	mColors[level] = color;
}

const QColor Log::color(ELevel level)
{
	QColor rv;
	auto i = mColors.find(level);
	if(i != mColors.end())
	{	rv = *i;	}
	return rv;
}

void Log::info(const QString &string)
{
	mFunction(string, Info);
}

void Log::warning(const QString &string)
{
	mFunction(string, Warning);
}

void Log::error(const QString &string)
{
	mFunction(string, Error);
}
