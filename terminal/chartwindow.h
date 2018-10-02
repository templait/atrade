#pragma once

#include "bdatasource.h"

#include <QWidget>

class ChartWindow : public QWidget
{
public:
	ChartWindow(QWidget* parent=0);
	~ChartWindow();
private:
	BDataSource *mDataSource;
};
