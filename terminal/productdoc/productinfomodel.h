#pragma once

#include <QAbstractListModel>
#include <factory.hpp>

class ProductInfoModel final: public QAbstractListModel
{
	Q_OBJECT
public:
	ProductInfoModel(QObject *parent=nullptr);
	void setContent(const QList<ProductInfo>& content);
private:
	QList<ProductInfo> mContent;
	// QAbstractItemModel interface
public:
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
