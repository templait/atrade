#include "configurationmodel.h"

#define COL_COUNT 3

ConfigurationModel::ConfigurationModel(QObject *parent)
	: QAbstractItemModel(parent)
	, mRoot("root", QVariant(), tr("Конфигурация"))
{
	addNewDataSource(ProductID("ab38fe10-d502-11e8-b568-0800200c9a66"));
}

void ConfigurationModel::addNewDataSource(const ProductID &productID)
{
	Configuration conf = DataSourceFactory::instance().defaultConfiguration(productID);
	mRoot.appendChild(conf);
}

Configuration *ConfigurationModel::index2configuration(const QModelIndex &index) const
{
	Configuration* rv = nullptr;
	if(void* ptr = index.internalPointer())
	{
		rv = static_cast<Configuration*>(ptr);
		Q_ASSERT(rv);
	}
	return rv;
}

QModelIndex ConfigurationModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex rv;
	if(column < COL_COUNT)
	{
		if(parent.isValid())
		{
			Configuration* parentConf = index2configuration(parent);
			if(Configuration* conf = parentConf->childAt(row))
			{
				QModelIndex idx = createIndex(row, column, conf);
				if(column <= columnCount(idx))
				{	rv = idx;	}
			}
		}
		else if(row==0)
		{
			rv = createIndex(0,column, const_cast<Configuration*>(&mRoot));
		}
	}
	return rv;
}

QModelIndex ConfigurationModel::parent(const QModelIndex &child) const
{
	QModelIndex rv;
	if(Configuration* parentConf = index2configuration(child)->parent())
	{
		rv = createIndex(0,0, parentConf);
	}
	return rv;
}

int ConfigurationModel::rowCount(const QModelIndex &parent) const
{
	int rv = 1;
	if(parent.isValid())
	{
		rv = index2configuration(parent)->childrenCount();
	}
	return rv;
}

int ConfigurationModel::columnCount(const QModelIndex & /*parent*/) const
{
	/*
	int rv = COL_COUNT;
	Configuration* conf = index2configuration(parent);
	if(conf->parent() == &mRoot || conf == &mRoot)
	{	rv = 1;	}
	return rv;
	*/
	return COL_COUNT;
}

QVariant ConfigurationModel::data(const QModelIndex &index, int role) const
{
	QVariant rv;
	Configuration* conf = index2configuration(index);
	if(index.column()==0)
	{
		switch(role)
		{
		case Qt::DisplayRole:
			rv = conf->title();
			break;

		}
	}
	else if(index.column()==1)
	{
		switch(role)
		{
		case Qt::DisplayRole:
			rv = conf->value();
			break;

		}
	}
	else if(index.column()==2)
	{
		switch(role)
		{
		case Qt::DisplayRole:
			rv = conf->name();
			break;

		}
	}
	return rv;
}

QVariant ConfigurationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant rv;
	if(orientation==Qt::Horizontal && role==Qt::DisplayRole)
	{
		switch(section)
		{
		case 0: rv = tr("Название"); break;
		case 1: rv = tr("Значение"); break;
		case 2: rv = tr("Имя"); break;
		}
	}
	return rv;
}

Qt::ItemFlags ConfigurationModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags rv = QAbstractItemModel::flags(index);
	if(index.isValid() && index.parent().isValid())
	{	rv |= Qt::ItemIsDropEnabled;	}
	return rv;
}

bool ConfigurationModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
	bool rv=false;
	//if(!parent.isValid())
	{
		rv = true;
	}
	return rv;
}

