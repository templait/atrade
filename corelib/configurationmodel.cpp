#include "configurationmodel.h"

#include <datasources/datasourcefactory.h>
#include <indicators/indicatorfactory.h>
#include <confnames.h>

#include <QDataStream>
#include <QMimeData>

#define COL_COUNT 3

ConfigurationModel::ConfigurationModel(const Configuration& configuration, QObject *parent)
	: QAbstractItemModel(parent)
    , mRoot(configuration)
{
}

const Configuration &ConfigurationModel::configuration(const QModelIndex &index) const
{
	return const_cast<ConfigurationModel*>(this)->configuration(index);
}

Configuration &ConfigurationModel::configuration(const QModelIndex &index)
{
	Configuration* rv = &mRoot;
	if(index.isValid())
	{
		rv = index2configuration(index);
	}
	return *rv;
}

void ConfigurationModel::insertChild(const QModelIndex &parent, const Configuration &child, int row)
{
	Configuration* parentConf = index2configuration(parent);
	int insRow = (row<0 || row>=parentConf->childrenCount()) ? parentConf->childrenCount() : row;
	beginInsertRows(parent, insRow, insRow);
	parentConf->insertChild(child, insRow);
	endInsertRows();
}

QModelIndex ConfigurationModel::modelIndex(const Configuration *conf, const QModelIndex &parent)
{
	const Configuration& rootConf = configuration(parent);
	QModelIndex rv = QModelIndex();
	if(&rootConf==conf)
	{	rv = parent;	}
	else
	{
		for(int i=0; i<rootConf.childrenCount(); i++)
		{
			rv = modelIndex(conf, index(i,0, parent));
			if(rv.isValid())
			{	break;	}
		}
	}
	return rv;
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

bool ConfigurationModel::isProduct(const Configuration *configuration) const
{
	bool rv = false;
	if(configuration)
	{
		ProductID id = configuration->value().toUuid();
		rv =  DataSourceFactory::instance().hasProduct(id) || IndicatorFactory::instance().hasProduct(id);
	}
	return rv;
}

bool ConfigurationModel::isChart(const Configuration *configuration) const
{
	return configuration && configuration->name()==CN_CHART;
}

bool ConfigurationModel::isRoot(const Configuration *configuration) const
{
	return configuration== &mRoot;
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
	int rv = mRoot.childrenCount();
	if(parent.isValid())
	{
		Configuration* parentConf = index2configuration(parent);
		// Не показываем дочернии элементы для продуктов.
		// Все изменения в настройках продуктов должны осущемтвляться с помощью ConfigurationEditorModule.
		if(! (DataSourceFactory::instance().hasProduct(parentConf->value().toUuid())
			  || IndicatorFactory::instance().hasProduct(parentConf->value().toUuid()))
		)
		{	rv = parentConf->childrenCount();	}
		else
		{	rv = 0;	}
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
		case Qt::EditRole:
			rv = conf->title();
			break;

		}
	}
	else if(index.column()==1)
	{
		switch(role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			rv = conf->value();
			break;

		}
	}
	else if(index.column()==2)
	{
		switch(role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
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
	if(rv)
	{	emit dataChanged(index, index);	}
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
		if(isRoot(conf))
		{	rv |= Qt::ItemIsDropEnabled;	}
		else if(isChart(conf) && index.column()==0)
		{	rv |= Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;	}
		else if(isProduct(conf))
		{	rv |= Qt::ItemIsDragEnabled;	}

		if(index.column()==0  && conf->userEditabe(Configuration::Title))
		{	rv |= Qt::ItemIsEditable;	}
		else if(index.column()==1  && conf->userEditabe(Configuration::Value))
		{	rv |= Qt::ItemIsEditable;	}
		else if(index.column()==2  && conf->userEditabe(Configuration::Name))
		{	rv |= Qt::ItemIsEditable;	}
	}
	return rv;
}

bool ConfigurationModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex & parent) const
{
	bool rv=false;
	if(data->hasFormat("configuration/product") || data->hasFormat("configuration/chart"))
	{
		rv = true;
		Configuration * parentConf = index2configuration(parent);
		if(action & (Qt::MoveAction|Qt::CopyAction))
		{
			if(data->hasFormat("configuration/product") && (isProduct(parentConf) || isRoot(parentConf)))
			{	rv = false;	}
			else if(data->hasFormat("configuration/chart") && (isChart(parentConf) || isProduct(parentConf)))
			{	rv = false;	}
		}
	}
	return rv;
}

bool ConfigurationModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int /*column*/, const QModelIndex &parent)
{
	bool rv=false;

	QStringList list {"configuration/product", "configuration/chart"};
	for(const QString& mimeType : list)
	{
		QByteArray encodedData = data->data(mimeType);
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QList<Configuration> configs;
		while (!stream.atEnd()) {
			Configuration conf;
			stream >> conf;
			configs << conf;
		}
		if(action==Qt::CopyAction || action==Qt::MoveAction)
		{
			for(const Configuration& conf : configs)
			{
				insertChild(parent, conf, row++);
			}
			rv = true;
		}
	}
	return rv;
}

QMimeData *ConfigurationModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *rv = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	const QModelIndex& index = indexes.first().sibling(indexes.first().row(), 1);
	Configuration* conf = index2configuration(index);
	ProductID id = conf->value().toUuid();
	if(DataSourceFactory::instance().hasProduct(id) || IndicatorFactory::instance().hasProduct(id))
	{
		stream << *conf;
		rv->setData("configuration/product", 	encodedData);
	}
	else if(conf->name() == CN_CHART)
	{
		stream << *conf;
		rv->setData("configuration/chart", 	encodedData);
	}
	return rv;
}

bool ConfigurationModel::removeRows(int row, int count, const QModelIndex &parent)
{
	Configuration* parentConf = index2configuration(parent);
	row = qBound(0, row, parentConf->childrenCount()-1);
	count = qBound(0, count, parentConf->childrenCount()-row);

	beginRemoveRows(parent, row, row+count-1);
	for(int i=row; i<row+count; i++)
	{	parentConf->deleteChild(i);	}
	endRemoveRows();

	return count>0;
}

Qt::DropActions ConfigurationModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}
/*
bool ConfigurationModel::insertRows(int row, int count, const QModelIndex &parent)
{
	Configuration* pConf = index2configuration(parent);
	beginInsertRows(parent, pConf->childrenCount()-1, pConf->childrenCount()-1);
	pConf->appendChild({"Name", 10, "Title"});
	endInsertRows();
	return true;
}*/

