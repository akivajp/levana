/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for frame managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/frame.hpp"
#include "lev/util.hpp"
#include "connect.hpp"

#include <string>
#include <map>
#include <luabind/luabind.hpp>

using namespace boost;

namespace lev
{

  class myFrame : public wxFrame
  {
    public:
      inline myFrame() : wxFrame() {}
      inline myFrame(wxWindow* parent, const wxString title, int height, int width, long style = wxDEFAULT_FRAME_STYLE)
        : wxFrame(parent, -1, title, wxDefaultPosition, wxSize(height, width), style)
      {}
    private:
      // Common Connect Interface
      DECLARE_CONNECT();
  };


  bool frame::close(bool force)
  {
    if (!_obj) { return false; }
    bool result = ((wxWindow *)_obj)->Close(force);
    if (result)
    {
      _obj = NULL;
      return true;
    }
    return false;
  }

  frame *frame::create(frame *parent, const char *title, int w, int h, long style)
  {
    frame   *newfrm = NULL;
    wxFrame *wxfrm  = NULL;
    try {
      newfrm = new frame();
      wxWindow *p = parent ? (wxWindow *)parent->_obj : NULL;
      if (style == -1) { style = wxDEFAULT_FRAME_STYLE; }
      wxfrm = new myFrame(p, wxString(title, wxConvUTF8), w, h, style);
      newfrm->_id = wxfrm->GetId();
      newfrm->_obj = wxfrm;
      newfrm->seticon(icon::levana_icon());
      return newfrm;
    }
    catch (...) {
      delete wxfrm;
      return NULL;
    }
  }

  int frame::create_l(lua_State *L)
  {
    using namespace luabind;
    frame *p = NULL;
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

    if (t["parent"]) { p = touserdata<frame>(t["parent"]); }
    else if (t["p"]) { p = touserdata<frame>(t["p"]); }
    else if (t["udata"]) { p = touserdata<frame>(t["udata"]); }

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

    object func = globals(L)["lev"]["gui"]["frame"]["create_c"];
    object result = func(p, title, h, w, style);
    result.push(L);
    return 1;
  }


  void frame::fit()
  {
    ((myFrame *)_obj)->Fit();
  }

  const char * frame::getstatus()
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

    frame *frm = (frame *)lua_touserdata(L, 1);
    menubar *mb = (menubar *)lua_touserdata(L, 2);
    if (frm == NULL || mb == NULL) { goto Error; }
    frm->set_menubar(mb);
    // to do
    return 1;

    Error:

    lua_pushboolean(L, false);
    return 1;
  }

  void frame::setonmenu(int id, luabind::object lua_func)
  {
    ((myFrame *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }

  void frame::settop(frame *top)
  {
    wxTheApp->SetTopWindow((wxFrame *)top->_obj);
  }



  void frame::setstatus(const char *str_status)
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

