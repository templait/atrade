#include "chartwidget.h"
#include "chartwindow.h"

#include <QGraphicsGridLayout>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QVBoxLayout>
#include <QDir> // killme
#include <QScrollBar>
#include <QDateTimeAxis>
#include <QSettings> //  killme

#include <tools.h>

#include <datasources/datasourcefile.h>  // killme
#include <datasources/datasourcequik.h>  // killme
#include <indicators/candleadapterindicator.h>  // killme
#include <indicators/lualineindicator.h>  // killme
#include <datasources/datasourcefactory.h>	// killme

using namespace QtCharts;

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	setMinimumSize(1000,700);

	mTimeInterval = IntervalD1;
	mCandleWidth = 6;

	mLayout = new QVBoxLayout;
	setLayout(mLayout);
	mLayout->setSpacing(1);

	mGraphicsView = new QGraphicsView;
	//mGraphicsView->setRenderHints(QPainter::Antialiasing);
	mGraphicsView->setScene(new QGraphicsScene(mGraphicsView));
	mGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mGraphicsWidget = new QGraphicsWidget;
/*
	QPalette p;
	p.setColor(QPalette::Background, Qt::red);
	mGraphicsWidget->setPalette(p);
	mGraphicsWidget->setAutoFillBackground(true);
*/
	mGraphicsView->scene()->addItem(mGraphicsWidget);
	mSceneLayout = new QGraphicsGridLayout;
	mSceneLayout->setSpacing(-15);
	mSceneLayout->setContentsMargins(0,0,0,0);
	mGraphicsWidget->setLayout(mSceneLayout);

	mLayout->addWidget(mGraphicsView);
/*
	BDataSource *ds;

	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "GAZP", "192.168.9.156", 5000, this);
	addDataSource(ds, 0);

	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "SBER", "192.168.9.156", 5000, this);
	ChartWidget* cw = addDataSource(ds, 1);

	ds = new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt", this);
	ChartWidget* cw = addDataSource(ds);
*/

	DataSource ds = DataSourceFactory::instance().get(QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"), {{"class","TQBR"}, {"code","SBER"}, {"interval", IntervalD1} });
	if(ds)
	{
		ChartWidget* cw = addDataSource(ds);

		QSettings settings;
		BIndicator *indicator;
		//indicator = new CandleAdapterIndicator(ds, CandleAdapterIndicator::MOpenClose, this);
		indicator = new LuaLineIndicator(settings.value("LuaDir", "../lua").toString() + "/indicators/MA.lua", ds, this);
		cw->addIndicator(indicator);
	}

	mScrollBar = new QScrollBar(Qt::Horizontal);
	mScrollBar->setEnabled(false);
	connect(mScrollBar, &QAbstractSlider::actionTriggered, [this](int){
		setScrollValue(mScrollBar->sliderPosition());
	});
	mLayout->addWidget(mScrollBar);

	adjustScroll();
}

ChartWidget* ChartWindow::addDataSource(DataSource dataSource, int widgetNum)
{
	ChartWidget* widget = nullptr;

	if(widgetNum >= mChartWidgets.size())
	{

		widgetNum = mChartWidgets.size();
		widget = new ChartWidget;
		mSceneLayout->addItem(widget, widgetNum, 0);
		mChartWidgets << widget;
	}
	else
	{	widget = mChartWidgets[widgetNum];	}

	connect(widget, SIGNAL(candlesAppended(DataSource,int)), SLOT(onCandlesAppend(DataSource, int)));
	widget->addDataSource(dataSource);

	return widget;
}

qint64 ChartWindow::timeFrame() const
{
	qint64 rv(-1);
	if(mChartWidgets.size()>0)
	{
		qreal width = mChartWidgets[0]->plotArea().width();
		qreal candleCount = width/(mCandleWidth+1);
		rv = secsInInterval(mTimeInterval)*candleCount;
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
		qint64 max = range.second.toSecsSinceEpoch()+secsInInterval(mTimeInterval);
		qint64 min = range.first.toSecsSinceEpoch()+timeFrame()/2-secsInInterval(mTimeInterval);
		qint64 step = secsInInterval(mTimeInterval);

		// нужно выровнять разницу между минимумом и максимумом по минимальному шагу
		qint64 rem = (max-min+1)%step;
		if(rem>0)
		{	max+=step-rem;	}

		mScrollBar->setRange(rescaleInt64(min), rescaleInt64(max));

		mScrollBar->setSingleStep(rescaleInt64(step));
		mScrollBar->setPageStep(rescaleInt64(timeFrame()));
		if(! mScrollBar->isEnabled())
		{
			mScrollBar->setEnabled(true);
			mScrollBar->setSliderPosition(mScrollBar->maximum());
		}
	}
	else
	{
		mScrollBar->setRange(0,0);
		mScrollBar->setEnabled(false);
	}
}

void ChartWindow::setScrollValue(int value)
{
	QDateTime max = QDateTime::fromSecsSinceEpoch(value);
	QDateTime min = QDateTime::fromSecsSinceEpoch(value - timeFrame());
	setViewTimeRange({min, max});
}

void ChartWindow::onCandlesAppend(DataSource, int)
{
	bool need2Last = false;
	if(mScrollBar->sliderPosition() == mScrollBar->maximum())
	{	need2Last = true;	}
	adjustScroll();
	if(need2Last)
	{
		mScrollBar->blockSignals(true);
		setScrollValue(mScrollBar->maximum());
		mScrollBar->setSliderPosition(mScrollBar->maximum());
		mScrollBar->blockSignals(false);
	}
}

void ChartWindow::adjustGraphicsScene()
{
	mGraphicsWidget->setGeometry(mGraphicsView->childrenRect());
	mGraphicsView->scene()->setSceneRect(mGraphicsView->childrenRect());
}

int ChartWindow::rescaleInt64(qint64 value) const
{
	//static_cast<int>(value * std::numeric_limits<int>::max() / std::numeric_limits<qint64>::max());
	return static_cast<int>(value);
}

void ChartWindow::resizeEvent(QResizeEvent *event)
{
	adjustGraphicsScene();
	adjustScroll();
	mScrollBar->setPageStep(rescaleInt64(timeFrame()));
	setScrollValue(mScrollBar->sliderPosition());
	QWidget::resizeEvent(event);
}

void ChartWindow::showEvent(QShowEvent *event)
{
	adjustGraphicsScene();
	adjustScroll();
	mScrollBar->setPageStep(rescaleInt64(timeFrame()));
	setScrollValue(mScrollBar->sliderPosition());
	QWidget::showEvent(event);
}
