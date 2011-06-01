/////////////////////////////////////////////////////////////////////////////
// Name:        src/sound.cpp
// Purpose:     source for sound control
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     05/13/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "lev/sound.hpp"
#include "register.hpp"

#include <luabind/adopt_policy.hpp>
#include <luabind/luabind.hpp>
#include <map>
#include <SFML/Audio.hpp>

extern "C" {
  int luaopen_sound(lua_State *L)
  {
    using namespace luabind;
    using namespace lev;

    luabind::open(L);

    // base of all
    if (not globals(L)["lev"])
    {
      module(L, "lev")
      [
        class_<base>("base")
          .property("type_id", &base::get_type_id)
          .property("type_name", &base::get_type_name)
      ];
    }

    module(L, "lev")
    [
      namespace_("sound")
      [
        class_<channel, base>("channel")
          .def("clean", &channel::clean)
          .def("load", &channel::load)
          .def("open", &channel::open)
          .def("pause", &channel::pause)
          .def("play", &channel::play)
          .def("play", &channel::play_with)
          .property("id", &channel::get_id)
          .property("is_playing", &channel::get_playing, &channel::set_playing)
          .property("isplaying", &channel::get_playing, &channel::set_playing)
          .property("len", &channel::get_length)
          .property("length", &channel::get_length)
          .property("pan", &channel::get_pan, &channel::set_pan)
          .property("pos", &channel::get_position, &channel::set_position)
          .property("position", &channel::get_position, &channel::set_position),
        class_<mixer, base>("mixer")
          .def("get_channel", &mixer::get_channel)
          .def("pause", &mixer::pause)
          .def("play", &mixer::play)
          .property("is_playing", &mixer::get_playing, &mixer::set_playing)
          .property("isplaying", &mixer::get_playing, &mixer::set_playing)
          .property("type_id", &mixer::get_type_id)
          .property("type_name", &mixer::get_type_name)
          .scope
          [
            def("create_c", &mixer::create, adopt(result))
          ],
        def("init", &sound::init),
        def("play", &sound::play),
        def("stop", &sound::stop_all)
      ]
    ];
    register_to(L, globals(L)["lev"]["sound"]["mixer"], "create", &mixer::create_l);

    return 0;
  }
}

namespace lev
{

  class myChannel
  {
    private:

      enum sound_type {
        SOUND_NONE = 0,
        SOUND_SAMPLE,
        SOUND_STREAM,
      };

      myChannel() : snd(NULL), buf(NULL), type(SOUND_NONE) {}

    public:

      ~myChannel() { Clean(); }

      bool Clean()
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            delete (sf::Sound *)snd;
            snd = NULL;
            delete buf;
            buf = NULL;
            type = SOUND_NONE;
            return true;
          case SOUND_STREAM:
            delete (sf::Music *)snd;
            snd = NULL;
            type = SOUND_NONE;
            return true;
        }
      }

      static myChannel *Create()
      {
        myChannel *ch = new myChannel;
        if (ch == NULL) { return NULL; }
        return ch;
      }

      float GetLength()
      {
        if (this->snd == NULL) { return -1; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            return buf->GetDuration();
          case SOUND_STREAM:
            return ((sf::Music *)snd)->GetDuration();
        }
      }

      float GetPan()
      {
        return 0;
//        if (this->buf == NULL) { return 0; }
//        return playback->get_pan();
      }

      bool GetPlaying()
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            return ( ((sf::Sound *)snd)->GetStatus() == sf::Sound::Playing );
          case SOUND_STREAM:
            return ( ((sf::Music *)snd)->GetStatus() == sf::Music::Playing );
        }
      }

      double GetPosition()
      {
        if (this->snd == NULL) { return -1; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            return ((sf::Sound *)snd)->GetPlayingOffset();
          case SOUND_STREAM:
            return ((sf::Music *)snd)->GetPlayingOffset();
        }
      }

      bool LoadSample(const char *filename)
      {
        sf::SoundBuffer *buffer = NULL;
        sf::Sound *sound = NULL;
        if (this->snd) { this->Clean(); }

        buffer = new sf::SoundBuffer;
        if (buffer == NULL) { goto Error; }
        if (not buffer->LoadFromFile(filename)) { goto Error; }
        sound = new sf::Sound;
        if (sound == NULL) { goto Error; }
        sound->SetBuffer(*buffer);

        this->buf  = buffer;
        this->snd  = sound;
        this->type = SOUND_SAMPLE;
        return true;

        Error:
        if (sound) { delete sound; }
        if (buffer) { delete buffer; }
        return false;
      }

      bool OpenStream(const char *filename)
      {
        sf::Music *music = NULL;
        if (this->snd) { this->Clean(); }

        music = new sf::Music(44100);
        if (music == NULL) { goto Error; }
        if (not music->OpenFromFile(filename)) { goto Error; }

        this->snd  = music;
        this->type = SOUND_STREAM;
        return true;

        Error:
        if (music) { delete music; }
        return false;
      }

      bool Play(const char *filename)
      {
        OpenStream(filename);
        return SetPlaying(true);
      }

      bool SetPan(float pan)
      {
        return false;
//        if (this->buf == NULL) { return false; }
//        playback->set_pan(pan);
//        return true;
      }

      bool SetPlaying(bool play)
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            if (play) { ((sf::Sound *)snd)->Play(); }
            else { ((sf::Sound *)snd)->Pause(); }
            return true;
          case SOUND_STREAM:
            if (play) { ((sf::Music *)snd)->Play(); }
            else { ((sf::Music *)snd)->Pause(); }
            return true;
        }
      }

      bool SetPosition(float pos)
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            ((sf::Sound *)snd)->SetPlayingOffset(pos);
            return true;
          case SOUND_STREAM:
            return false;
//            ((sf::Music *)snd)->SetPlayingOffset(pos);
//            return true;
        }
      }

      void *snd;
      sf::SoundBuffer *buf;
      int type;
  };

  channel::channel() : _obj(NULL) {}
  channel::~channel()
  {
    if (_obj) { delete (myChannel *)_obj; }
  }

  channel* channel::create(int id)
  {
    channel *ch = new channel;
    if (ch == NULL) { return NULL; }
    myChannel *obj = myChannel::Create();
    if (obj == NULL) { goto Error; }
    ch->_obj = obj;
    ch->_id = id;
    return ch;

    Error:
    delete ch;
    return NULL;
  }

  bool channel::clean() { return ((myChannel *)_obj)->Clean(); }
  double channel::get_length() { return ((myChannel *)_obj)->GetLength(); }
  float channel::get_pan() { return ((myChannel *)_obj)->GetPan(); }
  bool channel::get_playing() { return ((myChannel *)_obj)->GetPlaying(); }
  float channel::get_position() { return ((myChannel *)_obj)->GetPosition(); }

  bool channel::load(const char *filename) { return ((myChannel *)_obj)->LoadSample(filename); }
  bool channel::open(const char *filename) { return ((myChannel *)_obj)->OpenStream(filename); }
  bool channel::play_with(const char *filename) { return ((myChannel *)_obj)->Play(filename); }
  bool channel::set_pan(float pan) { return ((myChannel *)_obj)->SetPan(pan); }
  bool channel::set_playing(bool play) { return ((myChannel *)_obj)->SetPlaying(play); }
  bool channel::set_position(float pos) { return ((myChannel *)_obj)->SetPosition(pos); }



  // Mixer Implementation

  class myMixer
  {
    public:


      myMixer() : channels() {}

      ~myMixer()
      {
        std::map<unsigned, channel *>::iterator i;
        for (i = channels.begin(); i != channels.end(); i++)
        {
          if (i->second != NULL) { i->second->clean(); }
        }
      }

      static myMixer *Create()
      {
        myMixer *mx = new myMixer;
        if (mx == NULL) { return NULL; }
        return mx;

        Error:
        delete mx;
        return NULL;
      }

      bool CleanChannel(int channel_num)
      {
        if (channel_num < 0) { return false; }
        std::map<unsigned int, channel *>::iterator found;
        found = channels.find(channel_num);
        if (found != channels.end())
        {
          delete found->second;
          //found->second = NULL;
          channels.erase(found);
          return true;
        }
        else { return false; }
      }

      channel *CreateChannel(int channel_num)
      {
        channel *ch = channel::create(channel_num);
        if (ch == NULL) { return NULL; }
        channels[channel_num] = ch;
        return ch;
      }

      channel *GetChannel(int channel_num)
      {
        if (channel_num < 0) { return NULL; }
        else if (channel_num == 0)
        {
          int i;
          for (i = 1; ; i++)
          {
            std::map<unsigned int, channel *>::iterator found;
            found = channels.find(i);
            if (found == channels.end())
            {
              // channels[i] is not found
              return CreateChannel(i);
            }
            else if (channels[channel_num] == NULL)
            {
              // cannnels[i] is NULL
              return CreateChannel(i);
            }
            else { return channels[i]; }
          }
        }
        else // if (channel_num > 0)
        {
          if (channels[channel_num] == NULL)
          {
            // channels[i] is NULL
            return CreateChannel(channel_num);
          }
          else { return channels[channel_num]; }
        }
      }

      bool GetPlaying()
      {
//        return al_get_mixer_playing((ALLEGRO_MIXER *)mixer);
      }

      bool SetPlaying(bool play)
      {
//        return al_set_mixer_playing((ALLEGRO_MIXER *)mixer, play);
      }

    private:
//      ALLEGRO_VOICE *voice;
//      ALLEGRO_MIXER *mixer;
      std::map<unsigned int, channel *> channels;
  };


  mixer::mixer() : _obj(NULL) { }

  mixer::~mixer()
  {
    if (_obj) { delete (myMixer *)_obj; }
  }

  mixer* mixer::create()
  {
    if (not sound::init()) { return NULL; }
    mixer *mx = new mixer;
    if (mx == NULL) { return NULL; }
    myMixer *obj = myMixer::Create();
    if (obj == NULL) { goto Error; }
    mx->_obj = obj;
    return mx;

    Error:
    delete mx;
    return NULL;
  }

  int mixer::create_l(lua_State *L)
  {
    using namespace luabind;
    object func = globals(L)["lev"]["sound"]["mixer"]["create_c"];
    object mx = func();
    if (mx)
    {
      lua_pushcfunction(L, &mixer::get_field);
      object get_field(from_stack(L, 1));
      lua_pop(L, 1);
      object meta = getmetatable(mx);
      if (not meta["__index2"])
      {
        meta["__index2"] = meta["__index"];
        meta["__index"] = get_field;
      }
    }
    mx.push(L);
    return 1;
  }

  channel* mixer::get_channel(int channel_num)
  {
    return ((myMixer *)_obj)->GetChannel(channel_num);
  }

  int mixer::get_field(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    object mixer = object(from_stack(L, 1));
    object index = object(from_stack(L, 2));

    if (luabind::type(index) == LUA_TNUMBER)
    {
      object func = mixer["get_channel"];
      object channel = func(mixer, index);
      channel.push(L);
      return 1;
    }
    else
    {
      object func = getmetatable(mixer)["__index2"];
      object field = func(mixer, index);
      field.push(L);
      return 1;
    }
  }

  bool mixer::get_playing()
  {
    return ((myMixer *)_obj)->GetPlaying();
  }

  bool mixer::set_playing(bool play)
  {
    return ((myMixer *)_obj)->SetPlaying(play);
  }




  class mySoundManager
  {
    public:
      mySoundManager() : play_list() {}

      ~mySoundManager() { StopAll(); }

      static mySoundManager *Create()
      {
        mySoundManager *mng = new mySoundManager;
        if (mng == NULL) { return NULL; }
        return mng;

        Error:
        delete mng;
        return NULL;
      }

      bool PlayAdd(const char *filename)
      {
        sf::Music *music = new sf::Music(44100);
        if (music == NULL) { return false; }
        if (not music->OpenFromFile(filename)) { goto Error; }
        music->Play();
        play_list.push_back(music);
        return true;

        Error:
        delete music;
        return false;
      }

      bool StopAll()
      {
        int i;
        for (i = 0; i < play_list.size(); i++)
        {
          delete ((sf::Music *)play_list[i]);
        }
        play_list.clear();
        return true;
      }

      std::vector<sf::Music *> play_list;
  };

  sound::sound() : _obj(NULL) { }

  sound::~sound()
  {
    if (_obj) { delete (mySoundManager *)_obj; }
  }

  sound* sound::get()
  {
    static sound snd;
    if (snd._obj) { return &snd; }
    snd._obj = mySoundManager::Create();
    if (snd._obj == NULL) { return NULL; }
    else { return &snd; }
  }

  bool sound::init()
  {
    if (sound::get()) { return true; }
    return false;
  }

  bool sound::play(const char *filename)
  {
    sound *snd = sound::get();
    if (snd == NULL) { return false; }
    return ((mySoundManager *)snd->_obj)->PlayAdd(filename);
  }

  bool sound::stop_all()
  {
    sound *snd = sound::get();
    if (snd == NULL) { return false; }
    return ((mySoundManager *)snd->_obj)->StopAll();
  }
}

