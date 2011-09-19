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

#include <boost/format.hpp>
#include <string>
#include <map>
#include <luabind/luabind.hpp>

using namespace boost;

namespace lev
{

  class myFrame : public myHandler<wxFrame>
  {
    public:
      myFrame() : mb() { }
      virtual ~myFrame() { }

      luabind::object mb;
  };
  static myFrame* cast_frm(void *obj) { return (myFrame *)obj; }

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
      frm->set_icon(image::levana_icon());
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

    object t = util::get_merged(L, 1, -1);

    if (t["parent"]) { p = object_cast<control *>(t["parent"]); }
    else if (t["p"]) { p = object_cast<control *>(t["p"]); }
    else if (t["lua.userdata1"]) { p = object_cast<control *>(t["lua.userdata1"]); }

    if (t["caption"]) { title = object_cast<const char *>(t["caption"]); }
    else if (t["cap"]) { title = object_cast<const char *>(t["cap"]); }
    else if (t["c"]) { title = object_cast<const char *>(t["c"]); }
    else if (t["title"]) { title = object_cast<const char *>(t["title"]); }
    else if (t["t"]) { title = object_cast<const char *>(t["t"]); }
    else if (t["lua.string1"]) { title = object_cast<const char *>(t["lua.string1"]); }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["lua.number1"]) { w = object_cast<int>(t["lua.number1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["lua.number2"]) { h = object_cast<int>(t["lua.number2"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }
    else if (t["lua.string2"]) { s = object_cast<const char *>(t["lua.string2"]); }

    if (strstr(s, "fixed")) { style = style & ~wxRESIZE_BORDER; }

    object func = globals(L)["lev"]["classes"]["frame"]["create_c"];
    object frame = func(p, title, w, h, style);
    if (frame)
    {
    }
    frame.push(L);
    return 1;
  }


  void frame::fit()
  {
    ((myFrame *)_obj)->Fit();
  }

  luabind::object frame::get_menubar()
  {
    return cast_frm(_obj)->mb;
  }

  const char * frame::get_status()
  {
    return status;
  }

  const char *frame::get_title()
  {
    const std::string str = (const char *)((wxFrame *)_obj)->GetTitle().mb_str(wxConvUTF8);
    return str.c_str();
  }


  frame *frame::get_top()
  {
    static frame top;
    wxWindow *frm = wxTheApp->GetTopWindow();
    if (frm == NULL) { return NULL; }
    top._obj = frm;
    top.hold();
    return &top;
  }


  bool frame::set_icon(image *i)
  {
    if (i == NULL) { return false; }
    wxBitmap *bmp = (wxBitmap *)i->get_rawobj();
    wxIcon ico;
    ico.CopyFromBitmap(*bmp);
    cast_frm(_obj)->SetIcon(ico);
    return true;
  }

  bool frame::set_menubar(luabind::object mb)
  {
    using namespace luabind;
    menubar *mbar = NULL;
    lua_State *L = mb.interpreter();

    if (not mb) { return false; }
    else if (luabind::type(mb) != LUA_TUSERDATA) { return false; }
    else if (mb["type_id"] != LEV_TMENUBAR) { return false; }
    mbar = object_cast<menubar *>(mb);
    if (mbar == NULL) { return false; }

    cast_frm(_obj)->SetMenuBar((wxMenuBar *)mbar->get_rawobj());
    mbar->hold();
    cast_frm(_obj)->mb = mb;

    luabind::iterator i(mb["menus"]), end;
    for (; i != end; i++)
    {
      luabind::iterator j((*i)["fmap"]);
      for (; j != end; j++)
      {
        if (*j && luabind::type(*j) == LUA_TFUNCTION)
        {
          set_on_menu(object_cast<int>(j.key()), *j);
        }
        else if (*j && luabind::type(*j) == LUA_TSTRING)
        {
          const char *fbase =
            "return function(e)\n"
            "  if %1% then\n"
            "    %1%(e)\n"
            "  else\n"
            "    e:skip()\n"
            "  end\n"
            "end\n";
          std::string func = (boost::format(fbase) % *j).str();
          if (luaL_loadstring(L, func.c_str()) == 0)
          {
            object loaded(from_stack(L, -1));
            lua_pop(L, 1);
            set_on_menu(object_cast<int>(j.key()), loaded());
          }
        }
      }
    }
    return true;
  }

  void frame::set_top(frame *top)
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

  void frame::set_title(const char *title)
  {
    ((wxFrame *)_obj)->SetTitle(wxString(title, wxConvUTF8));
  }


}

