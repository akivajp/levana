#ifndef _FONT_HPP
#define _FONT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/font.hpp
// Purpose:     header for font management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     16/08/2010
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "gui.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_font(lua_State *L);
}

namespace lev
{

  class font : public base
  {
    protected:
      font();
    public:
      virtual ~font();
      font* clone();
      static font* load(const char *desc);
      static int load_l(lua_State *L);
      const char *get_face_name();
      const char *get_family();
      int get_point_size();
      void *get_rawobj() { return _obj; }
      const char *get_style();
      virtual type_id get_type_id() const { return LEV_TFONT; }
      virtual const char *get_type_name() const { return "lev.font"; }
      static font* select_font();
      bool set_face_name(const char *face);
      bool set_point_size(int size);
    protected:
      void *_obj;
  };

}

#endif // _FONT_HPP

