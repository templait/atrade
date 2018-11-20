#pragma once

#include <QVariant>

class Configuration
{
public:
	enum EParam {Name=0x01, Value=0x02, Title=0x04, WriteAll=0xFFFF, ReadOnly=0x0};

	Configuration();
	Configuration(const Configuration &other);
	Configuration(std::initializer_list<Configuration> list);
	Configuration(const QString& name, const QVariant& value, const QString& title = QString());
	Configuration( int userEditableMap, const QString& name, const QVariant& value, const QString& title = QString());

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

	void insertChild(Configuration child, int index=-1);
	void deleteChild(int index);
	int deleteAllChild(const QString& name = QString());
	const Configuration *childAt(int index) const;
	int childrenCount() const;
	bool containsChild(const QString& childName) const;

	bool userEditabe(EParam param) const;
	void setUserEditableMap(int map);
	Configuration * parent();
private:
	QString mName;
	QVariant mValue;
	QString mTitle;
	QList<Configuration> mChildren;
	Configuration *mParent;
	int mUserEditableMap;
};

QDataStream& operator<<(QDataStream& out, const Configuration& configuration);
QDataStream& operator>>(QDataStream &in, Configuration &configuration);
