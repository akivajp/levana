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
  namespace app
  {
    int autoloop();
    void cleanup();
    bool entry(int argc, char **argv);
    const char* get_name();
    frame *get_top();
    bool init();
    int  msgbox(const char *msg, const char *caption = "Message");
    bool yield();
    void set_name(const char *name);
    void set_top(frame *top);
  }
}

#endif // _APP_HPP

