require 'lev.image'
require 'lev.font'

red   = lev.prim.color(255, 0, 0, 200)
blue  = lev.prim.color(0, 0, 255, 128)
green = lev.prim.color(0, 255, 0, 128)

img = lev.image.create(640, 480)
img:clear(lev.prim.color(255, 0, 0, 128))
img:fill_circle(175, 125, 150, blue)
img:stroke_circle(250, 175, 150, green, 5)
img:draw_text('AAA', lev.font.load('64'), lev.prim.color(255, 0, 0), 50, 50)
img:draw_text('日本語', lev.font.load('64'), lev.prim.color(255, 0, 0), 40, 130)
img:save('test.png')

