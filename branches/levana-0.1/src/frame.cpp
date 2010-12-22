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

#include <string>
#include <map>


namespace levana
{

  class myFrame : public wxFrame
  {
    public:
      myFrame();
      myFrame(wxWindow* parent, wxWindowID id, const wxString &title,
              const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize,
              long style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxT("frame"));
      void Connect(int id, wxEventType eventType, luabind::object lua_func);
//      void Connect(wxEventType eventType, luabind::object lua_func);
      void ProcEvent(wxEvent &event);
    private:
      std::map<int, std::map<int, luabind::object> > event_map;
  };

  myFrame::myFrame()
    : wxFrame(), event_map()
  {}

  myFrame::myFrame(wxWindow *parent, wxWindowID id, const wxString &title,
                   const wxPoint &pos, const wxSize &size,
                   long style, const wxString &name)
    : wxFrame(parent, id, title, pos, size, style, name), event_map()
  {}

  void myFrame::Connect(int id, wxEventType eventType, luabind::object lua_func)
  {
    event_map[eventType][id] = lua_func;
    this->wxFrame::Connect(id, eventType, (wxObjectEventFunction)&myFrame::ProcEvent);
  }

  void myFrame::ProcEvent(wxEvent &event)
  {
    std::map<int, luabind::object> id_map;
    std::map<int, luabind::object>::iterator i;
    id_map = event_map[event.GetEventType()];
    i = id_map.find(-1);
    if (i != id_map.end())
    {
      i->second();
      return;
    }
    i = id_map.find(event.GetId());
    if (i != id_map.end())
    {
      i->second();
      return;
    }
  }

  frame::frame()
  {
    _obj = NULL;
  }

  frame::frame(int id, const char *title,
               int x, int y, int w, int h, long style, const char *name)
  {
    bool success;
    _obj = NULL;
    success = this->create(id, title, x, y, w, h, style, name);
    if (!success)
    {
      throw "allocation error : not enough memory for new frame";
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

  bool frame::create(int id, const char *title,
                     int x, int y, int w, int h, long style, const char *name)
  {
    if (_obj) { this->close(true); }
    wxWindow *p;
    wxPoint pos(x, y);
    wxSize  size(w, y);
    if (x == -1 && y == -1) { pos  = wxDefaultPosition; }
    if (w == -1 && h == -1) { size = wxDefaultSize; }
    if (style == -1) { style = wxDEFAULT_FRAME_STYLE; }

    _obj = new myFrame(NULL, id, wxString(title, wxConvUTF8),
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

