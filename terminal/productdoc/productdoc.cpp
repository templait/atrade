#include "productdoc.h"
#include "productinfomodel.h"
#include <qsettings.h>
#include <ui_productdoc.h>
#include <datasources/datasourcefactory.h>
#include <indicators/indicatorfactory.h>

namespace
{
bool operator==(const ProductInfo& first, const ProductInfo& second)
{
	return		first.type==second.type
			&&	first.info==second.info
			&&	first.count==second.count
			&&	first.addr==second.addr;
}

} // namespace

ProductDoc::ProductDoc(QWidget *parent) : QDockWidget(parent), mTimer(0)
{
	setWidget(new QWidget(this));
	ui = new Ui::wProductDoc;
	ui->setupUi(widget());
	setObjectName(widget()->objectName() + "_Doc");
	setWindowTitle(widget()->windowTitle());

	mProductInfoModel = new ProductInfoModel(this);
	ui->tvProducts->setModel(mProductInfoModel);
}

ProductDoc::~ProductDoc()
{
	delete ui;
}

void ProductDoc::saveState(QSettings &settings) const
{
	settings.setValue("HeaderGeometry", ui->tvProducts->header()->saveGeometry());
	settings.setValue("HeaderState", ui->tvProducts->header()->saveState());
}

void ProductDoc::restoreState(const QSettings &settings)
{
	ui->tvProducts->header()->restoreGeometry(settings.value("HeaderGeometry").toByteArray());
	ui->tvProducts->header()->restoreState(settings.value("HeaderState").toByteArray());
}

void ProductDoc::update()
{
	mProductInfoModel->setContent(mProductInfo+mIndicatorInfo);
}

void ProductDoc::timerEvent(QTimerEvent *)
{
	QList<ProductInfo> pInf = DataSourceFactory::instance().info();
	QList<ProductInfo> iInf = IndicatorFactory::instance().info();
	if(pInf!=mProductInfo || iInf!=mIndicatorInfo)
	{
		mProductInfo=pInf;
		mIndicatorInfo=iInf;
		update();
	}
}

void ProductDoc::showEvent(QShowEvent *)
{
	mTimer = startTimer(1000);
}

void ProductDoc::hideEvent(QHideEvent *)
{
	killTimer(mTimer);
}
