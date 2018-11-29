#include "productlistmodel.h"

#include <QApplication>
#include <QDataStream>
#include <QMimeData>

#include <datasources/datasourcefactory.h>
#include <indicators/indicatorfactory.h>

ProductListModel::ProductListModel(QObject *parent)
	: QAbstractItemModel(parent)
	, mSections{
			{tr("Источники данных"), DataSourceFactory::instance().productList()},
			{tr("Индикаторы"), IndicatorFactory::instance().productList()}
		  }
{
}

QModelIndex ProductListModel::dataSourcesRoot() const
{
	return index(0,0, QModelIndex());
}

QModelIndex ProductListModel::indicatorsRoot() const
{
	return index(1,0, QModelIndex());
}

QModelIndex ProductListModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex rv;
	if(row >= 0 && column >= 0)
	{
		int sectionNum = static_cast<int>(parent.internalId());
		if(sectionNum == -1)
		{
			if(sectionNum<=mSections.size() && column<2)
			{
				rv = createIndex(row, column, static_cast<quintptr>(parent.row()));
			}
		}
		else if(! parent.isValid())
		{
			if(column==0 && row<mSections.size())
			{
				rv = createIndex(row, column, quintptr(-1));
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
	int sectionNum = static_cast<int>(parent.internalId());
	if(sectionNum==-1)	// родитель - секция
	{
		rv = mSections[parent.row()].second.size();
	}
	else if(!parent.isValid())
	{
		rv = mSections.size();
	}
	return rv;
}

int ProductListModel::columnCount(const QModelIndex &parent) const
{
	int rv=0;
	int sectionNum = static_cast<int>(parent.internalId());
	if(sectionNum==-1)
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
			const ProductList& productList = mSections[sectionNum].second;
			switch(index.column())
			{
			case 0:
				rv = productList[index.row()].first;
				break;
			case 1:
				rv = productList[index.row()].second;
				break;
			}
		}
		else
		{
			int sectionNum = static_cast<int>(index.row());
			rv = mSections[sectionNum].first;
		}
	}
	return rv;
}

QMimeData *ProductListModel::mimeData(const QModelIndexList &indexes) const
{
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	for(const QModelIndex& index : indexes)
	{
		ProductID id = index.sibling(index.row(), 1).data().toUuid();
		stream << id;
	}
	QMimeData *rv = new QMimeData();
	rv->setData("conf/product-id", 	encodedData);
	return rv;
}

Qt::ItemFlags ProductListModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags rv = QAbstractItemModel::flags(index);
	if(index.isValid() && index.parent().isValid() && index.column()==0)
	{	rv |= Qt::ItemIsDragEnabled;	}
	return rv;
}
