-- Package Requirements

require 'lev.gui'
require 'lev.draw'
require 'lev.image'
require 'lev.package'
require 'lev.prim'
require 'lev.sound'
require 'lev.timer'
require 'lev.util'


-- Path Settings
lev.package.clear_search()
lev.package.add_search('')
lev.package.add_search('image')
lev.package.add_search('scenario')
lev.package.add_search('system')

-- Lyla Module Requirements

lev.require 'colors'
lev.require 'lyla'
lev.require 'layers'

-- Initialize

app = lev.app()
mixer = lev.sound.mixer()

-- Configuration
conf = conf or {}
conf.caption = 'Lyla Scripting System'
conf.fg_color = white
conf.fps = 50
conf.font_size = 17
conf.first_load = 'start'
conf.frame_h = 480
conf.frame_w = 640
conf.msg_x = 15
conf.msg_y = 380
conf.msg_w = 600
conf.msg_h = 90
conf.save_dir = 'savedata'
conf.save_game = 'log'
conf.save_system = 'system'
conf.sel_x = 15
conf.sel_y = 10
conf.wait_line_icon = 'wait_line.png'
conf.wait_page_icon = 'wait_page.png'

-- Mixer Channels

channels = {}
channels.effect = mixer.ch[0]
channels.voice  = mixer.ch[10]
channels.bgm    = mixer.ch[20]

-- Layers

layers_init()
-- background layer
layers_add(lev.image.transition(),
           {name = 'bg', visible = true, texture = true})
-- message background
layers_add(lev.image.create(conf.frame_w, conf.frame_h),
           {name = 'msg_bg', texture = true, msg = true})
layers.msg_bg.img:fill_rect(10, 375, conf.frame_w - 20, 95, lev.prim.color(0, 0, 255, 128))
-- message foreground
layers_add(lev.image.layout(conf.frame_w - 10),
           {name = 'msg_fg', x = conf.msg_x, y = conf.msg_y, compile = true, msg = true})
layers.msg_fg.img.font.size = conf.font_size
layers.msg_fg.img.color = conf.fg_color or white
-- selection background
layers_add(lev.image.create(conf.frame_w, conf.frame_h),
           {name = 'sel_bg', texture = true, msg = true})
layers.sel_bg.img:fill_rect(10, 5, conf.frame_w - 20, conf.frame_h - 10, lev.prim.color(255, 0, 200, 128))
-- selection foreground
layers_add(lev.image.layout(conf.frame_w - 10),
           {name = 'sel_fg', x = conf.sel_x, y = conf.sel_y, compile = true, msg = true})
layers.sel_fg.img.font.size = conf.font_size
layers.sel_fg.img.color = conf.fg_color or white
msg_activate('msg')
--msg_activate('sel')

-- wait line icon
local icon_path = lev.package.resolve(conf.wait_line_icon)
if not icon_path then
  error(string.format('%s is not found'), conf.wait_line_icon)
end
layers_add(lev.image.load(icon_path.full),
           {name = 'wait_line', visible = false, x = 590, y = 440, texture = true})
-- wait page icon
local icon_path = lev.package.resolve(conf.wait_page_icon)
if not icon_path then
  error(string.format('%s is not found'), conf.wait_page_icon)
end
layers_add(lev.image.load(icon_path.full),
           {name = 'wait_page', visible = false, x = 590, y = 440, texture = true})

-- Design

main = lev.gui.frame { caption = conf.caption, style = 'fixed' }
canvas = lev.gui.canvas { p = main, w = conf.frame_w, h = conf.frame_h }
main:fit()

-- Controls
lyla.init()
--if (not lyla.load(conf.first_load)) then
--  lev.gui.msgbox(string.format(_'%s is not found!', conf.first_load))
--  return -1
--end

timer = lev.timer()
timer.notify = function()
  lyla.proc()
end
timer:start(50)

canvas.on_left_down = function(e)
  -- click waiting process
  if lyla.status == 'wait_key' then
    lyla.key_pressed = true
  end

  -- clickable images' process
  for i,j in ipairs(layers) do
    if j.img.type_name == 'lev.image.layout' then
      local off_x = j.x
      local off_y = j.y
      j.img:on_left_click(e.x - off_x, e.y - off_y)
    end
  end

  -- allow to focus
  e:skip()
end

canvas.on_right_down = function(e)
--  print('right', e.x, e.y)
--  app:yield()
end

canvas.on_motion = function(e)
  for i,j in ipairs(layers) do
    if j.img.type_name == 'lev.image.layout' then
      local off_x = j.x
      local off_y = j.y
      j.img:on_hover(e.x - off_x, e.y - off_y)
    end
  end
end

canvas.on_key_down = function(e)
--  print('down', e.key, e.x, e.y)
--  e:skip()
end

canvas.on_paint = function(e)
  canvas:clear(0, 0, 0)
  for i, j in ipairs(layers) do
    if j.texture then
      canvas:texturize(j.img)
    elseif j.compile then
      canvas:compile(j.img)
    end
    if j.visible and j.img then
      canvas:draw(j.img, j.x or 0, j.y or 0)
    end 
  end
  canvas:swap()
end

-- Execute

main:show()
canvas:map2d(0, conf.frame_w, 0, conf.frame_h)
canvas:enable_alpha_blending()

sw = lev.stop_watch()
app.fps = conf.fps or 50

while main.is_valid do
--  canvas.on_paint()
  canvas:redraw()
  app:wait()
end

lyla.exit()
lyla.save_game()

print(log.history)

