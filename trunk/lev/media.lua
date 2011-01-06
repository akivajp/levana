-----------------------------------------------------------------------------
-- Name:        lev/media.lua
-- Purpose:     media controls
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     01/05/2011
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

require 'levana'

if not(media) then
  media = {}
end

-------------------------------------------------------------------
-- begin 'player' class rewrapping
class 'player' (levana.player)

function player:__init(conf)
  local c = conf
  c = c or {}
  if (type(c) ~= 'table') then c.p = c end
  c.p = c.parent or c.p or nil
  c.w = c.width  or c.w or -1
  c.h = c.height or c.h or -1
  levana.player.__init(self, c.p, c.w, c.h)
end

media.player = player
player = nil
-- end of 'player' class rewrapping
-------------------------------------------------------------------

