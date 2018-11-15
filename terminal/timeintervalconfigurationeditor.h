#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class TimeIntervalConfigurationEditor;
}

class TimeIntervalConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	TimeIntervalConfigurationEditor(const QModelIndex &index, QWidget *parent=nullptr);
	~TimeIntervalConfigurationEditor();
private:
	Ui::TimeIntervalConfigurationEditor* ui;
};
