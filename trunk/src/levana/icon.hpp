#ifndef _ICON_HPP
#define _ICON_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/icon.hpp
// Purpose:     Header for icon handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

namespace levana
{
  class icon
  {
    public:
      icon();
      icon(const char **bits);
      ~icon();
      bool load_xpm(const char *filename);
      static const icon &levana_icon();
      friend class frame;
    private:
      void *_obj;
  };
}


#endif // _ICON_HPP

