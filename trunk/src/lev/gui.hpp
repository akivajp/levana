/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/gui.hpp
// Purpose:     header for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"

namespace levana
{
  class htmlview: public control
  {
    public:
      htmlview(control *parent, int width, int height);
      ~htmlview();
      bool loadpage(const char *url);
      bool setpage(const char *src);
      const char *totext();
  };

  class text: public control
  {
    public:
      text(control *parent, int width, int height, const char *value = "");
      ~text();
      const char* getvalue();
      void setvalue(const char *value);
  };
}

