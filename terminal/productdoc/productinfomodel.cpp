#include "productinfomodel.h"

ProductInfoModel::ProductInfoModel(QObject *parent) : QAbstractListModel(parent)
{

}

void ProductInfoModel::setContent(const QList<ProductInfo> &content)
{
	beginResetModel();
	mContent=content;
	endResetModel();
}

int ProductInfoModel::rowCount(const QModelIndex & parent) const
{
	return parent.isValid() ? 0 : mContent.count();
}

int ProductInfoModel::columnCount(const QModelIndex & parent) const
{
	return parent.isValid() ? 0 : 4;
}

QVariant ProductInfoModel::data(const QModelIndex &index, int role) const
{
	QVariant rv;
	if(!index.parent().isValid() && index.row()<rowCount(index.parent()) && index.column() < columnCount(index.parent()))
	if(role==Qt::DisplayRole || role==Qt::ToolTipRole)
	{
		switch(index.column())
		{
		case 0:
			rv = mContent[index.row()].type;
			break;
		case 1:
			rv = mContent[index.row()].info;
			break;
		case 2:
			rv = mContent[index.row()].addr;
			break;
		case 3:
			rv = mContent[index.row()].count;
			break;
		}
	}
	return rv;
}

QVariant ProductInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant rv;
	if(orientation == Qt::Horizontal && role==Qt::DisplayRole)
	{
		switch(section)
		{
		case 0:
			rv = tr("Type");
			break;
		case 1:
			rv = tr("Info");
			break;
		case 2:
			rv = tr("Address");
			break;
		case 3:
			rv = tr("Count");
			break;
		}
	}
	return rv;
}
