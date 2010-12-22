/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for frame managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/frame.hpp"
#include "connect.hpp"

#include <string>
#include <map>

namespace levana
{

  class myFrame : public wxFrame
  {
    public:
      myFrame() : wxFrame(), fmap() {}
      myFrame(wxWindow* parent, wxWindowID id, const wxString &title,
              const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize,
              long style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxT("frame"));
      void Connect(int id, wxEventType eventType, luabind::object lua_func);
//      void Connect(wxEventType eventType, luabind::object lua_func);
      void ProcEvent(wxEvent &event);
      func_map fmap;
//      std::map<int, std::map<int, luabind::object> > fmap;
    private:
//      std::map<int, std::map<int, luabind::object> > event_map;
  };

  myFrame::myFrame(wxWindow *parent, wxWindowID id, const wxString &title,
                   const wxPoint &pos, const wxSize &size,
                   long style, const wxString &name)
    : wxFrame(parent, id, title, pos, size, style, name), fmap()
  {}

  void myFrame::Connect(int id, wxEventType eventType, luabind::object lua_func)
  {
    levana::Connect(this, id, eventType, lua_func);
  }

  void myFrame::ProcEvent(wxEvent &event)
  {
    levana::ProcEvent(this, event);
  }

  frame::frame(frame *parent, int id, const char *title,
               int x, int y, int w, int h, long style, const char *name)
    : base()
  {
    bool success = this->create(parent, id, title, x, y, w, h, style, name);
    if (!success)
    {
      throw "frame: allocation error";
    }
  }

  frame::~frame()
  {
    delete (myFrame *)_obj;
  }

  bool frame::close(bool force)
  {
    if (!_obj) { return false; }
    bool result = ((wxFrame *)_obj)->Close(force);
    if (result)
    {
      _obj = NULL;
      return true;
    }
    return false;
  }

  void frame::connect_menu(int id, luabind::object lua_func)
  {
    ((myFrame *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }

  bool frame::create(frame *parent, int id, const char *title,
                     int x, int y, int w, int h, long style, const char *name)
  {
    if (_obj) { this->close(true); }
    wxWindow *p;
    wxPoint pos(x, y);
    wxSize  size(w, h);
    wxFrame *parent_frm = NULL;
    if (x == -1 && y == -1) { pos  = wxDefaultPosition; }
    if (w == -1 && h == -1) { size = wxDefaultSize; }
    if (style == -1) { style = wxDEFAULT_FRAME_STYLE; }
    if (parent != NULL) { parent_frm = (wxFrame *)parent->_obj; }

    _obj = new myFrame(parent_frm, id, wxString(title, wxConvUTF8),
                         pos, size, style, wxString(name, wxConvUTF8));
    if (!_obj) { return false; }
    this->set_icon(icon::levana_icon());
    return true;
  }

  const char *frame::get_title()
  {
    const std::string str = (const char *)((wxFrame *)_obj)->GetTitle().mb_str(wxConvUTF8);
    return str.c_str();
  }

  void frame::set_icon(const icon &i)
  {
    ((wxFrame *)_obj)->SetIcon(*((wxIcon *)i._obj));
  }

  void frame::set_menubar(menubar *mb)
  {
    ((wxFrame *)_obj)->SetMenuBar((wxMenuBar *)mb->_obj);
  }

  void frame::set_title(const char *title)
  {
    ((wxFrame *)_obj)->SetTitle(wxString(title, wxConvUTF8));
  }

  bool frame::show(bool bShow)
  {
    return ((wxFrame *)_obj)->Show(bShow);
  }

  // static member functions

  frame *frame::get_top()
  {
    static frame top;
    top._obj = wxTheApp->GetTopWindow();
    if (top._obj == NULL) { return NULL; }
    return &top;
  }

  void frame::set_top(frame *top)
  {
    wxTheApp->SetTopWindow((wxFrame *)top->_obj);
  }

}

