#include "datasourcefilefactory.h"
#include "datasourcefile.h"

#include <log.h>
#include <tools.h>

#include <QDir>	// killme
#include <QSettings>

DataSourceFileFactory::DataSourceFileFactory()
	: DataSourceFactory::Unit(QString("File datasoure"), QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"))
{

}

BDataSource *DataSourceFileFactory::create(const DataSourceFactory::ProductID & id, const Configuration &settings) const
{
	BDataSource* rv=nullptr;

	if(id == "ab38fe10-d502-11e8-b568-0800200c9a66")
	{
		QSettings appSettings;
		QString path = QString("%1/%2/%3/%4.txt")
		        .arg(appSettings.value("DataSourceFileDir", "../DataSourceFile").toString())
				.arg(settings["class"].value().toString())
				.arg(settings["code"].value().toString())
				.arg(intervalToString(static_cast<ETimeInterval>(settings["interval"].value().toInt())));
		//rv = new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt");
		rv = new DataSourceFile(path);
	}
	else
	{
		Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(id.toString()));
	}

	return rv;
}
