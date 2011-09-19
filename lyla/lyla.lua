require 'lev.fs'
require 'lev.lua'
require 'lev.package'
require 'lev.string'

lev.require 'tags'

module('lyla', package.seeall)

-- state variables

buffer = lev.string.create()
content = ''
--pos = 0
msg = ''
status = ''

-- functions


function continue()
  local ch = tostring(buffer:index(0))
  if #ch == 0 then
    return false
  elseif ch == '[' then
    local tag = find_tag('[', ']')
    local code = tag:match('^%[(.*)%]$')
    if code then
      local f = loadstring(code)
      if f then f() end
    else
      local tag_name = tag:match('^([^%s]+)') or ''
      local tag_body = tag:match('^[^%s]+%s(.*)$')
      local params = {}
      if tag_body then
        local params_str = tag_body:gsub('%s+', ',')
        if params_str then params_str = params_str:gsub('%,+', ',') end
        if params_str then params_str = params_str:gsub('%,?%=%,?', '=') end
        if params_str then
          local f = loadstring(string.format('return {%s}', params_str))
          if f then
            params = f() or {}
          end
        end
      end
      
      if tags[tag_name] then
        tags[tag_name](params)
      else
        print(string.format('tags[%s] is not found', tag_name))
      end
      return '[' .. tag .. ']'
    end
  elseif ch == '\n' or ch == '\r' then
    -- nothing to do (no line feed)
    buffer = buffer:sub(1)
    return ''
  else
    msg = msg or ''
    msg = msg .. ch
    buffer = buffer:sub(1)
    return ch
  end
end


-- load file and init
function load(filename)
  local real_file
  local found = lev.package.resolve(filename)
              or lev.package.resolve(filename .. '.lyl')
              or lev.package.resolve(filename .. '.txt')
              or error(string.format('Neither %s.lyl nor %s.txt is not found', filename, filename))

  local infile = io.open(found.full, 'r')
  content = infile:read('*a')
  buffer = lev.string.mb_str(content)
  msg = ''
  status = 'continue'
  return true

--  if lev.fs.file_exists(filename) then
--    real_file = filename
--  elseif lev.fs.file_exists(filename .. '.lyl') then
--    real_file = filename .. '.lyl'
--  elseif lev.fs.file_exists(filename .. '.txt') then
--    real_file = filename .. '.txt'
--  else
--    error(string.format('Neither %s.lyl nor %s.txt is not found', filename, filename))
--    return false
--  end

--  local infile = io.open(real_file, 'r')
--  content = infile:read('*a')
--  buffer = lev.string.mb_str(content)
--  msg = ''
--  status = 'continue'
--  return true 
end


function proc()
  if status == 'continue' then
    return continue()
  elseif status == 'wait' then
    return wait()
  end
end


-- find tag starting with "left" and ending with "right". e.g: [ and ]
-- seek at next position from the correspondence termination
-- return the found string
function find_tag(left, right)
  local found = ''
  local ch = tostring(buffer:index(0))
  if not (ch == left) then
    return found
  end
  buffer = buffer:sub(1)

  local count = 1
  ch = tostring(buffer:index(0))
  buffer = buffer:sub(1)
  while #ch > 0 do
    if ch == left then
      count = count + 1
    elseif ch == right then
      count = count - 1
    end
    if count == 0 then
      return found
    end
    found = found .. ch
    ch = tostring(buffer:index(0))
    buffer = buffer:sub(1)
  end
  return ''
end

function wait()
  if tags.wait_timer.time < tags.wait_until then
    status = 'wait'
  else
    status = 'continue'
  end
end

