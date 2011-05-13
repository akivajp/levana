-----------------------------------------------------------------------------
-- Name:        lev/std.lua
-- Purpose:     script file including standard libraries
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/01/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

require 'lev/app'
require 'lev/cfg'
require 'lev/gui'
require 'lev/media'

function _(str)
  return str
end

collectgarbage()

