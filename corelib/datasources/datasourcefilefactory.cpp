#include "datasourcefilefactory.h"
#include "datasourcefile.h"

#include <QDir>	// killme

DataSourceFileFactory::DataSourceFileFactory()
    : DataSourceFactory::Unit(
            QString("File datasources"),
            QUuid("cbc78f17-d904-48b3-b31b-ab010bf4dd20"),
            {
                {QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"), QString("File datasourc")}
            }
                              )
{

}

BDataSource *DataSourceFileFactory::create(const DataSourceFactory::ProductID &/*id*/, const QVariantMap &/*setiings*/) const
{
	return new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt");
}
