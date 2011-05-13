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

#include "event.hpp"
#include "prim.hpp"

namespace lev
{

  class player : public control
  {
    public:
      player(control *parent, int width = -1, int height = -1);
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

