#ifndef _GL_HPP
#define _GL_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/gl.hpp
// Purpose:     header for OpenGL-Lua binding
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

typedef struct lua_State lua_State;

extern "C"
{
  extern int luaopen_gl(lua_State *L);
}

#endif // _GL_HPP

