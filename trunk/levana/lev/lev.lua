-----------------------------------------------------------------------------
-- Name:        lev.lua
-- Purpose:     levana library re-wrapping for flexible use
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/01/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------
-- Lev Engine

if lev then
  -- nothing to do
else
  package.cpath = package.cpath .. ';./src/?.so;./src/lib?.so'
  require 'levana'

  app   = {}
  lev   = {}

  -------------------------------------------------------------------
  -- begin 'app' table rewrapping
  app.autoloop = levana.app.autoloop

  app.top = function(top)
    if (top == nil) then return levana.app.get_top() end
    levana.app.set_top()
  end

  app.doevents = function(only_if_needed)
    if (only_if_needed == nil) then only_if_needed = false end
    return levana.app.yield(only_if_needed)
  end

  app.name = function(name)
    if (name == nil) then return levana.app.get_name() end
    levana.app.set_name(name)
  end
  -- end of 'app' table rewrapping
  -------------------------------------------------------------------


  -------------------------------------------------------------------
  -- begin 'frame' class rewrapping
  class 'frame'

  function frame:__init(conf_table)
    local c = conf_table
    if (c == nil) then c = {} end
    if (c.id == nil) then c.id = -1 end
    if (c.title == nil) then c.title = "Levana Application" end
    if (c.x == nil) then c.x = -1 end
    if (c.y == nil) then c.y = -1 end
    if (c.w == nil) then c.w = -1 end
    if (c.h == nil) then c.h = -1 end
    if (c.style == nil) then c.style = -1 end
    if (c.name == nil) then c.name = "frame" end
    self.obj =
      levana.frame(c.id, c.title,c.x, c.y, c.w, c.h, c.style, c.name)
  end

  function frame:close(force)
    if (force == nil) then show = false end
    return self.obj:close(force)
  end

  function frame:title(title)
    if (title == nil) then return self.obj:get_title() end
    self.obj:set_title(title)
  end

  function frame:show(show)
    if (show == nil) then show = true end
    return self.obj:show(show)
  end

  lev.frame = frame
  frame = nil
  -- end of 'frame' rewrapping
  -------------------------------------------------------------------

  app.name("Levana Application")
end

