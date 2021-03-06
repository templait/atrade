#include "mdiarea.h"
#include "chartwindow.h"
#include <QMdiSubWindow>
#include <QSettings>


MDIArea::MDIArea(QWidget *parent) : QMdiArea(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MDIArea::saveWindowState(QSettings& settings)
{
	settings.setValue("WindowTitle", windowTitle());
	settings.beginWriteArray(objectName());
	QList<QMdiSubWindow *> subs = subWindowList();
	int i=0;
	for(const QMdiSubWindow * sub : subs)
	{
		settings.setArrayIndex(i++);
		if(const ChartWindow* cw = qobject_cast<const ChartWindow*>(sub->widget()))
		{
			settings.setValue("WindowType", "ChartWindow");
			settings.setValue("Geometry", sub->geometry());
			cw->saveConfiguration(settings);
		}
	}
	settings.endArray();
}

void MDIArea::loadWindowState(QSettings& settings)
{
	setWindowTitle(settings.value("WindowTitle", objectName()).toString());
	int chartWinCount = settings.beginReadArray(objectName());
	for(int i=0; i<chartWinCount; i++)
	{
		settings.setArrayIndex(i);
		QString windowType = settings.value("WindowType").toString();
		if(windowType == "ChartWindow")
		{
			ChartWindow* cw = new ChartWindow;
			QMdiSubWindow* sub = addSubWindow(cw);
			sub->setGeometry(settings.value("Geometry").toRect());
			cw->show();
			cw->loadConfiguration(settings);
		}
	}
	settings.endArray();
}
