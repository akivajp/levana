/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "deriver.hpp"
#include "lev/frame.hpp"
#include "lev/gui.hpp"
#include "lev/image.hpp"
#include "lev/net.hpp"
#include "lev/util.hpp"

#include <string>
#include <luabind/luabind.hpp>

namespace lev
{

  int gui::msgbox_l(lua_State *L)
  {
    wxString msg, cap;
    const char *m = "Message", *c = "Message", *s = "";
    long style = 0;

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

    if (t["message"]) { m = object_cast<const char *>(t["message"]); }
    else if (t["mes"]) { m = object_cast<const char *>(t["mes"]); }
    else if (t["m"]) { m = object_cast<const char *>(t["m"]); }
    else if (t["str1"]) { m = object_cast<const char *>(t["str1"]); }

    if (t["caption"]) { c = object_cast<const char *>(t["caption"]); }
    else if (t["cap"]) { c = object_cast<const char *>(t["cap"]); }
    else if (t["c"]) { c = object_cast<const char *>(t["c"]); }
    else if (t["str2"]) { c = object_cast<const char *>(t["str2"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }
    else if (t["str3"]) { s = object_cast<const char *>(t["str3"]); }

    msg = wxString(m, wxConvUTF8);
    cap = wxString(c, wxConvUTF8);
    if (strstr(s, "yes")) { style = style | wxYES_NO; }
    if (strstr(s, "no")) { style = style | wxYES_NO; }
    if (strstr(s, "cancel")) { style = style | wxCANCEL; }
    if (strstr(s, "ok")) { style = style | wxOK; }
    if (strstr(s, "exclamation")) { style = style | wxICON_EXCLAMATION; }
    if (strstr(s, "hand")) { style = style | wxICON_HAND; }
    if (strstr(s, "error")) { style = style | wxICON_ERROR; }
    if (strstr(s, "question")) { style = style | wxICON_QUESTION; }
    if (strstr(s, "info")) { style = style | wxICON_INFORMATION; }

    int result = wxMessageBox(msg, cap, style);
    if (result == wxYES || result == wxOK) { lua_pushboolean(L, true); }
    else if (result == wxNO) { lua_pushboolean(L, false); }
    else if (result == wxCANCEL) { return 0; }
    return 1;
  }

  const char *gui::file_selector(const char *message, const char *def_path, const char *def_file,
                                 const char *def_ext, const char *wildcard, control *parent)
  {
    wxString msg, path, file, ext, wild, result;
    std::string str;
    int flags = 0;
    wxWindow *p = NULL;
    if (message) { msg = wxString(message, wxConvUTF8); }
    else { msg = wxT("Choose a file"); }
    if (def_path) { path = wxString(def_path, wxConvUTF8); }
    if (def_file) { file = wxString(def_file, wxConvUTF8); }
    if (def_ext)  { ext  = wxString(def_ext,  wxConvUTF8); }
    if (wildcard) { wild = wxString(wildcard, wxConvUTF8); }
    else { wild = wxT("*.*"); }
    if (parent) { p = (wxWindow *)parent->get_rawobj(); }
    result = wxFileSelector(msg, path, file, ext, wild, flags, p);
    str = (const char *)result.mb_str(wxConvUTF8);
    return str.c_str();
  }

  static wxWindow *cast_win(void *obj) { return (wxWindow *)obj; }

  control::control() : handler(), _id(0), _sz(NULL)
  { }

  control::~control()
  {
    if (system_managed)
    {
      // wxWidgets will automatically delete
    }
    else if (_id && !is_valid())
    {
      // the object isn't already found
      _obj = NULL;
    }
  }

  int control::get_height()
  {
    int h;
    cast_win(_obj)->GetSize(NULL, &h);
    return h;
  }

  int control::getid()
  {
    return _id;
  }

  sizer *control::get_sizer()
  {
    return _sz;
  }

  long control::get_style()
  {
    return ((wxWindow *)_obj)->GetWindowStyle();
  }

  int control::get_width()
  {
    int w;
    cast_win(_obj)->GetSize(&w, NULL);
    return w;
  }

  bool control::is_shown()
  {
    return ((wxWindow *)_obj)->IsShown();
  }

  bool control::is_valid()
  {
    if (wxWindow::FindWindowById(_id)) { return true; }
    return false;
  }

  bool control::set_height(int h)
  {
    cast_win(_obj)->SetSize(get_width(), h);
    return true;
  }

  void control::set_sizer(sizer *s)
  {
    if (s)
    {
      ((wxWindow *)_obj)->SetSizer((wxSizer *)s->get_rawobj());
      _sz = s;
      s->wx_managed = true;
    }
  }

  bool control::set_shown(bool showing)
  {
//    gui_lock();
    return ((wxWindow *)_obj)->Show(showing);
  }

  bool control::set_width(int w)
  {
    cast_win(_obj)->SetSize(w, get_height());
    return true;
  }



  // html viewer control definitions
  class myHtmlWindow : public wxHtmlWindow
  {
    public:
      inline myHtmlWindow(wxWindow *parent, int width, int height)
        : wxHtmlWindow(parent, -1, wxDefaultPosition, wxSize(width, height)),
          body()
      {}

      inline ~myHtmlWindow() {}
      std::string body;
  };
  inline static myHtmlWindow *cast_html(void *obj) { return (myHtmlWindow *)obj; }

  htmlview* htmlview::create(control *parent, int width, int height)
  {
    htmlview *html = NULL;
    myHtmlWindow *obj = NULL;
    wxWindow *p = NULL;
    try {
      html = new htmlview;
      if (parent)
      {
        p = (wxWindow *)parent->get_rawobj();
        html->system_managed = true;
      }
      obj = new myHtmlWindow(p, width, height);
      html->_id = obj->GetId();
      html->_obj = obj;
      if (p)
      {
        obj->SetRelatedFrame((wxFrame *)p, wxT("%s"));
        frame *frm = (frame *)parent;
        frm->set_status("");
        obj->SetRelatedStatusBar(0);
      }
      return html;
    }
    catch (...) {
      delete html;
      return NULL;
    }
  }

  int htmlview::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;

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
    else if (t["p"]) { p = t["p"]; }
    else if (t["udata"]) { p = t["udata"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    object func = globals(L)["lev"]["classes"]["htmlview"]["create_c"];
    object html = func(p, w, h);
    if (html)
    {
      // do something
    }
    html.push(L);
    return 1;
  }

  const char *htmlview::get_page()
  {
    return cast_html(_obj)->body.c_str();
  }

  bool htmlview::loadpage(const char *str_url)
  {
    image::init();
    url *u = NULL;
    std::string body;
    try {
      u = url::connect(str_url);
      if (u == NULL) { throw -1; }
      body = u->get_data();
      cast_html(_obj)->body = body;
      delete u;
    }
    catch (...) {
      delete u;
      return false;
    }
    return cast_html(_obj)->SetPage(wxString(body.c_str(), wxConvUTF8));
  }

  bool htmlview::set_page(const char *src)
  {
    image::init();
    cast_html(_obj)->body = src;
    return cast_html(_obj)->SetPage(wxString(src, wxConvUTF8));
  }

  const char* htmlview::totext()
  {
    const std::string text =
      (const char *)((myHtmlWindow *)_obj)->ToText().mb_str(wxConvUTF8);
    return text.c_str();
  }




  typedef MyHandler<wxTextCtrl> myTextCtrl;
  static myTextCtrl *cast_txt(void *obj) { return (myTextCtrl *)obj; }

  textbox* textbox::create(control *parent, int width, int height, const char *value, long style)
  {
    textbox *tb = NULL;
    myTextCtrl *obj = NULL;
    wxWindow *p = NULL;
    wxString val = wxEmptyString;
    try {
      tb = new textbox;
      if (parent)
      {
        p = (wxWindow *)parent->get_rawobj();
        tb->system_managed = true;
      }
      if (value) { val = wxString(value, wxConvUTF8); }
      obj = new myTextCtrl;
      if (not obj->Create(p, -1, val, wxDefaultPosition, wxSize(width, height), style))
      {
        throw -1;
      }
      tb->_obj = obj;
      tb->_id = obj->GetId();
      tb->connector = obj->GetConnector();
      tb->func_getter = obj->GetFuncGetter();
      return tb;
    }
    catch (...) {
      delete obj;
      delete tb;
      return NULL;
    }
  }

  int textbox::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;
    const char *v = "";
    const char *s = "";
    long style = 0;

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
    else if (t["p"]) { p = t["p"]; }
    else if (t["udata"]) { p = t["udata"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    if (t["value"]) { v = object_cast<const char *>(t["value"]); }
    else if (t["val"]) { v = object_cast<const char *>(t["val"]); }
    else if (t["v"]) { v = object_cast<const char *>(t["v"]); }
    else if (t["str1"]) { v = object_cast<const char *>(t["str1"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }

    if (strstr(s, "multi")) { style = wxTE_MULTILINE; }

    object func = globals(L)["lev"]["classes"]["textbox"]["create_c"];
    object tb = func(p, w, h, v, style);
    if (tb)
    {
      // do something
    }
    tb.push(L);
    return 1;
  }

  const char* textbox::get_value()
  {
    const std::string value =
      (const char *)((wxTextCtrl *)_obj)->GetValue().mb_str(wxConvUTF8);
    return value.c_str();
  }

  bool textbox::set_multiline()
  {
    long style = get_style();
    cast_txt(_obj)->SetWindowStyle(style | wxTE_MULTILINE);
    return true;
  }

  void textbox::set_value(const char *value)
  {
    ((wxTextCtrl *)_obj)->SetValue(wxString(value, wxConvUTF8));
  }

}

