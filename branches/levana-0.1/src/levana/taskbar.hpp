#ifndef _TASKBAR_HPP
#define _TASKBAR_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/taskbar.hpp
// Purpose:     header for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "icon.hpp"

namespace levana
{
  class systray
  {
    public:
      systray();
      ~systray();
      bool set_icon(const icon& i, const char *tooltip = "");
    private:
      void *_obj;
  };
}

#endif // _TASKBAR_HPP

