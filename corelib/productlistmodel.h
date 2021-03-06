#pragma once

#include <QAbstractItemModel>
#include <factory.hpp>

class ProductListModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	ProductListModel(QObject *parent=nullptr);
	QModelIndex dataSourcesRoot() const;
	QModelIndex indicatorsRoot() const;
private:
	QList<QPair<QString, ProductList> > mSections;

	// QAbstractItemModel interface
public:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;
	virtual int rowCount(const QModelIndex &parent) const override;
	virtual int columnCount(const QModelIndex &parent) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};
