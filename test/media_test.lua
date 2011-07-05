require 'lev'

app = lev.app.get()

frm = lev.gui.frame()
player = lev.gui.player(frm)
frm:show()
if (player:loadlocal('./test.mp3')) then
  player:play()
--  lev.gui.msgbox('OK')
else
--  lev.gui.msgbox('NG')
end

app:autoloop()

