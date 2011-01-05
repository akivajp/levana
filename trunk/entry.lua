require 'lev/std'

frm = gui.frame()
snd = gui.frame()
player = media.player {parent = snd, w = 100, h = 100}
player:load("/home/akiva/levana/kami.mp3")
frm:fit()
frm:show()

player:play()
while not(player:playing()) do end
while player:playing() do
  app:yield()
end

