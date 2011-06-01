/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/gui.hpp
// Purpose:     header for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include <lua.h>

namespace lev
{
  class htmlview: public control
  {
    public:
      htmlview(control *parent, int width, int height);
      virtual type_id get_type_id() const { return LEV_THTMLVIEW; }
      virtual const char *get_type_name() const { return "htmlview"; }
      bool loadpage(const char *url);
      bool setpage(const char *src);
      const char *totext();
  };

  class textbox: public control
  {
    private:
      textbox();
    public:
      virtual ~textbox();
      textbox(control *parent, int width, int height, const char *value = "");
      static textbox* create(control *parent, int width, int height, const char *value = "");
      static int create_l(lua_State *L);
      virtual luabind::object get_onkeydown();
      virtual type_id get_type_id() const { return LEV_TTEXTBOX; }
      virtual const char *get_type_name() const { return "textbox"; }
      const char* get_value();
      virtual bool set_onkeydown(luabind::object lua_func);
      void set_value(const char *value);
  };
}

