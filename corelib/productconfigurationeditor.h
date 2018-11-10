#pragma once

#include "configuration.h"

#include <QWidget>

class ProductConfigurationEditor : public QWidget
{
	Q_OBJECT
public:
	ProductConfigurationEditor(Configuration* configuration, QWidget *parent=nullptr);
	virtual ~ProductConfigurationEditor(){}
protected:
	Configuration* configuration() const;
private:
	Configuration* mConfiguration;

};
