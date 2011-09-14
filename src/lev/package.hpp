#ifndef _PACKAGE_HPP
#define _PACKAGE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/package.hpp
// Purpose:     header for package management
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     09/01/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "app.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_package(lua_State *L);
}

namespace lev
{

  class package
  {
    public:
      static const char *get_archive_dir(lua_State *L);
      static const char *get_path(lua_State *L);
      static int require_l(lua_State *L);
      static file_path* resolve(const std::string &base_path, const std::string &file);
      static int resolve_l(lua_State *L);
      static bool set_archive_dir(lua_State *L, const std::string &archive_dir);
      static bool set_path(lua_State *L, const std::string &path);
  };

}

#endif // _PACKAGE_HPP

