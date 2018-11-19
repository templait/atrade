#include <datasources/bdatasource.h>
#include <datasources/quikdatasource/quikdatasource.h> // killme
#include <functional>

typedef std::function<void(QList<const Candle*>)> ScrollFunction;

void scrollDataSources(const QList<const BDataSource*> dsList,  ScrollFunction fn)
{
	QList<int> idx;
	QList<const Candle*> list;
	for(int i=0;i<dsList.size();i++)
	{
		idx << 0;
		if(dsList[i]->size())
		{	list << dsList[i]->at(0);	}
	}

	auto it = std::min_element(list.begin(), list.end(), [](const Candle* first, const Candle* second){return first->time() < second->time();});

}

int main(int, char**)
{
}


