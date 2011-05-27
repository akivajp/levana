/////////////////////////////////////////////////////////////////////////////
// Name:        src/main.cpp
// Purpose:     entry point for standalone executable running lua script
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/entry.hpp"
#include "lev/app.hpp"

int main(int argc, char **argv)
{
  using namespace lev;

  const int len = 3;
  const char *entry[] = {"entry.lc", "entry.lua", "entry.txt"};
  bool done_something = false;

  // initializing lua statement
  lua_State *L = lua_open();
  // open basic (and critical) libs
  lua_pushcfunction(L, &luaopen_base);
  lua_pushstring(L, "");
  lua_call(L, 1, 0);
  lua_pushcfunction(L, &luaopen_package);
  lua_pushstring(L, LUA_LOADLIBNAME);
  lua_call(L, 1, 0);

  // lev library registration
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  lua_pushcfunction(L, &luaopen_debug);
  lua_setfield(L, -2, "debug");
  lua_pushcfunction(L, &luaopen_io);
  lua_setfield(L, -2, "io");
  lua_pushcfunction(L, &luaopen_lev);
  lua_setfield(L, -2, "lev");
  lua_pushcfunction(L, &luaopen_os);
  lua_setfield(L, -2, "os");
  lua_pushcfunction(L, &luaopen_string);
  lua_setfield(L, -2, "string");
  lua_pop(L, 2);

  // lev entry
  application::entry(L, argc, argv);

  for (int i = 1; i < argc; i++)
  {
    switch(argv[i][0])
    {
      case '-':
        if (argv[i][1] == 'e' && argc > i + 1)
        {
          i++;
          if (luaL_dostring(L, argv[i]))
          {
            wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
            exit(-1);
          }
          done_something = true;
        }
        break;
      default:
        if (luaL_dofile(L, argv[i]))
        {
          wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
          exit(-1);
        }
        done_something = true;
        break;
    }
  }

  if (done_something == false)
  {
    // nothing was done
    // run entry program
    for (int i = 0; i < len; i++)
    {
      if (access(entry[i], 0) < 0) { continue; }
      if (luaL_dofile(L, entry[i]))
      {
        wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
        exit(-1);
      }
      return 0;
    }
    wxMessageBox(_("Usage: create \"entry.txt\" file and put in the same directory with the program"), _("About usage"));
  }

  return 0;
}

