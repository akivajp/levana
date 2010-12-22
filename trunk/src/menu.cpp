/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/menu.hpp"

namespace levana
{

  menu::menu() : base()
  {
    try {
      _obj = new wxMenu();
    }
    catch(...) {
      throw "menu: allocation error";
    }
  }

  menu::menu(const char *title) : base()
  {
    try {
      _obj = new wxMenu(wxString(title, wxConvUTF8));
    }
    catch(...) {
      fprintf(stderr, "menu: allocation error");
    }
  }

  menu::~menu()
  {
    delete (wxMenu *)_obj;
  }

  int menu::append(int id, const char *str, const char *help_str)
  {
    wxString new_str(str, wxConvUTF8);
    wxString new_help(help_str, wxConvUTF8);
    wxMenuItem *item = ((wxMenu *)_obj)->Append(id, new_str, new_help);
    if (item == NULL) { return 0; }
    return item->GetId();
  }
  int menu::append(menu *m, int id, const char *str, const char *help_str)
  {
    return m->append(id, str, help_str);
  }


  menubar::menubar()
  {
    try {
      _obj = new wxMenuBar();
    }
    catch(...) {
      fprintf(stderr, "menubar: allocation error");
    }
  }

  menubar::~menubar()
  {
    delete (wxMenuBar *)_obj;
  }

  bool menubar::append(menu *m, const char *title)
  {
    wxString new_title(title, wxConvUTF8);
    return ((wxMenuBar *)_obj)->Append((wxMenu *)m->_obj, new_title);
  }

}

