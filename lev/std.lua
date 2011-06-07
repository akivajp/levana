-----------------------------------------------------------------------------
-- Name:        lev/std.lua
-- Purpose:     script file for standard workspace settings
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/01/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

require 'lev'
require 'lev/gui'

function _(str)
  return str
end

app = lev.app.get()
mixer = lev.sound.mixer.create()

collectgarbage()

