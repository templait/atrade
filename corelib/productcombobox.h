#pragma once

#include <QComboBox>
#include <factory.hpp>

class ProductListModel;

class ProductComboBox : public QComboBox
{
	Q_OBJECT
public:
	enum EType {TypeDataSource, TypeIndicator};
	ProductComboBox(QWidget* parent = nullptr, EType type=TypeDataSource);
	void setType(EType type);
	ProductID currentProduct() const;
private:
	ProductListModel *mModel;
};
