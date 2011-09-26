-- Package Requirements

require 'lev.gui'
require 'lev.draw'
require 'lev.image'
require 'lev.package'
require 'lev.prim'
require 'lev.sound'
require 'lev.timer'


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


-- Configuration
conf = conf or {}
conf.caption = 'Lyla Scripting System'
conf.fg_color = white
conf.font_size = 20
--conf.font_size = 12
conf.first_load = 'start'
conf.frame_h = 480
conf.frame_w = 640
conf.msg_x = 15
conf.msg_y = 380
conf.msg_w = 600
conf.msg_h = 90
conf.wait_line_icon = 'wait_line.png'
conf.wait_page_icon = 'wait_page.png'

-- Layers

layers_init()
-- background layer
layers_add(lev.image.create(conf.frame_w, conf.frame_h),
           {name = 'bg', on = true, texture = true})
-- message background
layers_add(lev.image.create(conf.frame_w, conf.frame_h),
           {name = 'msgbg', on = true, texture = true})
layers.msgbg:fill_rect(10, 375, 620, 95, lev.prim.color(0, 0, 255, 128))
-- message foreground
layers_add(lev.image.layout(conf.frame_w),
           {name = 'msgfg', on = true, x = conf.msg_x, y = conf.msg_y, compile = true})

layers.msgfg.font.size = conf.font_size
layers.msgfg.color = conf.fg_color or white
-- wait line icon
local icon_path = lev.package.resolve(conf.wait_line_icon)
if not icon_path then
  error(string.format('%s is not found'), conf.wait_line_icon)
end
layers_add(lev.image.load(icon_path.full),
           {name = 'wait_line', on = false, x = 590, y = 440, texture = true})
-- wait page icon
local icon_path = lev.package.resolve(conf.wait_page_icon)
if not icon_path then
  error(string.format('%s is not found'), conf.wait_page_icon)
end
layers_add(lev.image.load(icon_path.full),
           {name = 'wait_page', on = false, x = 590, y = 440, texture = true})

-- Initialize

app = lev.app()
mixer = lev.sound.mixer()

-- Design

main = lev.gui.frame { caption = conf.caption, style = 'fixed' }
canvas = lev.gui.canvas { p = main, w = conf.frame_w, h = conf.frame_h }
main:fit()

-- Controls
if (not lyla.load(conf.first_load)) then
  lev.gui.msgbox(string.format(_'%s is not found!', conf.first_load))
  return -1
end

timer = lev.timer()
timer.notify = function()
  lyla.proc()
end
timer:start(50)


canvas.on_left_down = function()
  if lyla.status == 'wait_key' then
    lyla.key_pressed = true
  end
end

-- Execute

main:show()
canvas:map2d(0, conf.frame_w, 0, conf.frame_h)
canvas:enable_alpha_blending()

while main.is_valid do
  canvas:clear(0, 0, 0)

  for i, j in ipairs(layers) do
    app:yield()
    if j.texture then
      canvas:texturize(j)
    elseif j.compile then
      canvas:compile(j)
    end
    if j.on then
      canvas:draw(j, j.x or 0, j.y or 0)
    end 
  end

  canvas:swap()
  app:wait()
--print()
end

layers.msgfg:save('aaa.png')
print(lyla.history)

