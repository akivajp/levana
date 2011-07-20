#ifndef _LOCALE_HPP
#define _LOCALE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/locale.hpp
// Purpose:     header for localization managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     20/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <luabind/luabind.hpp>
#include <string>

extern "C" {
  int luaopen_lev_locale(lua_State *L);
}

namespace lev
{

  class locale : public base
  {
    protected:
      locale() : _obj(NULL) { }
    public:
      virtual ~locale() { }
      bool add_catalog(const char *catalog);
      bool add_catalog_path(const char *path);
      const char *get_encoding();
      const char *get_language();
      const char *get_name();
//      virtual type_id get_type_id() const { return LEV_TLOCALE; }
      virtual const char *get_type_name() const { return "lev.locale"; }
      static const char *translate(const char *src);

      friend class application;

    protected:
      void *_obj;
  };

}

#endif // _STRING_HPP

