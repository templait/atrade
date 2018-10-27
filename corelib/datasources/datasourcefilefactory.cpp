#include "datasourcefilefactory.h"
#include "datasourcefile.h"

#include <log.h>
#include <tools.h>

#include <QDir>	// killme
#include <QSettings>

DataSourceFileFactory::DataSourceFileFactory()
    : DataSourceFactory::Unit(
            QString("File datasources"),
            QUuid("cbc78f17-d904-48b3-b31b-ab010bf4dd20"),
            {
                {QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"), QString("File datasoure")}
            }
                              )
{

}

BDataSource *DataSourceFileFactory::create(const DataSourceFactory::ProductID & id, const QVariantMap & settings) const
{
	BDataSource* rv=0;

	if(id == "ab38fe10-d502-11e8-b568-0800200c9a66")
	{
		QSettings appSettings;
		QString path = appSettings.value("DataSourceFileDir", "../DataSourceFile").toString();
		rv = new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt");
	}
	else
	{
		Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(id.toString()));
	}

	return rv;
}
