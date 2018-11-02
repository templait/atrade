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

//typedef QMap<QString, Parameter> Configuration;

class Configuration
{
public:
	Configuration();
	Configuration(const Configuration &other);
	Configuration(std::initializer_list<Configuration> list);
	Configuration(const QString& name, const QVariant& value, const QString& title = QString());

	Configuration operator[](const QString& childName) const;
	Configuration& operator[](const QString& childName);
	bool operator==(const Configuration& other) const;
	Configuration& operator=(const Configuration& other);
	friend QDataStream& operator<<(QDataStream& out, const Configuration& configuration);
	friend QDataStream& operator>>(QDataStream &in, Configuration &configuration);

	const QString& name() const;
	void setName(const QString& name);
	const QVariant &value() const;
	void setValue(const QVariant &value);
	const QString& title() const;
	void setTitle(const QString& title);

	bool appendChild(Configuration child);
	Configuration *childAt(int index);
	int childrenCount() const;

	Configuration * parent();
private:
	QString mName;
	QVariant mValue;
	QString mTitle;
	QMap<QString, Configuration> mChildren;
	Configuration *mParent;
};

QDataStream& operator<<(QDataStream& out, const Configuration& configuration);
QDataStream& operator>>(QDataStream &in, Configuration &configuration);
