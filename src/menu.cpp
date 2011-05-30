/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "register.hpp"
#include "lev/menu.hpp"
#include "lev/util.hpp"

namespace lev
{

  menu::menu() : control() {}

  menu::~menu()
  {
    if (_obj) { delete (wxMenu *)_obj; }
  }

  int menu::append(int id, const char *str, const char *help_str)
  {
    wxString new_str(str, wxConvUTF8);
    wxString new_help(help_str, wxConvUTF8);
    wxMenuItem *item = ((wxMenu *)_obj)->Append(id, new_str, new_help);
    if (item == NULL) { return 0; }
    return item->GetId();
  }

  int menu::append_l(lua_State *L)
  {
    using namespace luabind;
    object obj(from_stack(L, 1));
    const char *str = "(DEFAULT)";
    const char *help_str = NULL;
    object f;

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 2; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n, 1);
    object t(from_stack(L, -1));
    lua_pop(L, 1);

    if (t["string"]) { str = object_cast<const char *>(t["string"]); }
    else if (t["str1"]) { str = object_cast<const char *>(t["str1"]); }
    else if (t["str"]) { str = object_cast<const char *>(t["str"]); }
    else if (t["s"]) { str = object_cast<const char *>(t["s"]); }

    if (t["help_string"]) { str = object_cast<const char *>(t["help_string"]); }
    else if (t["help_str"]) { str = object_cast<const char *>(t["help_str"]); }
    else if (t["help"]) { str = object_cast<const char *>(t["help"]); }
    else if (t["str2"]) { str = object_cast<const char *>(t["str2"]); }

    if (t["function"]) { f = t["function"]; }
    else if (t["func"]) { f = t["func"]; }
    else if (t["f"]) { f = t["f"]; }

    menu *m = object_cast<menu *>(obj);
    int id = m->append(-1, str, help_str);
    if (id and f)
    {
      obj["fmap"][id] = f;
    }
    lua_pushnumber(L, id);
    return 1;
  }

  menu* menu::create(const char *title)
  {
    menu *m = new menu;
    if (m == NULL) { return m; }
    wxMenu *obj = new wxMenu(wxString(title, wxConvUTF8));
    if (obj == NULL) { goto Error; }
    m->_obj = obj;
    return m;

    Error:
    delete m;
    return NULL;
  }

  int menu::create_l(lua_State *L)
  {
    using namespace luabind;
    const char *title = NULL;

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 1; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n + 1, 1);
    object t(from_stack(L, -1));
    lua_remove(L, -1);

    if (t["title"]) { title = object_cast<const char *>(t["title"]); }
    else if (t["t"]) { title = object_cast<const char *>(t["t"]); }
    else if (t["str1"]) { title = object_cast<const char *>(t["str1"]); }

    object func = globals(L)["lev"]["gui"]["menu"]["create_c"];
    object menu = func(title);
    if (menu)
    {
      register_to(L, menu, "append", &menu::append_l);
      menu["fmap"] = newtable(L);
    }
    menu.push(L);
    return 1;
  }





  menubar::menubar() : control() { }

  menubar::~menubar()
  {
    if (_obj) { delete ((wxMenuBar *)_obj); }
  }

  bool menubar::append(menu *m, const char *title)
  {
    wxString new_title(title, wxConvUTF8);
    return ((wxMenuBar *)_obj)->Append((wxMenu *)m->_obj, new_title);
  }

  int menubar::append_l(lua_State *L)
  {
    using namespace luabind;
    object obj(from_stack(L, 1));
    object menu_obj;
    const char *title = "(DEFAULT)";

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 2; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n, 1);
    object t(from_stack(L, -1));
    lua_pop(L, 1);

    if (t["menu"]) { menu_obj = t["menu"]; }
    else if (t["m"]) { menu_obj = t["m"]; }
    else if (t["udata"]) { menu_obj = t["udata"]; }

    if (t["title"]) { title = object_cast<const char *>(t["title"]); }
    else if (t["t"]) { title = object_cast<const char *>(t["t"]); }
    else if (t["str"]) { title = object_cast<const char *>(t["str"]); }

    if (not menu_obj)
    {
      lua_pushboolean(L, false);
      return 1;
    }

    menubar *mb = object_cast<menubar *>(obj);
    menu *m = object_cast<menu *>(menu_obj);
    bool result = mb->append(m, title);
    if (result)
    {
      int i;
      for (i = 1; obj["menus"][i]; i++) {}
      obj["menus"][i] = menu_obj;
    }
    lua_pushboolean(L, result);
    return 1;
  }

  menubar* menubar::create()
  {
    menubar *mb = new menubar;
    if (mb == NULL) { return NULL; }
    wxMenuBar *obj = new wxMenuBar();
    if (obj == NULL) { goto Error; }
    mb->_obj = obj;
    return mb;

    Error:
    delete mb;
    return NULL;
  }

  int menubar::create_l(lua_State *L)
  {
    using namespace luabind;

    object func = globals(L)["lev"]["gui"]["menubar"]["create_c"];
    object mb = func();
    if (mb)
    {
      register_to(L, mb, "append", &menubar::append_l);
      mb["menus"] = newtable(L);
    }
    mb.push(L);
    return 1;
  }
}

