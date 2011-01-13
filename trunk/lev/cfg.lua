-----------------------------------------------------------------------------
-- Name:        lev/cfg.lua
-- Purpose:     config utility
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     01/07/2011
-- Copyright:   (C) 2010-2011 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

cfg = cfg or {}
cfg.frame_style = levana.cfg.frame_style

function cfg.h(h, conf)
  local c = conf or {}
  c.h = h
  return c
end

function cfg.height(height, conf)
  local c = conf or {}
  c.h = height
  c.height = height
  return c
end

function cfg.w(w, conf)
  local c = conf or {}
  c.w = w
  return c
end

function cfg.width(width, conf)
  local c = conf or {}
  c.w = width
  c.width = width
  return c
end

