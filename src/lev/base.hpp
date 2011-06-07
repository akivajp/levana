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
          LEV_TCONTROL_END,
          LEV_TIMAGE,
          LEV_TMIXER,
          LEV_TSIZER,
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
      virtual const char *get_type_name() const { return "base"; }
  };

}

#endif // _BASE_HPP

