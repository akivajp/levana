#ifndef _MEDIA_HPP
#define _MEDIA_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/media.hpp
// Purpose:     header for media controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/05/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"

namespace levana
{

  class player : public control
  {
    public:
      player(control *parent, int id, const char *uri = "", int width = -1, int height = -1);
      ~player();
      bool load(const char *filename);
      bool play();
      bool playing();
  };

}

#endif // _MEDIA_HPP

