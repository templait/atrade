#pragma once

#include <indicators/blineindicator.h>

struct lua_State;

class LuaLineIndicator : public BLineIndicator
{
public:
	LuaLineIndicator(const QString& fileName, DataSource dataSource, QObject *parent=0);
	virtual ~LuaLineIndicator();
private:
	lua_State* mState;
	int mLineCount;

	static int candleC(lua_State* L);

	// BLineIndicator interface
protected:
	virtual Point candle2point(int index) const override;
};
