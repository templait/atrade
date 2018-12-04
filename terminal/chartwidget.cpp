#include "chartwidget.h"
#include "chartconf.h"

#include <math.h>
#include <timeintervalconf.h>

#include "series/datasourceseries.h"
#include "series/lineindicatorseries.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QGraphicsLinearLayout>
#include <QValueAxis>
#include <configuration.h>
#include <datasources/datasource.h>
#include <indicators/indicator.h>

using namespace QtCharts;

ChartWidget::ChartWidget(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);

	mChart = new QChart;
	//mChart->setAnimationOptions(QChart::SeriesAnimations);
	layout->addItem(mChart);

	mValueAxis = new QValueAxis;
	mTimeAxis = new QDateTimeAxis;
	connect(mTimeAxis, SIGNAL(rangeChanged(QDateTime,QDateTime)), SLOT(onTimeRangeChanged(QDateTime,QDateTime)));
	mChart->addAxis(mValueAxis, Qt::AlignLeft);
	mChart->addAxis(mTimeAxis, Qt::AlignBottom);
	mChart->legend()->hide();
}

ChartWidget::ChartWidget(const ChartConf &chartConf, QGraphicsItem *parent)
    : ChartWidget(parent)
{
	for(int i=0; i<chartConf.childrenCount(); i++) // ищим источники
	{
		if(const DataSourceConf* dsConf = dynamic_cast<const DataSourceConf*>(chartConf.childAt(i)))
		{
			if(DataSourceFactory::instance().hasProduct(dsConf->productID()))
			{	addDataSource(*dsConf);	}
			else
			{	Log::warning(QString("%1. Can't create datasource. ID: %2").arg(__CLASS_NAME__).arg(dsConf->productID().toString()));	}
			for(int i=0; i<dsConf->childrenCount(); i++) // ищим индикаторы для источников
			{
				if(const IndicatorConf* indicatorConf = dynamic_cast<const IndicatorConf*>(dsConf->childAt(i)))
				{
					if(IndicatorFactory::instance().hasProduct(indicatorConf->productID()))
					{	addIndicator(*indicatorConf);	}
					else
					{	Log::warning(QString("%1. Can't create indicator. ID: %2").arg(__CLASS_NAME__).arg(dsConf->productID().toString()));	}
				}
			}
		}
	}
}

void ChartWidget::addDataSource(const DataSourceConf &dataSource)
{
	DataSource ds = DataSourceFactory::instance().product(dataSource);
	if(!ds.isNull())
	{
		DataSourceSeries *dss = new DataSourceSeries(mChart, ds, this);
		/*
		QColor increasingColor = dataSource.containsChild("increasingColor") ? dataSource["increasingColor"].value().value<QColor>() : QColor();
		QColor decreasingColor = dataSource.containsChild("decreasingColor") ? dataSource["decreasingColor"].value().value<QColor>() : QColor();
		QColor penColor = dataSource.containsChild("penColor") ? dataSource["penColor"].value().value<QColor>() : QColor();
		dss->setAppearance(increasingColor, decreasingColor, penColor);
		*/
		dss->setName(dataSource.title());
		connect(dss, SIGNAL(candlesAdppended(int)), SLOT(onCandlesAppended(int)));
		mSeries << dss;
	}
}

void ChartWidget::addIndicator(const IndicatorConf &indicator)
{
	Indicator ind = IndicatorFactory::instance().product(indicator);
	LineIndicatorSeries *is = new LineIndicatorSeries(mChart, ind, this);
	mSeries << is;
}

void ChartWidget::adjustValueAxis()
{
	qreal min = std::numeric_limits<double>::quiet_NaN();
	qreal max = std::numeric_limits<double>::quiet_NaN();
	for(const BSeries* series  : mSeries)
	{
		ValueRange seriesRange = series->valueRange();
		if(qIsNaN(min) || min > seriesRange.first)
		{	min = seriesRange.first;	}
		if(qIsNaN(max) || max < seriesRange.second)
		{	max = seriesRange.second;	}
	}
	if(! (qIsNaN(min) || qIsNaN(max)) )
	{	mValueAxis->setRange(min, max);	}
}

QValueAxis *ChartWidget::valueAxis()
{
	return mValueAxis;
}

QDateTimeAxis *ChartWidget::timeAxis()
{
	return mTimeAxis;
}

void ChartWidget::onCandlesAppended(int count)
{
	adjustValueAxis();
	DataSourceSeries* series = qobject_cast<DataSourceSeries*>(sender());
	Q_ASSERT(series);
	emit candlesAppended(series->dataSource(), count);
}

TimeRange ChartWidget::viewTimeRange() const
{
	return {mTimeAxis->min(), mTimeAxis->max()};
}

void ChartWidget::setViewTimeRange(const TimeRange & range)
{
	mTimeAxis->setRange(range.first.addMSecs(-1), range.second.addMSecs(1)); // Полный бред, но без добавления этих миллисекунд ось не отображается
	adjustValueAxis();
}

TimeRange ChartWidget::timeRange() const
{
	TimeRange rv;
	for(const BSeries* series  : mSeries)
	{
		TimeRange seriesRange = series->timeRange();
		if(seriesRange.first.isValid())
		{
			if(rv.first.isNull() || rv.first > seriesRange.first)
			{	rv.first = seriesRange.first;	}
			if(rv.second.isNull() || rv.second < seriesRange.second)
			{	rv.second = seriesRange.second;	}
		}
	}

	return rv;
}

QRectF ChartWidget::plotArea() const
{
	return mChart->plotArea();
}

void ChartWidget::onTimeRangeChanged(QDateTime min, QDateTime max)
{
	for(BSeries* series  : mSeries)
	{
		series->setViewTimeRange({min, max});
	}
}
