require 'lev.image'
require 'lev.package'
require 'lev.timer'
require 'lev.util'

module('tags', package.seeall)

wait_timer = lev.stop_watch()
wait_until = 0

replacers = {'anchor', 'link', 'ruby'}
stoppers = {'br', 'clear', 'cm', 'r', 's'}

function activate(param)
  local name = param.name or param.layer or ''
  msg_activate(name)
end

function anchor(param)
  local text = param.text or param.txt or param[1]
  local href = param.href

  if text then
    local on_click
    if href then
      on_click = function() lev.util.open(href) end
    end
    msg_reserve_clickable('anchor', text, on_click)
    lyla.history = lyla.history .. text
    table.insert(lyla.do_list, function() msg_show_next() end)
  end
end

function br()
  layers.msgfg.img:reserve_new_line()
  layers.msgfg.img:draw_next()
  lyla.history = lyla.history .. '[br]'
end

function clear()
  lyla.history = lyla.history .. '\n'
  msg_clear()
end

function cm()
  lyla.history = lyla.history .. '\n'
  msg_clear()
end

function eval(p)
  local expr = p.expression or p.expr or p.code or p[1] or ''
  if expr then
    local f = loadstring(string.format('return %s', tostring(expr)))
print('code: ', string.format('return %s', tostring(expr)))
print('f: ', f)
    local val
    if f then
      val = f()
    end
print('val: ', val)
    lyla.buffer = tostring(val) .. lyla.buffer
  end
end

function image(param)
  local src = param.src or param.storage or nil
  local layer = param.layer
  local x = param.x
  local y = param.y
  local mode = param.mode or param.trans or ''
  local duration = param.duration or param.time or 0

  if mode == 'fade_out' then
    if layers[layer].img.type_name == 'lev.image.transition' then
      layers[layer].img:set_next(nil, duration, mode)
print('fade out!')
    else
      layers[layer].img = lev.image.transition(layers[layer].img)
      layers[layer].img:set_next(nil, duration, mode)
    end
  end

  if not src or not layer then
    return false
  end
  local src_path = lev.package.resolve(src)
  if not src_path then
    print(string.format('%s is not found', src))
    return false
  end

  if layers[layer] then
    if layers[layer].img.type_name == 'lev.image.transition' then
      layers[layer].img:set_next(src_path.full, duration, mode)
    else
      layers[layer].img = lev.image.transition(src_path.full)
    end
--    layers[layer].img:reload(src_path.full)
--    if x then layers[layer].x = x end
--    if y then layers[layer].y = y end
  end
end

function jump(param)
  local file = param.filename or param.file or param.storage or nil
  local target = param.target
  lyla.load_scenario(file, target)
end

function l()
  lyla.key_pressed = false
  lyla.status = 'wait_key'
  layers.wait_line.visible = true
end

function link(param)
  local src = param.src or param.source or param.storage
  local file = param.filename or param.file or nil
  local target = param.target

  val = lyla.seek_to('[endlink]')
  if #val == 0 then return end

  local on_click
  if target then
    on_click = function() lyla.load_scenario(file, target) end
  end
  msg_reserve_clickable('anchor', val, on_click)
--  lyla.history = lyla.history .. text
  table.insert(lyla.do_list, function() msg_show_next() end)
end

function p()
  lyla.key_pressed = false
  lyla.status = 'wait_key'
  layers.wait_page.visible = true
end

function r()
  msg_reserve_new_line()
  msg_show_next()
  lyla.history = lyla.history .. '[r]'
end

function ruby(param)
  local text = param.text or param[1]
  if text then
    local ch = tostring(lyla.buffer:index(0))
    lyla.buffer = lyla.buffer:sub(1)
    msg_reserve_word(ch, text)
    lyla.history = lyla.history .. string.format('[ruby text="%s"]%s', text, ch)
    table.insert(lyla.do_list, function() msg_show_next() end)
  end
end

function wait(param)
  local delay = param.delay or param[1] or 100
  delay = tonumber(delay)
  if delay then
    wait_timer:start(0)
    wait_until = delay
    lyla.status = 'wait'
  else
  end
end

function s()
  lyla.key_pressed = false
  lyla.status = 'stop'
end

