#include "chartwidget.h"

#include <QBarCategoryAxis>
#include <QChartView>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QVBoxLayout>
#include <QValueAxis>

#include "series/bseriesex.h"

using namespace QtCharts;

ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	mChartView = new QChartView(this);
	//mChartView->setRenderHint(QPainter::Antialiasing);
	layout->addWidget(mChartView);

	mChart = new QChart();
	mChart->setAnimationOptions(QChart::SeriesAnimations);



	mChartView->setChart(mChart);
	mValueAxis = new QValueAxis;
	mTimeAxis = new QDateTimeAxis;
	mChart->addAxis(mValueAxis, Qt::AlignLeft);
	mChart->addAxis(mTimeAxis, Qt::AlignBottom);

	//mValueAxis->setRange(140, 260);
	mTimeAxis->setRange(QDateTime::currentDateTime().addDays(-200), QDateTime::currentDateTime());
}

void ChartWidget::addSeries(BSeriesEx *series)
{
	QAbstractSeries* s = series->abstractSeries();
	mChart->addSeries(s);
	s->attachAxis(mValueAxis);
	s->attachAxis(mTimeAxis);
	connect(s, SIGNAL(countChanged()), SLOT(onCountChanged()));


	mChart->legend()->setVisible(true);
	mChart->legend()->setAlignment(Qt::AlignBottom);
}

void ChartWidget::onCountChanged()
{
	const QAbstractSeries* s = static_cast<QAbstractSeries*>(sender());
	const BSeriesEx* ex = BSeriesEx::interface(s);
	auto r = ex->valueRange(QDateTime::currentDateTime().addDays(-200), QDateTime::currentDateTime());
	if(!(qIsNaN(r.first) || qIsNaN(r.second)))
	{	mValueAxis->setRange(r.first*0.99, r.second*1.01);	}
}
