#pragma once

#include <QAbstractItemModel>

#include <bconf.h>

class ConfModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	ConfModel(BConf *conf, QObject* parent=nullptr);
	const BConf *conf(const QModelIndex &index) const;
private:
	BConf* mRoot;
	BConf *conf(const QModelIndex &index);

	// QAbstractItemModel interface
public:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;
	virtual int rowCount(const QModelIndex &parent) const override;
	virtual int columnCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
/*	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int, int, const QModelIndex &parent) const override;
	virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent) override;
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
	virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
	virtual Qt::DropActions supportedDropActions() const override;
	*/
};
