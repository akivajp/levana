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
      static const char *get_path(lua_State *L);
      static luabind::object require(const char *package);
      static bool set_path(lua_State *L, const char *path);
  };

}

#endif // _PACKAGE_HPP

