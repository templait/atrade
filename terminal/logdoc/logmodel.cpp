#include "logmodel.h"
#include <log.h>

LogModel::LogModel(QObject *parent) : QAbstractListModel(parent)
{
}

void LogModel::appendString(const QString &string, int level)
{
	QModelIndex parent;
	beginInsertRows(parent, rowCount(parent), rowCount(parent));
	mItems.append({string, level});
	endInsertRows();
}

void LogModel::clear()
{
	if(mItems.size() > 0)
	{
		beginRemoveRows(QModelIndex(), 0, mItems.size()-1);
		mItems.clear();
		endRemoveRows();
	}
}

int LogModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : mItems.size();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : 2;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
	QVariant rv;
	if(!index.parent().isValid() && index.row()<rowCount(index.parent()) && index.column() < columnCount(index.parent()))
	{
		switch(role)
		{
		case Qt::DisplayRole:
			switch(index.column())
			{
			case 0:
				rv = mItems[index.row()].string;
				break;
			case 1:
				rv = mItems[index.row()].level;
				break;
			}
		break;
		case Qt::ForegroundRole:
			rv = Log::color(static_cast<Log::ELevel>(mItems[index.row()].level));
			break;
		default:
			break;
		}
	}
	return rv;
}
