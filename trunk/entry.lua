require 'lev/std'

frm = gui.frame()
snd = gui.frame()
player = media.player {parent = snd, w = 100, h = 100}
player:loadlocal("./kami.mp3")
--player:loaduri('http://o-life.jp/f/090421_t10_13_4_r___-.mp3')
frm:fit()
frm:show()

player:play()
player.volume = 0.5
while frm.isvalid do
  frm.status = tostring(player.volume)
  app:yield()
end

