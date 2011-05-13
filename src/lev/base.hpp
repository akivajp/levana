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

#include <boost/shared_ptr.hpp>

namespace lev
{
  class base
  {
    public:
      enum type
      {
        LEV_TBASE
      };
      inline base() {}
      inline virtual type gettype() const { return LEV_TBASE; }
      inline virtual bool isvalid() const { return bool(_obj); }
    protected:
      boost::shared_ptr<void> _obj;
  };
}



#endif // _BASE_HPP

