#pragma once

#include <confeditormodule.h>

namespace Ui
{
class TimeIntervalConfEditor;
}

class TimeIntervalConf;

class TimeIntervalConfEditor final : public ConfEditorModule
{
	Q_OBJECT
public:
	TimeIntervalConfEditor(TimeIntervalConf &conf, QWidget *parent=nullptr);
	virtual ~TimeIntervalConfEditor();
private:
	Ui::TimeIntervalConfEditor* ui;
};
