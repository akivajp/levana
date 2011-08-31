/////////////////////////////////////////////////////////////////////////////
// Name:        src/taskbar.cpp
// Purpose:     source for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "deriver.hpp"
#include "register.hpp"
#include "lev/taskbar.hpp"

namespace lev
{

  class myTaskBarIcon : public myHandler<wxTaskBarIcon>
  {
    public:
      myTaskBarIcon() : menugen() {}

      virtual wxMenu *CreatePopupMenu()
      {
        luabind::object result;
        if (menugen && luabind::type(menugen) != LUA_TFUNCTION) { return NULL; }
        result = menugen();
        if (luabind::type(result) != LUA_TUSERDATA) { return NULL; }
        if (result["type_id"] != base::LEV_TMENU) { return NULL; }
        menu *m = luabind::object_cast<menu *>(result);
        luabind::iterator i(result["fmap"]), end;
        for (; i != end; i++)
        {
          Connect(object_cast<int>(i.key()), wxEVT_COMMAND_MENU_SELECTED, *i);
        }
        m->hold();
        return (wxMenu *)m->get_rawobj();
      }

      luabind::object menugen;
  };
  static inline myTaskBarIcon* cast_tray(void *obj) { return (myTaskBarIcon *)obj; }

  systray::systray() : handler() { }

  systray::~systray() {}

  systray* systray::create()
  {
    systray *tray = NULL;
    myTaskBarIcon *obj = NULL;
    try {
      tray = new systray();
      obj = new myTaskBarIcon();
      tray->_obj = obj;
      tray->connector = obj->GetConnector();
      tray->func_getter = obj->GetFuncGetter();
      tray->system_managed = true;
      tray->set_icon(image::levana_icon(), "Levana Application");
      return tray;
    }
    catch (...) {
      delete obj;
      delete tray;
      return NULL;
    }
  }

  int systray::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["classes"]["systray"]["create_c"];
    object st = func();
    systray *tray = object_cast<systray *>(st);
    st.push(L);
    return 1;
  }

  luabind::object systray::get_menu_generator()
  {
    return cast_tray(_obj)->menugen;
  }

  bool systray::popup(menu *m)
  {
    return cast_tray(_obj)->PopupMenu((wxMenu *)m->_obj);
  }

  bool systray::remove_icon()
  {
    cast_tray(_obj)->RemoveIcon();
    return true;
  }

  bool systray::set_icon(image *i, const char *tooltip)
  {
    if (i == NULL) { return false; }
    wxBitmap *bmp = (wxBitmap *)i->get_rawobj();
    wxIcon ico;
    ico.CopyFromBitmap(*bmp);
    return cast_tray(_obj)->SetIcon(ico, wxString(tooltip, wxConvUTF8));
  }

  bool systray::set_menu_generator(luabind::object lua_func)
  {
    cast_tray(_obj)->menugen = lua_func;
    return true;
  }

}

