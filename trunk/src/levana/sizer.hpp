#ifndef _SIZER_HPP
#define _SIZER_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/sizer.hpp
// Purpose:     header for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"

namespace levana
{
  class sizer : public base
  {
    private:
      sizer();
      ~sizer();
  };

  class hsizer : public sizer
  {
    public:
      hsizer();
      ~hsizer();
  };

  class vsizer : public sizer
  {
    public:
      vsizer();
      ~vsizer();
  };
}

#endif // _SIZER_HPP

