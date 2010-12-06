#ifndef _ENTRY_HPP
#define _ENTRY_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/entry.hpp
// Purpose:     header for binding the C++ library to lua script
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

typedef struct lua_State lua_State;

extern "C"
{
  extern int luaopen_levana(lua_State *L);
}

#endif // _ENTRY_HPP

