#pragma once

#include <indicators/blineindicator.h>

struct lua_State;

class LuaLineIndicator : public BLineIndicator
{
public:
	LuaLineIndicator(const QString& fileName, DataSource dataSource, QObject *parent=nullptr);
	virtual ~LuaLineIndicator() override;
private:
	lua_State* mState;
	int mLineCount;
	QString mFileName;

	static int candleC(lua_State* L);
	static int candleO(lua_State* L);
	static int candleL(lua_State* L);
	static int candleH(lua_State* L);
	static int candleV(lua_State* L);

	// BLineIndicator interface
public:
	virtual bool isSame(const SerialT &other) const override;
		virtual SerialInfo info() const override;
protected:
	virtual Point candle2point(int index) const override;
};
