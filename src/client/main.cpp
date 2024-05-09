#include <std_include.hpp>

#include "game/game.hpp"
#include "havok/hks_api.hpp"
#include "loader/component_loader.hpp"

extern "C"
{
	int __declspec(dllexport) init(lua::lua_State* L)
	{
		game::minlog.WriteLine("ArchiT7OverchargedLibrary initiating");

		const lua::luaL_Reg ArchiT7OverchargedLibrary[] =
		{
			{nullptr, nullptr},
		};
		hks::hksI_openlib(L, "ArchiT7OverchargedLibrary", ArchiT7OverchargedLibrary, 0, 1);

		if (!component_loader::post_start())
		{
			game::Com_Error_("", 0, 0x200u, "Error while loading ArchiT7OverchargedLibrary components");
			game::minlog.WriteLine("Error while loading ArchiT7OverchargedLibrary components");
			return 0;
		}

		game::minlog.WriteLine("ArchiT7Overcharged initiated");

		game::LoadDvarHashMap();
		return 1;
	}
}