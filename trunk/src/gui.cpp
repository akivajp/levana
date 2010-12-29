/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/gui.hpp"

namespace levana
{
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
      _obj = new myHtmlWindow(p, width, height);
    }
    catch (...)
    {
      throw "htmlview: allocation error";
    }
  }

  htmlview::~htmlview()
  {
    delete (myHtmlWindow *)_obj;
  }

  bool htmlview::loadpage(const char *url)
  {
    return ((myHtmlWindow *)_obj)->LoadPage(wxString(url, wxConvUTF8));
  }

  bool htmlview::setpage(const char *src)
  {
    return ((myHtmlWindow *)_obj)->SetPage(wxString(src, wxConvUTF8));
  }
}
