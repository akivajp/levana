#ifndef _BASE_HPP
#define _BASE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/app.hpp
// Purpose:     header for base class of the all
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/12/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <string>

namespace lev
{

  class base
  {
    public:
      enum type_id
      {
        LEV_TBASE = 1,
          LEV_TCHANNEL,

          LEV_TCONTROL,
            LEV_TAPP,
            LEV_TFRAME,
            LEV_THTMLVIEW,
            LEV_TMENU,
            LEV_TSYSTRAY,
            LEV_TTEXTBOX,
            LEV_TTEXTEDIT,
          LEV_TCONTROL_END,

          LEV_TIMAGE,
          LEV_TINPUT,
            LEV_TINSTATE,
            LEV_TINRECORD,
          LEV_TINPUT_END,
          LEV_TMIXER,

          LEV_TNET_BASE,
            LEV_TACCEPTOR,
            LEV_TADDRESS,
            LEV_TCONNECTOR,
            LEV_TSOCKET,
            LEV_TURL,
          LEV_TNET_BASE_END,

          LEV_TSIZER,
            LEV_TGSIZER,
            LEV_THSIZER,
            LEV_TVSIZER,
          LEV_TSIZER_END,
        LEV_TBASE_END,
      };
    protected:
      base() { }
      virtual ~base() { }
    public:
      virtual type_id get_type_id() const { return LEV_TBASE; }
      virtual const char *get_type_name() const { return "lev.base"; }
      static std::string tostring(base *b);
  };

}

#endif // _BASE_HPP

