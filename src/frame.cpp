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
#include "connect.hpp"

#include <string>
#include <map>

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


  void frame::fit()
  {
    ((myFrame *)_obj)->Fit();
  }


  void frame::seticon(const icon &i)
  {
    ((wxFrame *)_obj)->SetIcon(*((wxIcon *)i._obj.get()));
  }

  void frame::setmenubar(menubar *mb)
  {
    ((wxFrame *)_obj)->SetMenuBar((wxMenuBar *)mb->_obj);
  }

  void frame::setonmenu(int id, luabind::object lua_func)
  {
    ((myFrame *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }


  // statusbar property
  const char * frame::getstatus()
  {
    return status;
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

  frame *frame::gettop()
  {
    static frame top;
    wxWindow *frm = wxTheApp->GetTopWindow();
    if (frm == NULL) { return NULL; }
    top._obj = frm;
    return &top;
  }

  void frame::settop(frame *top)
  {
    wxTheApp->SetTopWindow((wxFrame *)top->_obj);
  }

}

