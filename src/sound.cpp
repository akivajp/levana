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

#include <map>
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#include <luabind/luabind.hpp>

namespace lev
{

  class MyChannel
  {
    public:

      enum sound_type {
        SOUND_NONE = 0,
        SOUND_SAMPLE,
        SOUND_STREAM,
      };

      MyChannel() : type(0), snd(NULL), inst(NULL), mixer(NULL) {}

      ~MyChannel() {}

      MyChannel *Create()
      {
        MyChannel *ch = new MyChannel;
        if (ch == NULL) { return NULL; }
        return ch;
      }

      bool Clean()
      {
        switch (type)
        {
          case SOUND_SAMPLE:
            al_destroy_sample_instance( (ALLEGRO_SAMPLE_INSTANCE *)inst );
            al_destroy_sample( (ALLEGRO_SAMPLE *)snd );
            type = SOUND_NONE;
            snd  = NULL;
            inst = NULL;
            break;
          case SOUND_STREAM:
            al_destroy_audio_stream( (ALLEGRO_AUDIO_STREAM *)snd );
            type = SOUND_NONE;
            snd  = NULL;
            inst = NULL;
            break;
          case SOUND_NONE:
          default:
            break;
        }
        return true;
      }

      bool LoadSample(const char *filename)
      {
        ALLEGRO_SAMPLE *sample = NULL;
        ALLEGRO_SAMPLE_INSTANCE *instance = NULL;
        if (mixer == NULL) { return false; }

        sample = al_load_sample(filename);
        if (sample == NULL) { goto Error; }
        instance = al_create_sample_instance(sample);
        if (instance == NULL) { goto Error; }
        if (not al_attach_sample_instance_to_mixer(instance, (ALLEGRO_MIXER *)mixer))
        {
          goto Error;
        }
//        al_set_sample_instance_gain(instance, 1.0);
//        al_set_sample_instance_pan(instance, 0.0);
//        al_set_sample_instance_speed(instance, 1.0);
//        al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_ONCE);
        if (this->snd) { this->Clean(); }
        inst = instance;
        snd  = sample;
        type = SOUND_SAMPLE;
        al_play_sample_instance(instance);
        return true;

        Error:

        if (instance) { al_destroy_sample_instance(instance); }
        if (sample) { al_destroy_sample(sample); }
        return false;
      }

      bool OpenStream(const char *filename)
      {
        ALLEGRO_AUDIO_STREAM *stream = NULL;
        if (mixer == NULL) { return false; }

        stream = al_load_audio_stream(filename, 4, 2048);
        if (stream == NULL) { goto Error; }
        if (not al_attach_audio_stream_to_mixer(stream, (ALLEGRO_MIXER *)mixer))
        {
          goto Error;
        }
        if (this->snd) { this->Clean(); }
        snd = stream;
        type = SOUND_STREAM;
        return true;

        Error:

        if (stream) { al_destroy_audio_stream(stream); }
        return false;
      }

      bool GetPlaying()
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            return al_get_sample_instance_playing((ALLEGRO_SAMPLE_INSTANCE *)this->inst);
          case SOUND_STREAM:
            return al_get_audio_stream_playing((ALLEGRO_AUDIO_STREAM *)this->snd);
          case SOUND_NONE:
          default:
            return false;
        }
      }

      bool SetPlaying(bool play)
      {
        if (this->snd == NULL) { return false; }
        switch (this->type)
        {
          case SOUND_SAMPLE:
            return al_set_sample_instance_playing((ALLEGRO_SAMPLE_INSTANCE *)this->inst, play);
          case SOUND_STREAM:
            return al_set_audio_stream_playing((ALLEGRO_AUDIO_STREAM *)this->snd, play);
          case SOUND_NONE:
          default:
            return false;
        }
      }

      int type;
      void *snd;
      void *inst;
      void *mixer;
  };

  channel::~channel()
  {
    if (_obj) { delete (MyChannel *)_obj; }
  }

  // static member function

  channel* channel::create(void *mx, int id)
  {
    channel *ch = new channel;
    if (ch == NULL) { return NULL; }
    MyChannel *obj = new MyChannel;
    if (obj == NULL) { goto Error; }
    ch->_obj = obj;
    ch->_id = id;
    obj->mixer = mx;
    return ch;

    Error:
    delete ch;
    return NULL;
  }

  bool channel::clean()
  {
    return ((MyChannel *)_obj)->Clean();
  }

  bool channel::is_valid()
  {
    if ( ((MyChannel *)_obj)->snd == NULL ) { return false; }
    return true;
  }

  bool channel::load(const char *filename)
  {
    return ((MyChannel *)_obj)->LoadSample(filename);
  }

  bool channel::open(const char *filename)
  {
    return ((MyChannel *)_obj)->OpenStream(filename);
  }

  bool channel::get_playing()
  {
    return ((MyChannel *)_obj)->GetPlaying();
  }

  bool channel::set_playing(bool play)
  {
    return ((MyChannel *)_obj)->SetPlaying(play);
  }


  class MyMixer
  {
    public:


      MyMixer() : voice(NULL), mixer(NULL), channels() {}

      ~MyMixer()
      {
        std::map<unsigned, channel *>::iterator i;
        for (i = channels.begin(); i != channels.end(); i++)
        {
          if (i->second != NULL) { i->second->clean(); }
        }
        if (voice)
        {
          al_detach_voice(voice);
          al_destroy_voice(voice);
        }
        if (mixer)
        {
          al_destroy_mixer(mixer);
        }
      }

      static MyMixer *Create()
      {
        MyMixer *mx = new MyMixer;
        if (mx == NULL) { return NULL; }

        mx->mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
        if (mx->mixer == NULL) { goto Error; }
        mx->voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
        if (mx->voice == NULL) { goto Error; }
        al_attach_mixer_to_voice(mx->mixer, mx->voice);
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
        channel *ch = channel::create(mixer, channel_num);
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
        return al_get_mixer_playing((ALLEGRO_MIXER *)mixer);
      }

      bool SetPlaying(bool play)
      {
        return al_set_mixer_playing((ALLEGRO_MIXER *)mixer, play);
      }

    private:
      ALLEGRO_VOICE *voice;
      ALLEGRO_MIXER *mixer;
      std::map<unsigned int, channel *> channels;
  };


  mixer::~mixer()
  {
    delete (MyMixer *)_obj;
  }

  // static methods

  mixer* mixer::create()
  {
    if (not sound::init()) { return NULL; }
    mixer *mx = new mixer;
    if (mx == NULL) { return NULL; }
    mx->_obj = MyMixer::Create();
    if (mx->_obj == NULL) { goto Error; }

    return mx;

    Error:
    if (mx) { delete (mixer *)mx; }
    return NULL;
  }

  // member functions


  channel* mixer::get_channel(int channel_num)
  {
    return ((MyMixer *)_obj)->GetChannel(channel_num);
  }

  bool mixer::get_playing()
  {
    return ((MyMixer *)_obj)->GetPlaying();
  }

  bool mixer::set_playing(bool play)
  {
    return ((MyMixer *)_obj)->SetPlaying(play);
  }

  sound::sound() : _items()
  {
    al_init();
    al_install_audio();
    al_init_acodec_addon();
  }

  sound::~sound()
  {
    al_uninstall_audio();
    al_uninstall_system();
  }

  sound* sound::get()
  {
    static sound snd;
    return &snd;
  }

  bool sound::init()
  {
    if (sound::get()) { return true; }
    return false;
  }

  bool sound::play(const char *filename)
  {
    sound *snd = sound::get();
    if (snd->_items.size() == 0)
    {
      sound::init();
      al_reserve_samples(128);
    }
    ALLEGRO_SAMPLE *s = al_load_sample(filename);
    if (s == NULL) { return false; }
    al_play_sample(s, /* gain */ 1.0, /* pan */ 0.0, /* speed */ 1.0,
                   ALLEGRO_PLAYMODE_ONCE, NULL);
    snd->_items.push_back(s);
    return true;
  }

  bool sound::stop()
  {
    sound *snd = sound::get();
    if (snd->_items.size() == 0) { return false; }
    int i;
    for (i = 0; i < snd->_items.size(); i++)
    {
      al_destroy_sample((ALLEGRO_SAMPLE *)snd->_items[i]);
    }
    snd->_items.clear();
    return true;
  }

}

