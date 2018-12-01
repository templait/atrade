#include "filedatasourceconf.h"

FileDataSourceConf::FileDataSourceConf(const ProductID &id, const QString &className, const QString &code)
    : DataSourceConf("File Datasource")
    , mClassName(className)
    , mCode(code)
{
	setProductID(id);
}

const QString &FileDataSourceConf::className() const
{
	return mClassName;
}

void FileDataSourceConf::setClassName(const QString &className)
{
	mClassName=className;
}

const QString &FileDataSourceConf::code() const
{
	return mCode;
}

void FileDataSourceConf::setCode(const QString &code)
{
	mCode=code;
}

bool FileDataSourceConf::isSame(const BConf &other) const
{
	bool rv = false;
	if(const FileDataSourceConf* dsConf = dynamic_cast<const FileDataSourceConf*>(&other))
	{
		rv = BConf::isSame(other)
		        && mClassName==dsConf->mClassName
		        && mCode==dsConf->mCode;
	}
	return rv;
}

BConf *FileDataSourceConf::clone() const
{
	return new FileDataSourceConf(*this);
}
