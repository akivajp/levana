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
#include "register.hpp"

#include <string>

int main(int argc, char **argv)
{
  using namespace luabind;
  using namespace lev;

  const int len = 3;
  const char *entry[] = {"entry.lc", "entry.lua", "entry.txt"};
  bool done_something = false;

  // initializing lua statement
  lua_State *L = lua_open();
  // open basic (and critical) libs
  lua_pushcfunction(L, &luaopen_base);
  lua_pushstring(L, LUA_COLIBNAME);
  lua_call(L, 1, 0);
  lua_pushcfunction(L, &luaopen_package);
  lua_pushstring(L, LUA_LOADLIBNAME);
  lua_call(L, 1, 0);

  // lev library registration
  register_to(globals(L)["package"]["preload"], LUA_DBLIBNAME, &luaopen_debug);
  register_to(globals(L)["package"]["preload"], LUA_IOLIBNAME, &luaopen_io);
  register_to(globals(L)["package"]["preload"], LUA_MATHLIBNAME, &luaopen_math);
  register_to(globals(L)["package"]["preload"], LUA_OSLIBNAME, &luaopen_os);
  register_to(globals(L)["package"]["preload"], LUA_STRLIBNAME, &luaopen_string);
  register_to(globals(L)["package"]["preload"], LUA_TABLIBNAME, &luaopen_table);
  lev::set_preloaders(L);

  // lev entry
  application::entry(L, argc, argv);

  for (int i = 1; i < argc; i++)
  {
    switch(argv[i][0])
    {
      case '-':
        if (argv[i][1] == 'e' && argc > i + 1)
        {
          // -e stat
          i++;
          if (luaL_dostring(L, argv[i]))
          {
            wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
            return -1;
          }
          done_something = true;
        }
        else if (argv[i][1] == '\0')
        {
          // - (stdin)
          typedef std::istreambuf_iterator<char> iterator;
          std::string input(iterator(std::cin), iterator());
          if (luaL_dostring(L, input.c_str()))
          {
            wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
            return -1;
          }
          done_something = true;
        }
        break;
      default:
        if (done_something) { break; }
        globals(L)["arg"] = newtable(L);
        object arg = globals(L)["arg"];
        arg[-1] = std::string(argv[0]);
        arg[0] =  std::string(argv[i]);
        for (int j = 1; j <= argc - i - 1; j++)
        {
          arg[j] = std::string(argv[i + j]);
        }
        if (wxFileName::DirExists(wxString(argv[i], wxConvUTF8)))
        {
          // argv[i] is directory
          // run entry program in argv[i] directory
          for (int j = 0; j < len; j++)
          {
            std::string filename = argv[i];
            filename = filename + "/" + entry[j];
            if (access(filename.c_str(), 0) < 0) { continue; }
            if (luaL_dofile(L, filename.c_str()))
            {
              wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
              return -1;
            }
            return 0;
          }
          wxString msg = _("Usage: create \"entry.txt\" file and put in \"") + wxString(argv[i], wxConvUTF8) + _("\" directory");
          wxMessageBox(msg, _("About usage"));
          done_something = true;
        }
        else
        {
          // argv[i] is file
          if (luaL_dofile(L, argv[i]))
          {
            wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
            return -1;
          }
          done_something = true;
        }
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
        return -1;
      }
      return 0;
    }
    wxMessageBox(_("Usage: create \"entry.txt\" file and put in the same directory with the program"), _("About usage"));
  }

  lua_close(L);

  return 0;
}

