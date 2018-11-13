#pragma once

#include <QMdiArea>

class QSettings;

class MDIArea : public QMdiArea
{
	Q_OBJECT
public:
	using QMdiArea::QMdiArea;
	void saveWindowState(QSettings& settings);
	void loadWindowState(QSettings& settings);
};
