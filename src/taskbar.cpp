/////////////////////////////////////////////////////////////////////////////
// Name:        src/taskbar.cpp
// Purpose:     source for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "connect.hpp"
#include "lev/taskbar.hpp"
#include "register.hpp"

namespace lev
{

  class myTaskBarIcon : public wxTaskBarIcon
  {
    public:
      myTaskBarIcon() : wxTaskBarIcon(), menu_generator(), owner() {}

//      void SetRClickMenu(wxMenu *m) { this->menu = m; }

      virtual wxMenu *CreatePopupMenu()
      {
        luabind::object result;
        if (luabind::type(menu_generator) != LUA_TFUNCTION) { return NULL; }
        result = menu_generator();
        if (luabind::type(result) != LUA_TUSERDATA) { return NULL; }
        if (result["type_id"] != base::LEV_TMENU) { return NULL; }

        menu *m = luabind::object_cast<menu *>(result);
        luabind::iterator i(result["fmap"]), end;
        for (; i != end; i++)
        {
          owner["set_onmenu"](owner, i.key(), *i);
        }
        return (wxMenu *)m->get_obj();
      }

//      wxMenu *menu;
      luabind::object menu_generator;
      luabind::object owner;
      // Common Connect Interface
      DECLARE_CONNECT();
  };
  static inline myTaskBarIcon* cast_systray(void *obj) { return (myTaskBarIcon *)obj; }

  systray::systray() : control() { }
  systray::~systray() {}

  systray* systray::create()
  {
    systray *st = new systray();
    if (st == NULL) { return NULL; }
    myTaskBarIcon *obj = new myTaskBarIcon();
    if (obj == NULL) { goto Error; }
    st->_obj = obj;
    st->wx_managed = true;
    st->set_icon(icon::levana_icon(), "Levana Application");
    return st;

    Error:
    delete st;
    return NULL; }

  int systray::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["gui"]["systray"]["create_c"];
    object st = func();
    systray *tray = object_cast<systray *>(st);
    cast_systray(tray->_obj)->owner = st;
    st.push(L);
    return 1;
  }

  luabind::object systray::get_menu_generator()
  {
    return cast_systray(_obj)->menu_generator;
  }

  bool systray::popup(menu *m)
  {
    return ((myTaskBarIcon *)_obj)->PopupMenu((wxMenu *)m->_obj);
  }

  bool systray::set_icon(const icon& i, const char *tooltip)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
    return tray->SetIcon(*((wxIcon*)i._obj.get()), wxString(tooltip, wxConvUTF8));
  }

//  void systray::set_menu(menu *m)
//  {
//    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
//    tray->SetRClickMenu((wxMenu *)m->_obj);
//  }

//  int systray::set_menu_l(lua_State *L)
//  {
//    using namespace luabind;
//
//    luaL_checktype(L, 1, LUA_TUSERDATA);
//    luaL_checktype(L, 2, LUA_TUSERDATA);
//    object systray_obj(from_stack(L, 1));
//    object menu_obj(from_stack(L, 2));
//
//    systray *st = object_cast<systray *>(systray_obj);
//    menu *m = object_cast<menu *>(menu_obj);
//    st->set_menu(m);
//
//    luabind::iterator i(menu_obj["fmap"]), end;
//    for (; i != end; i++)
//    {
//      systray_obj["set_onmenu"](systray_obj, i.key(), *i);
//    }
//    lua_pushboolean(L, true);
//    return 1;
//  }

  bool systray::set_menu_generator(luabind::object lua_func)
  {
    cast_systray(_obj)->menu_generator = lua_func;
    return true;
  }

  bool systray::set_onmenu(int id, luabind::object lua_func)
  {
    ((myTaskBarIcon *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
    return true;
  }
}

