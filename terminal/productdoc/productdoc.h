#pragma once

#include <QDockWidget>
#include <datasources/datasourcefactory.h>

class ProductInfoModel;
class QSettings;

namespace Ui {class wProductDoc;}

class ProductDoc final: public QDockWidget
{
	Q_OBJECT
public:
	ProductDoc(QWidget *parent=nullptr);
	~ProductDoc() override;

	void saveState(QSettings& settings) const;
	void restoreState(const QSettings& settings);
private:
	Ui::wProductDoc *ui;
	int mTimer;
	QList<ProductInfo> mProductInfo;
	QList<ProductInfo> mIndicatorInfo;
	ProductInfoModel *mProductInfoModel;

	void update();

	// QObject interface
protected:
	virtual void timerEvent(QTimerEvent *event) override;

	// QWidget interface
protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;
};
