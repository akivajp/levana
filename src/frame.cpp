/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for frame managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "lev/app.hpp"
#include "lev/frame.hpp"
#include "lev/util.hpp"
#include "deriver.hpp"
#include "register.hpp"

#include <string>
#include <map>
#include <luabind/luabind.hpp>

using namespace boost;

namespace lev
{

  typedef MyHandler<wxFrame> myFrame;
//  class myFrame : public wxFrame
//  {
//    public:
//      inline myFrame() : wxFrame() {}
//      inline myFrame(wxWindow* parent, const wxString title, int height, int width, long style = wxDEFAULT_FRAME_STYLE)
//        : wxFrame(parent, -1, title, wxDefaultPosition, wxSize(height, width), style)
//      {}
//  };


  bool frame::close(bool force)
  {
    if (!_obj) { return false; }
//    safe_gui_lock lock;
//    bool result = ((wxWindow *)_obj)->Close(force);
    return ((wxWindow *)_obj)->Close(force);
//    ((wxEvtHandler *)_obj)->QueueEvent(new wxCommandEvent(wxEVT_CLOSE_WINDOW));
//    wxCommandEvent close(wxEVT_CLOSE_WINDOW);
//    ((wxEvtHandler *)_obj)->AddPendingEvent(close);
  }

  frame *frame::create(control *parent, const char *title, int w, int h, long style)
  {
    frame   *frm = NULL;
    myFrame *obj = NULL;
    try {
      frm = new frame();
      wxWindow *p = parent ? (wxWindow *)parent->get_rawobj() : NULL;
      if (style == -1) { style = wxDEFAULT_FRAME_STYLE; }
      obj = new myFrame;
      if (not obj->Create(p, -1, wxString(title, wxConvUTF8), wxDefaultPosition, wxSize(w, h), style))
      {
        throw -1;
      }
      frm->_id = obj->GetId();
      frm->_obj = obj;
      frm->connector = obj->GetConnector();
      frm->func_getter = obj->GetFuncGetter();
      frm->seticon(icon::levana_icon());
      frm->system_managed = true;
      return frm;
    }
    catch (...) {
      delete obj;
      delete frm;
      return NULL;
    }
  }

  int frame::create_l(lua_State *L)
  {
    using namespace luabind;
    control *p = NULL;
    const char *title = "Levana Application";
    int w = -1, h = -1;
    const char *s = "default";
    long style = wxDEFAULT_FRAME_STYLE;
    frame *frm = NULL;

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

    if (t["parent"]) { p = object_cast<control *>(t["parent"]); }
    else if (t["p"]) { p = object_cast<control *>(t["p"]); }
    else if (t["udata"]) { p = object_cast<control *>(t["udata"]); }

    if (t["title"]) { title = object_cast<const char *>(t["title"]); }
    else if (t["t"]) { title = object_cast<const char *>(t["t"]); }
    else if (t["str1"]) { title = object_cast<const char *>(t["str1"]); }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }
    else if (t["str2"]) { s = object_cast<const char *>(t["str2"]); }

    if (strstr(s, "fixed")) { style = style & ~wxRESIZE_BORDER; }

    object func = globals(L)["lev"]["classes"]["frame"]["create_c"];
    object frame = func(p, title, w, h, style);
    if (frame)
    {
      register_to(L, frame, "set_menubar", &frame::set_menubar_l);
      register_to(L, frame, "setmenubar", &frame::set_menubar_l);
    }
    frame.push(L);
    return 1;
  }


  void frame::fit()
  {
    ((myFrame *)_obj)->Fit();
  }

  const char * frame::get_status()
  {
    return status;
  }

  const char *frame::gettitle()
  {
    const std::string str = (const char *)((wxFrame *)_obj)->GetTitle().mb_str(wxConvUTF8);
    return str.c_str();
  }


  frame *frame::gettop()
  {
    static frame top;
    wxWindow *frm = wxTheApp->GetTopWindow();
    if (frm == NULL) { return NULL; }
    top._obj = frm;
    return &top;
  }


  void frame::seticon(const icon &i)
  {
    ((wxFrame *)_obj)->SetIcon(*((wxIcon *)i._obj.get()));
  }

  void frame::set_menubar(menubar *mb)
  {
    ((wxFrame *)_obj)->SetMenuBar((wxMenuBar *)mb->_obj);
  }

  int frame::set_menubar_l(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    luaL_checktype(L, 2, LUA_TUSERDATA);
    object frame_obj(from_stack(L, 1));
    object mb(from_stack(L, 2));

    frame *frm = object_cast<frame *>(frame_obj);
    frm->set_menubar(object_cast<menubar *>(mb));
    luabind::iterator i(mb["menus"]), end;
    for (; i != end; i++)
    {
      luabind::iterator j((*i)["fmap"]);
      for (; j != end; j++)
      {
        frame_obj["set_on_menu"](frame_obj, j.key(), *j);
      }
    }
    lua_pushboolean(L, true);
    return 1;
  }

  void frame::settop(frame *top)
  {
    wxTheApp->SetTopWindow((wxFrame *)top->_obj);
  }

  void frame::set_status(const char *str_status)
  {
    wxStatusBar *sb = ((myFrame *)_obj)->GetStatusBar();
    status = str_status;
    if     (status == NULL && sb == NULL) { return; }
    if     (status == NULL && sb != NULL) { delete sb; }
    else if(status != NULL && sb == NULL)
    {
      ((myFrame *)_obj)->CreateStatusBar();
      ((myFrame *)_obj)->SetStatusText(wxString(str_status, wxConvUTF8));
    }
    else // if (status != NULL && sb != NULL)
    {
      ((myFrame *)_obj)->SetStatusText(wxString(str_status, wxConvUTF8));
    }
  }

  void frame::settitle(const char *title)
  {
    ((wxFrame *)_obj)->SetTitle(wxString(title, wxConvUTF8));
  }


}

