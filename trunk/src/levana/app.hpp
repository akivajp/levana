#ifndef _APP_HPP
#define _APP_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/app.hpp
// Purpose:     header for application managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "frame.hpp"

namespace levana
{
  class appli
  {
    public:
      appli();
      ~appli();
      int  autoloop();
      int  msgbox(const char *msg);
      int  msgbox(const char *msg, const char *caption);
      bool yield();
      // name property base
      const char* name_get();
      void name_set(const char *name);
      // top frame property base
      frame *top_get();
      void   top_set(frame *top);
      // static method
      static bool entry(int argc, char **argv);
  };
}

#endif // _APP_HPP

