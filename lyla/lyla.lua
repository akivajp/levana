require 'string'

local lev = require 'lev'
local _G = _G

module 'lyla'

-- function definitions

function load(filename)
  local real_file
  if lev.fs.file_exists(filename) then
    real_file = filename
  elseif lev.fs.file_exists(filename .. '.lyl') then
    real_file = filename .. '.lyl'
  elseif lev.fs.file_exists(filename .. '.txt') then
    real_file = filename .. '.txt'
  else
    _G.error(_G.string.format('%s{,.lyl,.txt} are not found', filename))
  end
  return real_file 
end

function next()
end

