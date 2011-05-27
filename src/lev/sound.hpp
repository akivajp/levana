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

#include <lua.h>
#include <string>
#include <map>
#include <vector>

namespace lev
{

  class channel : public base
  {
    private:
      channel() : _obj(NULL) {}
    public:
      ~channel();
      static channel *create(void *mx, int id);
      bool clean();
      int  get_id() { return _id; }
      bool get_playing();
      bool is_valid();
      bool load(const char *filename);
      bool open(const char *filename);
      bool pause() { return set_playing(false); }
      bool play() { return set_playing(true); }
      bool set_playing(bool play);

      friend class mixer;
    private:
      int _id;
      void *_obj;
  };

  class mixer : public base
  {
    private:
      mixer()  {}
    public:
      ~mixer();
      static mixer *create();
      bool clean_channel(int channel_num);
      channel *get_channel(int channel_num);
      bool get_playing();
      bool pause() { return set_playing(false); }
      bool play() { return set_playing(true); }
      bool set_playing(bool play);
    private:
      void *_obj;
  };

  class sound : public base
  {
    private:
      sound();
      ~sound();
      static sound* get();
    public:
      static bool init();
      static bool play(const char *filename);
      static bool stop();
    private:
      std::vector<void *> _items;
  };

}

#endif // _SOUND_HPP

