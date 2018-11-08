#pragma once

#include "configuration.h"

#include <QWidget>

class ProductConfigurationEditor : public QWidget
{
	Q_OBJECT
public:
	ProductConfigurationEditor(const Configuration& configuration, QWidget *parent=nullptr);
	virtual ~ProductConfigurationEditor(){}
	const Configuration& configuration() const;
private:
	Configuration mConfiguration;

};
