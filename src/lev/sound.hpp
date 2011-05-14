#ifndef _SOUND_HPP
#define _SOUND_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/sound.hpp
// Purpose:     header for sound control
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     05/13/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <string>

namespace lev
{

  class sound : public base
  {
    public:
      sound *create();
      sound *del();
      static bool play(std::string filename, int channel = 0);
    private:
  };

}

#endif // _SOUND_HPP

