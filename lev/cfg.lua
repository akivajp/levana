-----------------------------------------------------------------------------
-- Name:        lev/cfg.lua
-- Purpose:     config utility
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     01/07/2011
-- Copyright:   (C) 2010-2011 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

-- dependency
require 'lev'
lev.load_cfg()
local lev = lev

-- modulize
module 'cfg'

defaults = {
  h = -1,
  w = -1,
  winh = 480,
  winw = 640,
  x = -1,
  y = -1,
}

frame_style = lev.cfg.frame_style

