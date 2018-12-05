#pragma once

#include <QAbstractListModel>

class LogModel final: public QAbstractListModel
{
	Q_OBJECT
public:
	LogModel(QObject *parent=nullptr);
	void appendString(const QString & string, int level = 0);
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
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
};
