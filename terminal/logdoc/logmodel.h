#pragma once

#include <QAbstractListModel>

class LogModel : public QAbstractListModel
{
	Q_OBJECT
public:
	LogModel(QObject *parent=nullptr);
	void appendString(const QString & string, int level = 0);
public slots:
	void clear();
private:
	struct Item
	{
		QString string;
		int level;
	};
	QList<Item> mItems;
	// QAbstractItemModel interface
public:
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
};
