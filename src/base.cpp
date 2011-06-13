/////////////////////////////////////////////////////////////////////////////
// Name:        src/app.cpp
// Purpose:     source for base class of the all
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     13/06/2011
// Copyright:   (C) 2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/base.hpp"

#include <string>
#include <boost/format.hpp>

namespace lev
{

  std::string base::tostring(base *b)
  {
    return boost::io::str(boost::format("%1%: %2%") % b->get_type_name() % b);
  }

}

