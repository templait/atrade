#pragma once

#include <QMdiArea>

class QSettings;

class MDIArea : public QMdiArea
{
	Q_OBJECT
public:
	MDIArea(QWidget* parent=nullptr);
	void saveWindowState(QSettings& settings);
	void loadWindowState(QSettings& settings);
};
