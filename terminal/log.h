#pragma once

#include <QColor>
#include <QMap>
#include <functional>

class QString;

class Log
{
public:
	enum ELevel  {Info, Warning, Error};
	typedef std::function<void(const QString&, ELevel)> LogFunction;

	static void setLogFunction(LogFunction function);
	static void setColor(const QColor& color, ELevel level);
	static const QColor color(ELevel level);

	static void info(const QString& string);
	static void warning(const QString& string);
	static void error(const QString& string);
private:
static LogFunction mFunction;
static QMap<ELevel, QColor> mColors;
};
