/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "connect.hpp"
#include "lev/gui.hpp"
#include "lev/util.hpp"

#include <string>
#include <luabind/luabind.hpp>

namespace lev
{
  // html viewer control definitions
  class myHtmlWindow : public wxHtmlWindow
  {
    public:
      inline myHtmlWindow(wxWindow *parent, int width, int height)
        : wxHtmlWindow(parent, -1, wxDefaultPosition, wxSize(width, height))
      {}

      inline ~myHtmlWindow() {}
  };

  htmlview::htmlview(control *parent, int width, int height)
    : control()
  {
    try {
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->_obj; }
      wxHtmlWindow *html = new myHtmlWindow(p, width, height);
      _id = html->GetId();
      _obj = html;
    }
    catch (...)
    {
      throw "htmlview: allocation error";
    }
  }


  bool htmlview::loadpage(const char *url)
  {
    return ((myHtmlWindow *)_obj)->LoadPage(wxString(url, wxConvUTF8));
  }

  bool htmlview::setpage(const char *src)
  {
    return ((myHtmlWindow *)_obj)->SetPage(wxString(src, wxConvUTF8));
  }

  const char* htmlview::totext()
  {
    const std::string text =
      (const char *)((myHtmlWindow *)_obj)->ToText().mb_str(wxConvUTF8);
    return text.c_str();
  }




  // text control definitions
  class myTextCtrl : public wxTextCtrl
  {
    public:
      inline myTextCtrl(wxWindow *parent, int width, int height, wxString &value) :
        wxTextCtrl(parent, -1, value, wxDefaultPosition, wxSize(width, height)) {}
    private:
      // Common Connect Interface
      DECLARE_CONNECT();
  };


  textbox::textbox() : control() { }
  textbox::~textbox() {}

  textbox* textbox::create(control *parent, int width, int height, const char *value)
  {
    textbox *tb = new textbox;
    if (tb == NULL) { return NULL; }
    wxWindow *p = NULL;
    wxString val = wxEmptyString;
    if (parent) { p = (wxWindow *)parent->_obj; }
    if (value) { val = wxString(value, wxConvUTF8); }

    myTextCtrl *text = new myTextCtrl(p, width, height, val);
    if (text == NULL) { goto Error; }

    if (p == NULL) { tb->_managing = true; }
    else { tb->_managing = false; }
    tb->_id = text->GetId();
    tb->_obj = text;
    return tb;

    Error:
    delete tb;
    return NULL;
  }

  int textbox::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;
    const char *v = "";

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

    if (t["parent"]) { p = t["parent"]; }
    if (t["p"]) { p = t["p"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    if (t["value"]) { v = object_cast<const char *>(t["value"]); }
    else if (t["val"]) { v = object_cast<const char *>(t["val"]); }
    else if (t["v"]) { v = object_cast<const char *>(t["v"]); }

    object func = globals(L)["lev"]["gui"]["textbox"]["create_c"];
    object tb = func(p, w, h, v);
    if (tb)
    {
      // do something
    }
    tb.push(L);
    return 1;
  }

  luabind::object textbox::get_onkeydown()
  {
    return ((myTextCtrl *)_obj)->_fmap[wxEVT_KEY_DOWN][-1];
  }

  const char* textbox::get_value()
  {
    const std::string value =
      (const char *)((wxTextCtrl *)_obj)->GetValue().mb_str(wxConvUTF8);
    return value.c_str();
  }

  bool textbox::set_onkeydown(luabind::object lua_func)
  {
    ((myTextCtrl *)_obj)->Connect(-1, wxEVT_KEY_DOWN, lua_func);
    return true;
  }

  void textbox::set_value(const char *value)
  {
    ((wxTextCtrl *)_obj)->SetValue(wxString(value, wxConvUTF8));
  }
}

