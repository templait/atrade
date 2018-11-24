#include "confmodel.h"

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

bool ConfModel::appendChild(const QModelIndex &parent, int childType)
{
	int row = rowCount(parent);
	beginInsertRows(parent, row, row);
	bool rv = conf(parent)->appendChild(childType);
	endInsertRows();
	return rv;
}

QModelIndex ConfModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column, parent.isValid()
	                   ? const_cast<BConf*>(conf(parent))->childAt(row)
	                   : const_cast<BConf*>(mRoot) );
}

QModelIndex ConfModel::parent(const QModelIndex &child) const
{
	QModelIndex rv;
	BConf* parentConf = const_cast<BConf*>(conf(child))->parentConf();
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
