#include "productlistmodel.h"

ProductListModel::ProductListModel(QObject *parent)
	: QAbstractItemModel(parent)
	, mSections{
			{tr("Источники данных"), DataSourceFactory::instance().productList()}
		  }
{
}

QString ProductListModel::sectionNumToName(int sectionNumber) const
{
	return mSections.keys()[sectionNumber];
}

QModelIndex ProductListModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex rv;
	if(row >= 0 && column >= 0)
	{
		if(! parent.isValid())
		{
			if(column==0 && row<mSections.size())
			{
				rv = createIndex(row, column, quintptr(-1));
			}
		}
		else if(parent.isValid() && !parent.parent().isValid())
		{
			int sectionNum = static_cast<int>(parent.internalId());
			if(sectionNum<=mSections.size() && column<2)
			{
				rv = createIndex(row, column, static_cast<quintptr>(row));
			}
		}
	}
	return rv;
}

QModelIndex ProductListModel::parent(const QModelIndex &child) const
{
	QModelIndex rv;
	if(child.isValid())
	{
		int sectionNum = static_cast<int>(child.internalId());
		if(sectionNum>=0)
		{	rv = index(sectionNum, 0, QModelIndex());	}
	}
	return rv;
}

int ProductListModel::rowCount(const QModelIndex &parent) const
{
	int rv=0;
	if(parent.isValid() && !parent.parent().isValid())
	{
		{	rv = mSections[sectionNumToName(parent.row())].size();}
	}
	else if(!parent.isValid())
	{
		rv= mSections.size();
	}
	return rv;
}

int ProductListModel::columnCount(const QModelIndex &parent) const
{
	int rv=0;
	if(parent.isValid() && !parent.parent().isValid())
	{	rv = 2;	}
	else if(!parent.isValid())
	{	rv = 1;	}
	return rv;
}

QVariant ProductListModel::data(const QModelIndex &index, int role) const
{
	QVariant rv;
	if(role == Qt::DisplayRole && index.isValid())
	{
		if(index.parent().isValid())
		{
			int sectionNum = static_cast<int>(index.internalId());
			const ProductList& productList = mSections[sectionNumToName(sectionNum)];
			switch(index.column())
			{
			case 0:
				rv = productList[index.row()].first;
				break;
			case 1:
				rv = productList[index.row()].second.toByteArray();
				break;
			}
		}
		else
		{
			int sectionNum = static_cast<int>(index.row());
			rv = sectionNumToName(sectionNum);
		}
	}
	return rv;
}
