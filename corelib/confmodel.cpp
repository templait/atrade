#include "confmodel.h"

#include <QDataStream>
#include <QMimeData>
#include <QStack>

#include <datasources/datasourcefactory.h>
#include <indicators/indicatorfactory.h>

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

void ConfModel::saveIndexes(QDataStream &out, const QModelIndexList &indexes) const
{
	out << indexes.count();
	for (const QModelIndex &index: indexes){
		QModelIndex localIndex = index;
		QStack<int> indexParentStack;
		while (localIndex.isValid())
		{
			indexParentStack << localIndex.row();
			localIndex = localIndex.parent();
		}

		out << indexParentStack.size();
		while (!indexParentStack.isEmpty())
		{
			out << indexParentStack.pop();
		}
	}
}

QModelIndexList ConfModel::loadIndexes(QDataStream &in) const
{
	QModelIndexList result;
	int count;
	in >> count;
	for(int i=0; i<count; i++)
	{
		int childDepth = 0;
		in >> childDepth;

		QModelIndex currentIndex = {};
		for (int i = 0; i < childDepth; ++i)
		{
			int row = 0;
			in >> row;
			currentIndex = index(row, 0, currentIndex);
		}
		result << currentIndex;
	}
	return result;
}

QModelIndex ConfModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex rv;
	if(parent.isValid())
	{
		BConf* parentConf = const_cast<BConf*>(conf(parent));
		if(row < parentConf->childrenCount())
		{	rv = createIndex(row, column, const_cast<BConf*>(parentConf->childAt(row)));	}
	}
	else
	{	rv = createIndex(row, column, const_cast<BConf*>(mRoot));	}
	return rv;
/*
	return createIndex(row, column, parent.isValid()
					   ? const_cast<BConf*>(const_cast<BConf*>(conf(parent))->childAt(row))
	                   : const_cast<BConf*>(mRoot) );
*/
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
		rv |= (Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
	}
	return rv;
}

// workaround. Сие необходимо для обхода глюка КуТе. При обработке события DragStrart необходимо вернуть true.
// Иначе не будут поступать события DrugMove. Поэтому при первом заходе с новыми данными, всегда возвращаем true.
static const QMimeData *__data = nullptr;

bool ConfModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int, int, const QModelIndex &parent) const
{
	if(data != __data)
	{
		__data = data;
		return true;
	}

	bool rv = false;
	if(parent.isValid())
	{
		if(action==Qt::MoveAction && data->hasFormat("conf/model-index"))
		{
			QByteArray encodedData = data->data("conf/model-index");
			QDataStream stream(&encodedData, QIODevice::ReadOnly);
			for(const QModelIndex& index : loadIndexes(stream))
			{
				rv = conf(parent)->canAppendChild(*conf(index));
				if(rv) break;
			}
		}
		else if(action==Qt::CopyAction && data->hasFormat("conf/product-id"))
		{
			QByteArray encodedData = data->data("conf/product-id");
			QDataStream stream(&encodedData, QIODevice::ReadOnly);
			ProductID id;
			stream >> id;
			if(DataSourceFactory::instance().hasProduct(id))
			{
				BConf* cfg = DataSourceFactory::instance().createDefaultConf(id);
				rv = conf(parent)->canAppendChild(*cfg);
				delete cfg;
			}
			else if(IndicatorFactory::instance().hasProduct(id))
			{
				BConf* cfg = IndicatorFactory::instance().createDefaultConf(id);
				rv = conf(parent)->canAppendChild(*cfg);
				delete cfg;
			}
		}
	}
	return rv;
}

bool ConfModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent)
{
	__data = nullptr;
	bool rv=false;
	if(action==Qt::MoveAction && data->hasFormat("conf/model-index"))
	{
		QByteArray encodedData = data->data("conf/model-index");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);
		const QModelIndexList& indexes = loadIndexes(stream);
		beginInsertRows(parent, row, row+indexes.count()-1);
		for(const QModelIndex& index : indexes)
		{
			conf(index)->setTitle(conf(index)->title()+"1");
			conf(parent)->insertChild(*conf(index), row++);
		}
		endInsertRows();
		rv = true;
	}
	else if(action==Qt::CopyAction && data->hasFormat("conf/product-id"))
	{
		QByteArray encodedData = data->data("conf/product-id");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);
		ProductID id;
		stream >> id;
		if(DataSourceFactory::instance().hasProduct(id))
		{
			BConf* cfg = DataSourceFactory::instance().createDefaultConf(id);
			rv = conf(parent)->insertChild(*cfg, row);
			delete cfg;
		}
		else if(IndicatorFactory::instance().hasProduct(id))
		{
			BConf* cfg = IndicatorFactory::instance().createDefaultConf(id);
			rv = conf(parent)->insertChild(*cfg, row);
			delete cfg;
		}
	}
	return rv;
}

QMimeData *ConfModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *rv = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	QModelIndexList idxs;
	for(int row=indexes.first().row(); row<=indexes.last().row(); row++)
	{
		idxs << indexes.first().sibling(row,0);
	}
	saveIndexes(stream, idxs);

	rv->setData("conf/model-index", 	encodedData);
	return rv;
}

bool ConfModel::removeRows(int row, int count, const QModelIndex &parent)
{
	BConf* parentConf = conf(parent);
	row = qBound(0, row, parentConf->childrenCount()-1);
	count = qBound(0, count, parentConf->childrenCount()-row);

	beginRemoveRows(parent, row, row+count-1);
	for(int i=row; i<row+count; i++)
	{	parentConf->removeChild(i);	}
	endRemoveRows();

	return count>0;
}

Qt::DropActions ConfModel::supportedDropActions() const
{
	return Qt::MoveAction | Qt::CopyAction;
}

QStringList ConfModel::mimeTypes() const
{
	// эта функция необходима т.к. КуТе проверяет не только canDropMimeData() перед тем как разрешить перемещение элемента
	return {"conf/model-index", "conf/product-id"};
}
