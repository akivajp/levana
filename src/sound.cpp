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

//#include <vorbis/vorbisfile.h>

namespace lev
{

  bool sound::play(std::string filename, int channel)
  {
    /*
    OggVorbis_File vf;
    vorbis_info *vi;
    boost::shared_ptr<void> raw_data;
    int pcm_length;

    // opening ogg file, checking validity
    if (ov_fopen((char *)filename.c_str(), &vf) != 0) { return false; }
    vi = ov_info(&vf, -1);
    pcm_length = vi->channels * 2 * ov_pcm_total(&vf, -1);
    raw_data.reset( malloc(pcm_length + 44) );
    if (raw_data.get() == NULL)
    {
      ov_clear(&vf);
      return false;
    }
    else
    {
      // wave header description
      struct wave_header {
        char riff[ 4 ];
        wxUint32 riff_size;
        char wave[ 4 ];
        char fmt[ 4 ];
        wxUint32 fmt_size;
        wxUint16 formatID;
        wxUint16 channel;
        wxUint32 sampling_rate;
        wxUint32 data_size_per_sec;
        wxUint16 block_size;
        wxUint16 bit_per_sample;
        char data[ 4 ];
        wxUint32 pcm_length;
      } *head;

      head = (struct wave_header *)raw_data.get();
      memcpy(head->riff, "RIFF", 4);
      head->riff_size = pcm_length + 44 - 8;
      memcpy(head->wave, "WAVE", 4);
      memcpy(head->fmt , "fmt ", 4);
      head->fmt_size = 16;
      head->formatID = 1;
      head->channel = vi->channels;
      head->sampling_rate = vi->rate;
      head->data_size_per_sec = head->sampling_rate * 2 * head->channel;
      head->block_size = 2 * head->channel;
      head->bit_per_sample = 16;
      memcpy(head->data, "data", 4);
      head->pcm_length = pcm_length;
    }

    int pos = 44;
    int ret;
    do {
      int current;
      ret = ov_read(&vf, (char *)raw_data.get() + pos, pcm_length, 0, 2, 1, &current);
      pos += ret;
    } while (ret != 0);

    wxSound *sound = new wxSound(pcm_length + 44, (wxByte *)raw_data.get());
    ov_clear(&vf);
    if (sound == NULL)
    {
      ov_clear(&vf);
      return false;
    }
    if (sound->IsOk())
    {
      if (channel == 0)
      {
        sound->Play(wxSOUND_SYNC);
        return true;
      }
      else
      {
        sound->Play(wxSOUND_ASYNC);
        return true;
      }
    }
    else { return false; }
    */
  }

}

