/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/gui.hpp
// Purpose:     header for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"

namespace lev
{
  class htmlview: public control
  {
    public:
      htmlview(control *parent, int width, int height);
      bool loadpage(const char *url);
      bool setpage(const char *src);
      const char *totext();
  };

  class text: public control
  {
    public:
      text(control *parent, int width, int height, const char *value = "");
      const char* getvalue();
      void setvalue(const char *value);
  };
}

