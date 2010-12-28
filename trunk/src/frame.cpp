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
      inline myFrame() : wxFrame(), fmap() {}

      inline myFrame(wxWindow* parent, wxWindowID id, const wxString &title,
              const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize,
              long style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxT("frame"))
        : wxFrame(parent, id, title, pos, size, style, name), fmap()
      {}

      // Common Connect Interface
      inline void Connect(int id, wxEventType eventType, luabind::object lua_func)
      {
        levana::Connect(this, id, eventType, lua_func);
      }
      void ProcEvent(wxEvent &event)
      {
        levana::ProcEvent(this, event);
      }
      func_map fmap;
  };


  frame::frame(frame *parent, int id, const char *title,
               int x, int y, int w, int h, long style, const char *name)
    : ctrl()
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
    this->seticon(icon::levana_icon());
    this->id = ((myFrame *)_obj)->GetId();
    return true;
  }

  void frame::fit()
  {
    ((myFrame *)_obj)->Fit();
  }

  void frame::seticon(const icon &i)
  {
    ((wxFrame *)_obj)->SetIcon(*((wxIcon *)i._obj));
  }

  void frame::setmenubar(menubar *mb)
  {
    ((wxFrame *)_obj)->SetMenuBar((wxMenuBar *)mb->_obj);
  }
  void frame::setmenubar(frame *f, menubar *mb)
  {
    f->setmenubar(mb);
  }

  void frame::setonmenu(int id, luabind::object lua_func)
  {
    ((myFrame *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }
//  void frame::setonmenu(frame *f, int id, luabind::object lua_func)
//  {
//    f->setonmenu(id, lua_func);
//  }

  bool frame::show(bool showing)
  {
    return ((wxFrame *)_obj)->Show(showing);
  }

  // title property
  const char *frame::gettitle()
  {
    const std::string str = (const char *)((wxFrame *)_obj)->GetTitle().mb_str(wxConvUTF8);
    return str.c_str();
  }
  void frame::settitle(const char *title)
  {
    ((wxFrame *)_obj)->SetTitle(wxString(title, wxConvUTF8));
  }

  // static member functions

  frame *frame::gettop()
  {
    static frame top;
    top._obj = wxTheApp->GetTopWindow();
    if (top._obj == NULL) { return NULL; }
    return &top;
  }

  void frame::settop(frame *top)
  {
    wxTheApp->SetTopWindow((wxFrame *)top->_obj);
  }

}

