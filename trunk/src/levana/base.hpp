#ifndef _BASE_HPP
#define _BASE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/app.hpp
// Purpose:     header for base class of the all
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/12/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

namespace levana
{
  class base
  {
    public:
      inline base() : _obj(0) {}
    protected:
      void *_obj;
  };
}



#endif // _BASE_HPP

