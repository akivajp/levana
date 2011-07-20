/////////////////////////////////////////////////////////////////////////////
// Name:        src/sizer.cpp
// Purpose:     source for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/sizer.hpp"

#include "lev/event.hpp"
#include "lev/gui.hpp"
#include "lev/util.hpp"
#include "register.hpp"

#include <luabind/luabind.hpp>

namespace lev
{

  inline static wxSizer *cast_sizer(void *obj) { return (wxSizer *)obj; }

  sizer::sizer() : base(), wx_managed(false), _obj(NULL) {}

  sizer::~sizer()
  {
    if (wx_managed)
    {
      // object is in wxWidgets control
    }
    else
    {
      delete (wxSizer *)_obj;
    }
  }

  void sizer::add_ctrl(control *c, int proportion, int border)
  {
    cast_sizer(_obj)->Add((wxWindow*)c->get_rawobj(), proportion, wxGROW, border);
  }
  void sizer::add_sizer(sizer *s, int proportion, int border)
  {
    cast_sizer(_obj)->Add((wxSizer*)s->get_rawobj(), proportion, wxGROW, border);
    s->wx_managed = true;
  }
  void sizer::add_space(int width, int height, int proportion, int border)
  {
    ((wxSizer *)_obj)->Add(width, height, proportion, wxGROW, border);
  }

  int sizer::add_l(lua_State *L)
  {
    using namespace luabind;
//    object obj(from_stack(L, 1));
    sizer *sz = object_cast<sizer *>(object(from_stack(L, 1)));
    object o;
    int p = 0, b = 0;
    int w = 10, h = 10;

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 2; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n, 1);
    object t(from_stack(L, -1));
    lua_remove(L, -1);

    if (t["object"]) { o = t["object"]; }
    else if (t["o"]) { o = t["o"]; }
    else if (t["control"]) { o = t["control"]; }
    else if (t["ctrl"]) { o = t["ctrl"]; }
    else if (t["c"]) { o = t["c"]; }
    else if (t["sizer"]) { o = t["sizer"]; }
    else if (t["sz"]) { o = t["sz"]; }
    else if (t["s"]) { o = t["s"]; }
    else if (t["udata"]) { o = t["udata"]; }

    if (o)
    {
      if (t["proportion"]) { p = object_cast<int>(t["proportion"]); }
      else if (t["prop"]) { p = object_cast<int>(t["prop"]); }
      else if (t["p"]) { p = object_cast<int>(t["p"]); }
      else if (t["num1"]) { p = object_cast<int>(t["num1"]); }

      if (t["border"]) { b = object_cast<int>(t["border"]); }
      else if (t["b"]) { b = object_cast<int>(t["b"]); }
      else if (t["num2"]) { b = object_cast<int>(t["num2"]); }
    }
    else
    {
      if (t["width"]) { w = object_cast<int>(t["width"]); }
      else if (t["w"]) { w = object_cast<int>(t["w"]); }
      else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

      if (t["height"]) { h = object_cast<int>(t["height"]); }
      else if (t["h"]) { h = object_cast<int>(t["h"]); }
      else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

      if (t["proportion"]) { p = object_cast<int>(t["proportion"]); }
      else if (t["prop"]) { p = object_cast<int>(t["prop"]); }
      else if (t["p"]) { p = object_cast<int>(t["p"]); }
      else if (t["num3"]) { p = object_cast<int>(t["num3"]); }

      if (t["border"]) { b = object_cast<int>(t["border"]); }
      else if (t["b"]) { b = object_cast<int>(t["b"]); }
      else if (t["num4"]) { b = object_cast<int>(t["num4"]); }
    }

    if (o)
    {
      int id = object_cast<int>(o["type_id"]);
      if (LEV_TCONTROL <= id && id < LEV_TCONTROL_END)
      {
        control *ctrl = object_cast<control *>(o);
        sz->add_ctrl(ctrl, p, b);
      }
      else if (LEV_TSIZER <= id && id < LEV_TSIZER_END)
      {
        sizer *s = object_cast<sizer *>(o);
        sz->add_sizer(s, p, b);
      }
    }
    else
    {
      sz->add_space(w, h, p, b);
    }
    return 0;
  }

  void sizer::fit(control *c)
  {
    cast_sizer(_obj)->Fit((wxWindow*)c->get_rawobj());
  }

  void sizer::fitinside(control *c)
  {
    cast_sizer(_obj)->FitInside((wxWindow *)c->get_rawobj());
  }

  void sizer::layout()
  {
    ((wxSizer *)_obj)->Layout();
  }

  gsizer::gsizer() : sizer() { }
  gsizer::~gsizer() { }

  gsizer* gsizer::create(int cols, int vgap, int hgap)
  {
    gsizer *sz = NULL;
    wxFlexGridSizer *obj = NULL;
    try {
      sz = new gsizer;
      obj = new wxFlexGridSizer(cols, vgap, hgap);
      sz->_obj = obj;
      sz->wx_managed = false;
      return sz;
    }
    catch (...) {
      delete sz;
      return NULL;
    }
  }

  int gsizer::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["classes"]["gsizer"]["create_c"];
    object sz = func();
    if (sz)
    {
      if (not sz["add"])
      {
        register_to(L, sz, "add", &sizer::add_l);
      }
    }
    sz.push(L);
    return 1;
  }


  hsizer::hsizer() : sizer() { }
  hsizer::~hsizer() { }

  hsizer* hsizer::create()
  {
    hsizer *sz = NULL;
    wxBoxSizer *obj = NULL;
    try {
      sz = new hsizer;
      obj = new wxBoxSizer(wxHORIZONTAL);
      sz->_obj = obj;
      sz->wx_managed = false;
      return sz;
    }
    catch (...) {
      delete sz;
      return NULL;
    }
  }

  int hsizer::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["classes"]["hsizer"]["create_c"];
    object sz = func();
    if (sz)
    {
      if (not sz["add"])
      {
        register_to(L, sz, "add", &sizer::add_l);
      }
    }
    sz.push(L);
    return 1;
  }

  vsizer::vsizer() : sizer() {}
  vsizer::~vsizer() {}

  vsizer* vsizer::create()
  {
    vsizer *sz = NULL;
    wxBoxSizer *obj = NULL;
    try {
      sz  = new vsizer;
      obj = new wxBoxSizer(wxVERTICAL);
      sz->_obj = obj;
      sz->wx_managed = false;
      return sz;
    }
    catch (...) {
      delete sz;
      return NULL;
    }
  }

  int vsizer::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["classes"]["vsizer"]["create_c"];
    object sz = func();
    if (sz)
    {
      if (not sz["add"])
      {
        register_to(L, sz, "add", &sizer::add_l);
      }
    }
    sz.push(L);
    return 1;
  }

}

