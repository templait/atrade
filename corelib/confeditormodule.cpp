#include "confeditormodule.h"

ConfEditorModule::ConfEditorModule(BConf &conf, QWidget *parent)
	:QWidget(parent), mConf(conf)
{

}

const BConf &ConfEditorModule::conf() const
{
	return mConf;
}

BConf &ConfEditorModule::conf()
{
	return mConf;
}
