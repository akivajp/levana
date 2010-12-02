#include "prec.h"
#include "levana/entry.hpp"

/////////////////////////////////////////////////////////////////////////////
// Name:        src/main.cpp
// Purpose:     entry point for standalone executable running lua script
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    const int len = 3;
    const char *entry[] = {"entry.lc", "entry.lua", "entry.txt"};
    lua_State *L = lua_open();
    luaL_openlibs(L);

    // levana library registration
    luabind::object preload = luabind::globals(L)["package"]["preload"];
    preload.push(L);
    lua_pushcfunction(L, &luaopen_levana);
    lua_setfield(L, -2, "levana");
    lua_gettop(L);
    lua_gettop(L);

    // run entry program
    for (int i = 0; i < len; i++)
    {
      if (access(entry[i], 0) < 0) { continue; }
      if (luaL_dofile(L, entry[i]))
      {
        printf("%s\n", lua_tostring(L, -1));
      }
    }
  }
  else
  {
    // argc >= 2
  }
  return 0;
}

