#ifndef _ICON_HPP
#define _ICON_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/icon.hpp
// Purpose:     header for icon handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"

namespace levana
{
  class icon : public base
  {
    public:
      inline icon() : base() {}
      icon(const char **bits);
      icon(const char *filename);
      ~icon();
      bool load_xpm(const char *filename);
      static const icon &levana_icon();
      static const icon &levana_icon2();
      friend class frame;
      friend class systray;
  };
}


#endif // _ICON_HPP

