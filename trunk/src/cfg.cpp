/////////////////////////////////////////////////////////////////////////////
// Name:        src/cfg.cpp
// Purpose:     source for defining constant values
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/29/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/cfg.hpp"

#include <luabind/luabind.hpp>
#include <string>
#include <wx/wx.h>

namespace lev
{
  namespace cfg
  {
    int frame_style(lua_State *L)
    {
      int n = lua_gettop(L);
      lua_Integer style = wxDEFAULT_FRAME_STYLE;
      for (int i = 1; i <= n; i++)
      {
        switch( luaL_checkinteger(L, i) )
        {
          case cfg::FIXED:
            style = style & ~wxRESIZE_BORDER;
            break;
          default:
            break;
        }
      }
      lua_pushinteger(L, style);
      return 1;
    }

    int luaopen_cfg(lua_State *L)
    {
      using namespace luabind;

      module(L, "lev")
      [
        namespace_("cfg")
      ];

      object cfg(globals(L)["lev"]["cfg"]);
      cfg["default"] = cfg::DEFAULT;
      cfg["fixed"]   = cfg::FIXED;
      return 1;
    }
  }
}

