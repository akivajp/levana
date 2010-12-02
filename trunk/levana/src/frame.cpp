/////////////////////////////////////////////////////////////////////////////
// Name:        levana/frame.hpp
// Purpose:     source for frame managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/frame.hpp"

#include <string>

class subFrame : wxFrame
{
  public:
    subFrame(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE,
            const wxString& name = wxT("frame"));
};

namespace levana
{

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
    delete (wxFrame *)_obj;
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

    _obj = new wxFrame(NULL, id, wxString(title, wxConvUTF8),
                         pos, size, style, wxString(name, wxConvUTF8));
    if (!_obj) { return false; }
    return true;
  }

  const char *frame::get_title()
  {
    const std::string str = (const char *)((wxFrame *)_obj)->GetTitle().mb_str(wxConvUTF8);
    return str.c_str();
  }

  void frame::set_title(const char *title)
  {
    ((wxFrame *)_obj)->SetTitle(wxString(title, wxConvUTF8));
  }

  bool frame::show(bool bShow)
  {
    return ((wxFrame *)_obj)->Show(bShow);
  }

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

