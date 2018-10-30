#pragma once

#include <QVariant>

class Parameter
{
public:
	Parameter(){}
	Parameter(const QVariant& value, const QString& title = QString());

	void setTitle(const QString& title);
	const QString& title() const;

	void setValue(const QVariant& value);
	const QVariant& value() const;

	bool operator==(const Parameter& other) const;
private:
	QVariant mValue;
	QString mTitle;
};

typedef QMap<QString, Parameter> Configuration;
