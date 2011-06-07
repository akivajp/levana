require 'lev'

app = lev.app.get()

frm = lev.gui.frame.create()
player = lev.gui.player.create(frm)
frm:show()
if (player:loadlocal('kami.mp3')) then
  player:play()
end

app:run()

