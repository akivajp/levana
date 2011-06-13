require 'lev.image'

red = lev.color(255, 0, 0, 200)
blue = lev.color(0, 0, 255, 128)
img = lev.image.create(640, 480)
img:draw_circle(100, 50, 150, red, blue)
img:draw_text('AAA', 50, 50, 0, lev.color(255, 0, 0))
--img:draw_text('日本語', 50, 50, 0, lev.color(255, 0, 0))
img:save('test.png')

