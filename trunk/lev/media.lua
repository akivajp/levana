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
  if (c.uri == nil) then c.uri = "" end
  if (c.w == nil) then c.w = -1 end
  if (c.h == nil) then c.h = -1 end
  levana.player.__init(self, c.parent, -1, c.uri, c.w, c.h)
end

media.player = player
player = nil
-- end of 'player' class rewrapping
-------------------------------------------------------------------

