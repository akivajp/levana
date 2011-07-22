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

  static wxMenu* cast_menu(void *obj) { return (wxMenu *)obj; }

  menu::menu() : handler() {}

  menu::~menu() {}

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
    const char *str = NULL;
    const char *help_str = NULL;
    object f;
    bool enable = true;
    int id;
    const char *id_name = NULL;

    object t = util::get_merged(L, 2, -1);

    if (t["string"]) { str = object_cast<const char *>(t["string"]); }
    else if (t["str1"]) { str = object_cast<const char *>(t["str1"]); }
    else if (t["str"]) { str = object_cast<const char *>(t["str"]); }
    else if (t["s"]) { str = object_cast<const char *>(t["s"]); }

    if (t["help_string"]) { str = object_cast<const char *>(t["help_string"]); }
    else if (t["help_str"]) { str = object_cast<const char *>(t["help_str"]); }
    else if (t["help"]) { str = object_cast<const char *>(t["help"]); }
    else if (t["str2"]) { str = object_cast<const char *>(t["str2"]); }

    if (t["id_name"]) { id_name = object_cast<const char *>(t["id_name"]); }
    else if (t["idname"]) { id_name = object_cast<const char *>(t["idname"]); }
    else if (t["id"]) { id_name = object_cast<const char *>(t["id"]); }
    else if (t["name"]) { id_name = object_cast<const char *>(t["name"]); }
    else if (t["str3"]) { id_name = object_cast<const char *>(t["str3"]); }

    if (t["enabling"]) { enable = t["enabling"]; }
    else if (t["enable"]) { enable = t["enable"]; }
    else if (t["ena"]) { enable = t["ena"]; }
    else if (t["e"]) { enable = t["e"]; }
    else if (t["bool1"]) { enable = t["bool1"]; }

    if (t["function"]) { f = t["function"]; }
    else if (t["func"]) { f = t["func"]; }
    else if (t["f"]) { f = t["f"]; }

    menu *m = object_cast<menu *>(obj);
    if (str == NULL)
    {
      wxMenuItem *item = cast_menu(m->get_rawobj())->AppendSeparator();
      id = item->GetId();
    }
    else
    {
      id = m->append(-1, str, help_str);
    }

    if (!enable) { m->enable(id, false); }
    if (id and f)
    {
      obj["fmap"][id] = f;
    }
    if (id and id_name)
    {
      obj[id_name] = id;
    }
    lua_pushnumber(L, id);
    return 1;
  }

  luabind::object menu::build(luabind::object setting)
  {
    using namespace luabind;
    lua_State *L = setting.interpreter();
    const char *title = NULL;
    object m;

    if (!setting || luabind::type(setting) != LUA_TTABLE) { return m; }

    iterator i(setting), end;
    if (setting["title"]) { title = object_cast<const char *>(setting["title"]); }
    else if (setting["t"]) { title = object_cast<const char *>(setting["t"]); }
    else if (setting["label"]) { title = object_cast<const char *>(setting["label"]); }
    else if (type(*i) == LUA_TSTRING)
    {
      title = object_cast<const char *>(*i);
      i++;
    }

    m = globals(L)["lev"]["gui"]["menu"](title);
    for (; i != end; i++)
    {
      if (type(*i) == LUA_TTABLE)
      {
        m["append"](m, *i);
      }
    }
    return m;
  }

  menu* menu::create(const char *title)
  {
    menu *m = NULL;
    wxMenu *obj = NULL;
    try {
      m = new menu;
      m->_obj = obj = new wxMenu(wxString(title, wxConvUTF8));
      m->system_managed = false;
      return m;
    }
    catch (...) {
      delete m;
      return NULL;
    }
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

    object func = globals(L)["lev"]["classes"]["menu"]["create_c"];
    object menu = func(title);
    if (menu)
    {
      register_to(L, menu, "append", &menu::append_l);
      menu["fmap"] = newtable(L);
    }
    menu.push(L);
    return 1;
  }

  bool menu::enable(int id, bool enabling)
  {
    cast_menu(_obj)->Enable(id, enabling);
    return true;
  }


  static wxMenuBar* cast_mb(void *obj) { return (wxMenuBar *)obj; }

  bool menubar::append(menu *m, const char *title)
  {
    wxString new_title(title, wxConvUTF8);
    if (cast_mb(_obj)->Append(cast_menu(m->get_rawobj()), new_title))
    {
      m->hold();
      return true;
    }
    return false;
  }

  int menubar::append_l(lua_State *L)
  {
    using namespace luabind;
    object obj(from_stack(L, 1));
    object menu_obj;
    const char *title = NULL;
    const char *name = NULL;

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
    if (not menu_obj) { luaL_error(L, "menu (lev.gui.menu) is not specified"); }

    if (t["title"]) { title = object_cast<const char *>(t["title"]); }
    else if (t["t"]) { title = object_cast<const char *>(t["t"]); }
    else if (t["str1"]) { title = object_cast<const char *>(t["str1"]); }
    if (title == NULL) { luaL_error(L, "title (string) is not specified"); }

    if (t["id"]) { name = object_cast<const char *>(t["id"]); }
    else if (t["name"]) { name = object_cast<const char *>(t["name"]); }
    else if (t["str2"]) { name = object_cast<const char *>(t["str2"]); }

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
      if (name)
      {
        obj[name] = menu_obj;
      }
    }
    lua_pushboolean(L, result);
    return 1;
  }

  luabind::object menubar::build(luabind::object setting)
  {
    using namespace luabind;
    lua_State *L = setting.interpreter();
    object mb;

    if (!setting || type(setting) != LUA_TTABLE) { return mb; }
    mb = globals(L)["lev"]["gui"]["menubar"]();

    for (iterator i(setting), end; i != end; i++)
    {
      object m, name;
      const char *label1 = NULL, *label2 = NULL;

      iterator j(*i);
      if (j == end || type(*j) != LUA_TSTRING) { continue; }
      label1 = object_cast<const char *>(*j++);
      if (j != end && type(*j) == LUA_TSTRING)
      {
        label2 = object_cast<const char *>(*j++);
      }
      if (j != end && type(*j) == LUA_TSTRING) { name = *j++; }
      else if ((*i)["id"]) { name = (*i)["id"]; }
      else if ((*i)["name"]) { name = (*i)["name"]; }
      m = globals(L)["lev"]["gui"]["menu"](label2);
      for (; j != end; j++)
      {
        if (type(*j) == LUA_TTABLE)
        {
          m["append"](m, *j);
        }
      }
      mb["append"](mb, m, label1, name);
    }
    return mb;
  }

  menubar* menubar::create()
  {
    menubar *mb = NULL;
    wxMenuBar *obj = NULL;
    try {
      mb = new menubar;
      mb->_obj = obj = new wxMenuBar();
      mb->_id = obj->GetId();
      return mb;
    }
    catch (...) {
      delete mb;
      return NULL;
    }
  }

  int menubar::create_l(lua_State *L)
  {
    using namespace luabind;
    object classes = globals(L)["lev"]["classes"];

    object func = classes["menubar"]["create_c"];
    object mb = func();
    if (mb)
    {
      register_to(L, classes["menubar"], "append", &menubar::append_l);
      mb["append"] = classes["menubar"]["append"];
      mb["menus"] = newtable(L);
    }
    mb.push(L);
    return 1;
  }

  bool menubar::enable(int id, bool enabling)
  {
    cast_mb(_obj)->Enable(id, enabling);
    return true;
  }

}

