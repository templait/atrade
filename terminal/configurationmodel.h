#pragma once

#include <QAbstractItemModel>

#include <configuration.h>
#include <datasources/datasourcefactory.h>

class ConfigurationModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	ConfigurationModel(const Configuration& configuration, QObject* parent=nullptr);
	const Configuration& configuration() const;
private:
	Configuration mRoot;
	Configuration *index2configuration(const QModelIndex &index) const;

	// QAbstractItemModel interface
public:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;
	virtual int rowCount(const QModelIndex &parent) const override;
	virtual int columnCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int, int, const QModelIndex &) const override;
	virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent) override;
};
