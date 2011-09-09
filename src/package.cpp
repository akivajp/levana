/////////////////////////////////////////////////////////////////////////////
// Name:        src/package.cpp
// Purpose:     source for package management
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     09/01/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/package.hpp"

int luaopen_lev_package(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("package")
    [
      def("require", &package::require)
    ]
  ];

  return 0;
}

namespace lev
{

  const char *package::get_path(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    return object_cast<const char *>(globals(L)["lev"]["package"]["path"]);
  }

  luabind::object package::require(const char *package)
  {
    return luabind::object();
  }

  bool package::set_path(lua_State *L, const char *path)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    globals(L)["lev"]["package"]["path"] = path;
    return true;
  }

}

