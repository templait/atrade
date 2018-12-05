#pragma once

#include <confeditormodule.h>

namespace Ui
{
class TitleConfEditor;
}

class TitleConfEditor : public ConfEditorModule
{
	Q_OBJECT
public:
	TitleConfEditor(BConf &conf, QWidget *parent=nullptr);
	~TitleConfEditor();
private:
	Ui::TitleConfEditor* ui;

	void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &);
};
