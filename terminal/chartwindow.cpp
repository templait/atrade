#include "chartwidget.h"
#include "chartwindow.h"

#include <ui_chartwindow.h>
#include <QDir> // killme
#include <QScrollBar>
#include <QDateTimeAxis>
#include <QSettings> //  killme

#include <QGraphicsGridLayout>
#include <tools.h>

#include <datasources/datasourcefile.h>  // killme
#include <datasources/datasourcequik.h>  // killme
#include <indicators/candleadapterindicator.h>  // killme
#include <indicators/lualineindicator.h>  // killme
#include <datasources/datasourcefactory.h>	// killme

using namespace QtCharts;

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	//setMinimumSize(1000,700);

	mCandleWidth = 6;
	mConfiguration = defaultConfiguration();

	ui = new Ui::ChartWindow;
	ui->setupUi(this);
	//ui->graphicsView->setRenderHints(QPainter::Antialiasing);
	ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));

/*
	QPalette p;
	p.setColor(QPalette::Background, Qt::red);
	mGraphicsWidget->setPalette(p);
	mGraphicsWidget->setAutoFillBackground(true);
*/
	mGraphicsWidget = new QGraphicsWidget;
	ui->graphicsView->scene()->addItem(mGraphicsWidget);
	mSceneLayout = new QGraphicsGridLayout;
	mSceneLayout->setSpacing(-15);
	mSceneLayout->setContentsMargins(0,0,0,0);
	mGraphicsWidget->setLayout(mSceneLayout);

	for(int i = IntervalM1; i!=IntervalMN1; i++)
	{	ui->cbTimeInterval->addItem(intervalToString(static_cast<ETimeInterval>(i)), i);	}

/*
	BDataSource *ds;

	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "GAZP", "192.168.9.156", 5000, this);
	addDataSource(ds, 0);

	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "SBER", "192.168.9.156", 5000, this);
	ChartWidget* cw = addDataSource(ds, 1);

	ds = new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt", this);
	ChartWidget* cw = addDataSource(ds);
*/
/*
	Configuration conf = DataSourceFactory::instance().defaultConfiguration(QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"));
	conf["class"].setValue("TQBR");
	conf["code"].setValue("SBER");
	conf["interval"].setValue(IntervalD1);
	*/
	//ChartWidget* cw =*/ addDataSource(DataSourceFactory::instance().product(conf));
	/*if(cw)
	{
		QSettings settings;
		BIndicator *indicator;
		//indicator = new CandleAdapterIndicator(ds, CandleAdapterIndicator::MOpenClose, this);
		indicator = new LuaLineIndicator(settings.value("LuaDir", "../lua").toString() + "/indicators/MA.lua", ds, this);
		//cw->addIndicator(indicator);
	}*/

	ui->scrollBar->setEnabled(false);
	connect(ui->scrollBar, &QAbstractSlider::actionTriggered, [this](int){
		setScrollValue(ui->scrollBar->sliderPosition());
	});

	connect(ui->cbTimeInterval, QOverload<int>::of(&QComboBox::activated), [this](int index){
		setTimeInterval(static_cast<ETimeInterval>(ui->cbTimeInterval->itemData(index).toInt()));
	});
}

ChartWindow::ChartWindow(const Configuration &configuration, QWidget *parent)
    : ChartWindow(parent)
{
	loadConfiguration(configuration);
}

ChartWindow::~ChartWindow()
{
	delete ui;
}

qint64 ChartWindow::timeFrame() const
{
	qint64 rv(-1);
	if(mChartWidgets.size()>0)
	{
		qreal width = mChartWidgets[0]->plotArea().width();
		qint64 candleCount = static_cast<qint64>(width/(mCandleWidth+1));
		rv = secsInInterval(timeInterval())*candleCount;
	}
	return rv;
}

void ChartWindow::setViewTimeRange(const TimeRange & range)
{
	for(ChartWidget *w : mChartWidgets)
	{
		w->setViewTimeRange(range);
	}
}

TimeRange ChartWindow::seriesTimeRange() const
{
	TimeRange rv;
	for(ChartWidget *w : mChartWidgets)
	{
		TimeRange wRange = w->timeRange();
		if(rv.first.isNull() || rv.first > wRange.first)
		{	rv.first = wRange.first;	}
		if(rv.second.isNull() || rv.second < wRange.second)
		{	rv.second = wRange.second;	}
	}
	return rv;
}

void ChartWindow::adjustScroll()
{
	TimeRange range = seriesTimeRange();
	if(! (range.first.isNull() || range.second.isNull()))
	{
		qint64 max = range.second.toSecsSinceEpoch()+secsInInterval(timeInterval());
		qint64 min = range.first.toSecsSinceEpoch()+timeFrame()/2-secsInInterval(timeInterval());
		qint64 step = secsInInterval(timeInterval());

		// нужно выровнять разницу между минимумом и максимумом по минимальному шагу
		qint64 rem = (max-min+1)%step;
		if(rem>0)
		{	max+=step-rem;	}

		ui->scrollBar->setRange(rescaleInt64(min), rescaleInt64(max));

		ui->scrollBar->setSingleStep(rescaleInt64(step));
		ui->scrollBar->setPageStep(rescaleInt64(timeFrame()));
		if(! ui->scrollBar->isEnabled())
		{
			ui->scrollBar->setEnabled(true);
			ui->scrollBar->setSliderPosition(ui->scrollBar->maximum());
		}
	}
	else
	{
		ui->scrollBar->setRange(0,0);
		ui->scrollBar->setEnabled(false);
	}
}

void ChartWindow::setScrollValue(int value)
{
	QDateTime max = QDateTime::fromSecsSinceEpoch(value);
	QDateTime min = QDateTime::fromSecsSinceEpoch(value - timeFrame());
	setViewTimeRange({min, max});
}

void ChartWindow::onCandlesAppend(const DataSource &dataSource, int)
{
	bool need2Last = false;
	if(ui->scrollBar->sliderPosition() == ui->scrollBar->maximum())
	{	need2Last = true;	}
	adjustScroll();
	if(need2Last)
	{
		ui->scrollBar->blockSignals(true);
		setScrollValue(ui->scrollBar->maximum());
		ui->scrollBar->setSliderPosition(ui->scrollBar->maximum());
		ui->scrollBar->blockSignals(false);
	}
}

void ChartWindow::adjustGraphicsScene()
{
	mGraphicsWidget->setGeometry(ui->graphicsView->childrenRect());
	ui->graphicsView->scene()->setSceneRect(ui->graphicsView->childrenRect());
}

void ChartWindow::setTimeInterval(ETimeInterval interval)
{
	Configuration newConf(mConfiguration);
	newConf["TimeInterval"].setValue(interval);
	loadConfiguration(newConf);
}

ETimeInterval ChartWindow::timeInterval() const
{
	Q_ASSERT(mConfiguration.containsChild("TimeInterval"));
	return static_cast<ETimeInterval>(mConfiguration["TimeInterval"].value().toInt());
}

int ChartWindow::rescaleInt64(qint64 value) const
{
	//static_cast<int>(value * std::numeric_limits<int>::max() / std::numeric_limits<qint64>::max());
	return static_cast<int>(value);
}

void ChartWindow::loadConfiguration(const Configuration& configuration)
{
	clear();
	mConfiguration = configuration;
	for(int i=0; i<mConfiguration.childrenCount(); i++)
	{
		const Configuration* conf = mConfiguration.childAt(i);
		if(conf->name() == "chart")
		{	
			cregetChartWidget(*conf, i);
		}
	}
	ui->cbTimeInterval->blockSignals(true);
	for(int i=0; i<ui->cbTimeInterval->count(); i++)
	{
		if(ui->cbTimeInterval->itemData(i) == timeInterval())
		{
			ui->cbTimeInterval->setCurrentIndex(i);
			break;
		}
	}
	ui->cbTimeInterval->blockSignals(false);
	QApplication::processEvents(); // нужно подождать пока все виджеты примут должный размер
	adjustScroll();
}

const Configuration &ChartWindow::configuration() const
{
	return mConfiguration;
}

Configuration ChartWindow::defaultConfiguration()
{
	Configuration chartWindow("ChartWindow", QVariant(), tr("Oкно графиков"));
	chartWindow.appendChild({Configuration::Value, "TimeInterval", ETimeInterval::IntervalD1, tr("Интервал")});
	chartWindow.appendChild({Configuration::Title, "chart", QVariant(), tr("График")});

	return chartWindow;
}

void ChartWindow::saveConfiguration(QSettings &settings) const
{
	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream << mConfiguration;
	settings.setValue("configuration", array);
}

void ChartWindow::loadConfiguration(QSettings &settings)
{
	QByteArray array = settings.value("configuration").toByteArray();
	QDataStream stream(&array, QIODevice::ReadOnly);
	Configuration conf;
	stream >> conf;
	loadConfiguration(conf);
}

void ChartWindow::clear()
{
	qDeleteAll(mChartWidgets);
	mChartWidgets.clear();
	adjustScroll();
}

ChartWidget *ChartWindow::cregetChartWidget(const Configuration& configuration, int widgetNum)
{
	ChartWidget * rv = nullptr;

	if(widgetNum >= mChartWidgets.size())
	{
		widgetNum = mChartWidgets.size();
		rv = new ChartWidget(timeInterval(), configuration);
		mSceneLayout->addItem(rv, widgetNum, 0);
		mChartWidgets << rv;
		connect(rv, &ChartWidget::candlesAppended, this, &ChartWindow::onCandlesAppend);
	}
	else
	{	rv = mChartWidgets[widgetNum];	}
	return rv;
}

void ChartWindow::resizeEvent(QResizeEvent *event)
{
	adjustGraphicsScene();
	adjustScroll();
	ui->scrollBar->setPageStep(rescaleInt64(timeFrame()));
	setScrollValue(ui->scrollBar->sliderPosition());
	QWidget::resizeEvent(event);
}

void ChartWindow::showEvent(QShowEvent *event)
{
	adjustGraphicsScene();
	adjustScroll();
	ui->scrollBar->setPageStep(rescaleInt64(timeFrame()));
	setScrollValue(ui->scrollBar->sliderPosition());
	QWidget::showEvent(event);
}
