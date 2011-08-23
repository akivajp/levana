#ifndef _INFO_HPP
#define _INFO_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/info.hpp
// Purpose:     header for platform infomation magaging class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     08/21/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_info(lua_State *L);
}

namespace lev
{

  class info : public base
  {
    protected:
      info();
    public:
      virtual ~info();
      static info* create();
      int get_architecture();
      int get_endianness();
      int get_os_major_version();
      int get_os_minor_version();
      static std::string get_os_description();
      std::string get_os_name();
      virtual type_id get_type_id() const { return LEV_TINFO; }
      virtual const char *get_type_name() const { return "lev.info"; }
    protected:
      void *_obj;
  };

}

#endif // _INFO_HPP

