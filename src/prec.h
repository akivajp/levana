#ifndef _PREC_H
#define _PREC_H

/////////////////////////////////////////////////////////////////////////////
// Name:        src/prec.h
// Purpose:     using for generate pre compiled header
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>

#include <luabind/adopt_policy.hpp>
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>
#include <luabind/operator.hpp>

#include <wx/wx.h>
#include <wx/evtloop.h>
#include <wx/fontdlg.h>
#include <wx/fs_inet.h>
#include <wx/glcanvas.h>
#include <wx/mediactrl.h>
#include <wx/rawbmp.h>
#include <wx/socket.h>
#include <wx/sound.h>
#include <wx/sstream.h>
#include <wx/stc/stc.h>
#include <wx/stdpaths.h>
#include <wx/taskbar.h>
#include <wx/thread.h>
#include <wx/url.h>
#include <wx/wxhtml.h>

#include <GL/glu.h>

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>

#endif // _PREC_H

