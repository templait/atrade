#include "lualineindicator.h"
#include <log.h>
#include <tools.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

LuaLineIndicator::LuaLineIndicator(const QString &fileName, DataSource dataSource, QObject* parent) : BLineIndicator(dataSource, parent)
{
	mState = luaL_newstate();
	if(luaL_loadfile(mState, fileName.toLocal8Bit()))
	{
		Log::error(QString("%1:%2").arg(__CLASS_NAME__).arg(lua_tostring(mState,-1)));
	}
	else
	{
		luaL_openlibs(mState);

		lua_pushlightuserdata(mState, this);
		lua_pushcclosure(mState, candleC, 1);
		lua_setglobal(mState, "C");

		lua_pushlightuserdata(mState, this);
		lua_pushcclosure(mState, candleO, 1);
		lua_setglobal(mState, "O");

		lua_pushlightuserdata(mState, this);
		lua_pushcclosure(mState, candleL, 1);
		lua_setglobal(mState, "L");

		lua_pushlightuserdata(mState, this);
		lua_pushcclosure(mState, candleH, 1);
		lua_setglobal(mState, "H");

		lua_pushlightuserdata(mState, this);
		lua_pushcclosure(mState, candleV, 1);
		lua_setglobal(mState, "V");

		if(lua_pcall(mState, 0, 0, 0) != 0)
		{
			Log::error(QString("%1.Failed to run script: %1\n").arg(__CLASS_NAME__).arg(lua_tostring(mState, -1)));
		}
		else
		{
			lua_getglobal(mState,"Init");
			if(lua_isfunction(mState, -1) )
			{
				if(!lua_pcall(mState,0,1,0))
				{
					if (!lua_isnil(mState, -1))
					{
						mLineCount = lua_tonumber(mState,-1);
						lua_pop(mState,1);
					}
				}
				else
				{	Log::error(lua_tostring(mState,-1));	}
			}
		}
	}
}

LuaLineIndicator::~LuaLineIndicator()
{
	lua_close(mState);
}

int LuaLineIndicator::candleC(lua_State *L)
{
	const void* pt = lua_topointer(L, lua_upvalueindex(1));
	const LuaLineIndicator* indicator = reinterpret_cast<const LuaLineIndicator*>(pt);
	double index = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushnumber(L, indicator->dataSource()->at(index)->close());
	return 1;
}

int LuaLineIndicator::candleO(lua_State *L)
{
	const void* pt = lua_topointer(L, lua_upvalueindex(1));
	const LuaLineIndicator* indicator = reinterpret_cast<const LuaLineIndicator*>(pt);
	double index = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushnumber(L, indicator->dataSource()->at(index)->open());
	return 1;
}

int LuaLineIndicator::candleL(lua_State *L)
{
	const void* pt = lua_topointer(L, lua_upvalueindex(1));
	const LuaLineIndicator* indicator = reinterpret_cast<const LuaLineIndicator*>(pt);
	double index = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushnumber(L, indicator->dataSource()->at(index)->low());
	return 1;
}

int LuaLineIndicator::candleH(lua_State *L)
{
	const void* pt = lua_topointer(L, lua_upvalueindex(1));
	const LuaLineIndicator* indicator = reinterpret_cast<const LuaLineIndicator*>(pt);
	double index = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushnumber(L, indicator->dataSource()->at(index)->high());
	return 1;
}

int LuaLineIndicator::candleV(lua_State *L)
{
	const void* pt = lua_topointer(L, lua_upvalueindex(1));
	const LuaLineIndicator* indicator = reinterpret_cast<const LuaLineIndicator*>(pt);
	double index = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushnumber(L, indicator->dataSource()->at(index)->volume());
	return 1;
}

Point LuaLineIndicator::candle2point(int index) const
{
	Point rv;
	lua_getglobal(mState,"OnCalculate");
	if(lua_isfunction(mState, -1) )
	{
		lua_pushnumber(mState, index);
		if(!lua_pcall(mState,1,mLineCount,0))
		{
			//if (!lua_isnil(mState, -1))
			for(int i=0;i<mLineCount;i++)
			{
				if(lua_isnumber(mState, -1))
				{
					double val = lua_tonumber(mState,-1);
					rv = Point(val, dataSource()->at(index)->time());
				}
				lua_pop(mState,1);
			}
		}
		else
		{	Log::error(lua_tostring(mState,-1));	}
	}

	return rv;
}
