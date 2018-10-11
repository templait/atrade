#include "chartwidget.h"

#include <math.h>

#include <series/bseriesex.h>

#include <QChart>
#include <QDateTimeAxis>
#include <QGraphicsLinearLayout>
#include <QValueAxis>


using namespace QtCharts;

ChartWidget::ChartWidget(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);

	mChart = new QChart;
	layout->addItem(mChart);

	mValueAxis = new QValueAxis;
	mTimeAxis = new QDateTimeAxis;
	//mTimeAxis->setFormat("dd");
	connect(mTimeAxis, SIGNAL(rangeChanged(QDateTime,QDateTime)), SLOT(onTimeRangeChanged(QDateTime,QDateTime)));
	mChart->addAxis(mValueAxis, Qt::AlignLeft);
	mChart->addAxis(mTimeAxis, Qt::AlignBottom);
}

void ChartWidget::addSeries(BSeriesEx *series)
{
	QAbstractSeries* s = series->abstractSeries();
	mChart->addSeries(s);
	s->attachAxis(mValueAxis);
	s->attachAxis(mTimeAxis);
	adjustValueAxis();
	connect(s, SIGNAL(countChanged()), SLOT(onCountChanged()));

	//mChart->legend()->setVisible(true);
	mChart->legend()->setAlignment(Qt::AlignBottom);
}

void ChartWidget::adjustValueAxis()
{
	qreal min = NAN;
	qreal max = NAN;
	for(const QAbstractSeries* s : mChart->series())
	{
		const BSeriesEx* ex = BSeriesEx::interface(s);
		auto seriesRange = ex->valueRange(viewTimeRange().first, viewTimeRange().second);
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

TimeRange ChartWidget::viewTimeRange() const
{
	return {mTimeAxis->min(), mTimeAxis->max()};
}
#include<QtDebug>
void ChartWidget::setViewTimeRange(const TimeRange & range)
{
	mTimeAxis->setRange(range.first.addMSecs(-1), range.second.addMSecs(1)); // Полный бред, но без добавления этих миллисекунд ось не отображается
}

TimeRange ChartWidget::seriesTimeRange() const
{
	TimeRange rv;
	for(const QAbstractSeries* s : mChart->series())
	{
		const BSeriesEx* ex = BSeriesEx::interface(s);
		QPair<QDateTime, QDateTime> seriesRange = ex->timeRange();
		if(rv.first.isNull() || rv.first > seriesRange.first)
		{	rv.first = seriesRange.first;	}
		if(rv.second.isNull() || rv.second < seriesRange.second)
		{	rv.second = seriesRange.second;	}
	}

	return rv;
}

QRectF ChartWidget::plotArea() const
{
	return mChart->plotArea();
}

void ChartWidget::onCountChanged()
{
	adjustValueAxis();
}

void ChartWidget::onTimeRangeChanged(QDateTime min, QDateTime max)
{
	for(QAbstractSeries* s : mChart->series())
	{
		BSeriesEx* ex = BSeriesEx::interface(s);
		ex->setViewTimeRange({min, max});
	}
}
