#include "configurationmodel.h"

#include <QDataStream>
#include <QMimeData>

#define COL_COUNT 3

ConfigurationModel::ConfigurationModel(const Configuration& configuration, QObject *parent)
	: QAbstractItemModel(parent)
    , mRoot(configuration)
{
}

const Configuration &ConfigurationModel::configuration() const
{
	return mRoot;
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
			if(const Configuration* conf = parentConf->childAt(row))
			{
				QModelIndex idx = createIndex(row, column, const_cast<Configuration*>(conf));
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

bool ConfigurationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool rv = false;
	Configuration* conf = index2configuration(index);
	if(index.column()==0)
	{
		switch(role)
		{
		case Qt::EditRole:
			conf->setTitle(value.toString());
			rv = true;
			break;

		}
	}
	else if(index.column()==1)
	{
		switch(role)
		{
		case Qt::EditRole:
			conf->setValue(value);
			rv = true;
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

	if(index.isValid())
	{
		Configuration* conf = index2configuration(index);
		if(conf->name() == "chart")
		{	rv |= Qt::ItemIsDropEnabled;	}

		if(index.column()==0  && conf->userEditabe(Configuration::Title))
		{	rv |= Qt::ItemIsEditable;	}
		else if(index.column()==1  && conf->userEditabe(Configuration::Value))
		{	rv |= Qt::ItemIsEditable;	}
		else if(index.column()==2  && conf->userEditabe(Configuration::Name))
		{	rv |= Qt::ItemIsEditable;	}
	}
	return rv;
}

bool ConfigurationModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex &/*parent*/) const
{
	bool rv=false;
	if(action & (Qt::MoveAction|Qt::CopyAction) && data->hasFormat("configuration/datasource"))
	{	rv = true;	}
	return rv;
}

bool ConfigurationModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int /*column*/, const QModelIndex &parent)
{
	QByteArray encodedData = data->data("configuration/datasource");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QList<Configuration> configs;
	while (!stream.atEnd()) {
		Configuration conf;
		stream >> conf;
		configs << conf;
	}
	if(action==Qt::CopyAction)
	{
		beginInsertRows(parent, row, row+configs.size()-1);
		for(const Configuration& conf : configs)
		{
			index2configuration(parent)->appendChild(conf);
		}
		endInsertRows();
	}
	return true;
}

