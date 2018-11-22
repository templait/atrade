#include "confmodel.h"

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
	BConf* rv = mRoot;
	if(index.isValid())
	{
		void* ptr = index.internalPointer();
		Q_CHECK_PTR(rv);
		rv = static_cast<BConf*>(ptr);
	}
	return rv;
}

QModelIndex ConfModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex rv;
	if(column < COL_COUNT)
	{
		if(parent.isValid())
		{
			const BConf* parentConf = conf(parent);
			if(BConf* conf = const_cast<BConf*>(parentConf)->childAt(row))
			{
				QModelIndex idx = createIndex(row, column, conf);
				if(column <= columnCount(idx))
				{	rv = idx;	}
			}
		}
		else
		{	rv = createIndex(row, column, const_cast<ConfModel*>(this)->conf(parent));	}
	}
	return rv;
}

QModelIndex ConfModel::parent(const QModelIndex &child) const
{
	QModelIndex rv;
	BConf* childConf = const_cast<BConf*>(conf(child));
	if(childConf!=mRoot)
	{	createIndex(0,0, childConf->parentConf());	}
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
