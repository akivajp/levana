#ifndef _MEDIA_HPP
#define _MEDIA_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/media.hpp
// Purpose:     header for media controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/05/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "gui.hpp"
#include "prim.hpp"

#include <lua.h>

namespace lev
{

  class player : public control
  {
    protected:
      player();
    public:
      virtual ~player();
      static player* create(control *parent, int width = -1, int height = -1);
      static int create_l(lua_State *L);
      size getbestsize();
      double getvolume();
      bool ispaused();
      bool isplaying();
      bool isstopped();
      bool loadlocal(const char *filename);
      bool loaduri(const char *uri);
      bool pause();
      bool play();
      bool setvolume(double vol);
      bool stop();
  };

}

#endif // _MEDIA_HPP

