#include "confmodel.h"

#include <QDataStream>
#include <QMimeData>
#include <QtDebug>

#define COL_COUNT 1

ConfModel::ConfModel(BConf *conf, QObject *parent)
	: QAbstractItemModel(parent)
	, mRoot(conf)
{

}

const BConf *ConfModel::conf(const QModelIndex &index) const
{
	return const_cast<ConfModel*>(this)->conf(index);
}

BConf *ConfModel::conf(const QModelIndex &index)
{
	BConf* rv = nullptr;
	if(index.isValid())
	{
		void* ptr = index.internalPointer();
		rv = static_cast<BConf*>(ptr);
		Q_CHECK_PTR(rv);
	}
	return rv;
}

bool ConfModel::appendChild(const QModelIndex &parent)
{
	int row = rowCount(parent);
	beginInsertRows(parent, row, row);
	bool rv = conf(parent)->appendNewChild();
	endInsertRows();
	return rv;
}

QModelIndex ConfModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column, parent.isValid()
					   ? const_cast<BConf*>(const_cast<BConf*>(conf(parent))->childAt(row))
	                   : const_cast<BConf*>(mRoot) );
}

QModelIndex ConfModel::parent(const QModelIndex &child) const
{
	QModelIndex rv;
	BConf* parentConf = const_cast<BConf*>(const_cast<ConfModel*>(this)->conf(child)->parentConf());
	if(parentConf)
	{	rv = createIndex(0,0, parentConf);	}
	return rv;
}

int ConfModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? conf(parent)->childrenCount() : 1;
}

int ConfModel::columnCount(const QModelIndex &) const
{
	return COL_COUNT;
}

QVariant ConfModel::data(const QModelIndex &index, int role) const
{
	QVariant rv;
	switch(role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
		rv = conf(index)->title();
		break;
	}
	return rv;
}

Qt::ItemFlags ConfModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags rv = QAbstractItemModel::flags(index);

	if(index.isValid())
	{
		rv |= Qt::ItemIsDragEnabled;
	}
	return rv;
}

bool ConfModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int, int, const QModelIndex &parent) const
{
	QByteArray encodedData = data->data("configuration/pointer");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	while(!stream.atEnd())
	{
		BConf* cfg;
		stream.readRawData(reinterpret_cast<char*>(&cfg), sizeof(cfg));
		qDebug() << "drop: " << (ulong)cfg;//->metaObject()->className();
	}
	return false;
}

QMimeData *ConfModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *rv = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	for(int row=indexes.first().row(); row<=indexes.last().row(); row++)
	{
		const BConf* cfg = conf(indexes.first().sibling(row,0));
		stream.writeBytes(reinterpret_cast<const char*>(&cfg), sizeof(cfg));
		qDebug() << "drag: " << (ulong)cfg;//->metaObject()->className();
	}

	rv->setData("configuration/pointer", 	encodedData);
	return rv;
}
