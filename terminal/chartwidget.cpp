#include "chartwidget.h"

#include <QBarCategoryAxis>
#include <QChartView>
#include <QVBoxLayout>
#include <QValueAxis>

ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	mChartView = new QtCharts::QChartView(this);
	//mChartView->setRenderHint(QPainter::Antialiasing);
	layout->addWidget(mChartView);

	mChart = new QtCharts::QChart();
	mChart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);



	mChartView->setChart(mChart);
}

void ChartWidget::addSeries(QtCharts::QAbstractSeries *series)
{
	mChart->addSeries(series);
	connect(series, SIGNAL(countChanged()), SLOT(onCountChanged()));


	mChart->legend()->setVisible(true);
	mChart->legend()->setAlignment(Qt::AlignBottom);
}

void ChartWidget::onCountChanged()
{
	mChart->createDefaultAxes();

	QtCharts::QValueAxis *axisY = qobject_cast<QtCharts::QValueAxis *>(mChart->axes(Qt::Vertical).at(0));
	axisY->setMax(260);
	axisY->setMin(20);

	//QtCharts::QBarCategoryAxis *axisX = qobject_cast<QtCharts::QBarCategoryAxis *>(mChart->axes(Qt::Horizontal).at(0));
	//axisX->setRange(axisX->at(0), axisX->at(100));
	//qDebug() << "min: " << axisX->min() << "max: " << axisX->max();
}
