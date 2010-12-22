#ifndef _MENU_HPP
#define _MENU_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/frame.hpp
// Purpose:     header for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"

namespace levana
{

  class menu : public base
  {
    public:
      menu();
      menu(const char *title);
      ~menu();
      int append(int id, const char *str, const char *help_str = "");
      static int append(menu *m, int id, const char *str, const char *help_str = "");
      friend class systray;
      friend class menubar;
  };

  class menubar : public base
  {
    public:
      menubar();
      ~menubar();
      bool append(menu *m, const char *title);
      friend class frame;
  };

}

#endif // _MENU_HPP

