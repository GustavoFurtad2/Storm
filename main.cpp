#include <string>
#include <iostream>

#include "./lua/include/lua.hpp"

#ifdef _WIN32
#pragma comment(lib, "./lua/liblua54.a")
#endif

bool checkLua(lua_State* L, int r) {

	if (r == LUA_OK) {
		return true;
	}

	std::string errorMessage = lua_tostring(L, -1);
	std::cout << errorMessage << "\n";
	
	return false;
}

int main() {

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	if (checkLua(L, luaL_dofile(L, "main.lua"))) {

	}
	else {

		std::string errorMessage = lua_tostring(L, -1);
		std::cout << errorMessage << "\n";
	}

	lua_close(L);
	return 0;
}